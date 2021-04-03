#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "world.h"
#include "network.h"
#include "viewer.h"
#include "SafeQueue.h"

#include <thread>
#include <iostream>
#include <atomic>

// TODO

class Server
{
public:
    World& world; // World pointer

    std::atomic<bool> running = false; // Is server running

    sf::TcpListener listener; // Listener
    sf::SocketSelector selector; // Socket selector

    const int port; // Server port
    const unsigned short MaxPlayers = 3; // Max allowed players
    int playersConnected = 0; // Number of connected players

    // Sockets map
    std::unordered_map<PlayerId, std::unique_ptr<sf::TcpSocket>> sockets;

    // Recieved packets (from clients)
    // Using thread-safe queue
    SafeQueue<sf::Packet> receivedPackets;

    std::thread syncThread; // Thread
    std::mutex newPlayerMutex; // Player mutex
    sf::Clock clock; // Clock

    std::atomic<bool> dirty = false; // Is server dirty, default = false

    // Server constructor
    Server(int port, World& world) :
        port(port),
        world(world)
    {
        // Starting listener
        if (listener.listen(port) == sf::Socket::Done)
        {
            std::cout << "Server is started on port: " << port << ". Waiting for clients.\n";
            running = true;
        }
        else
        {
            std::cout << "Error - Failed to bind the port " << port << std::endl;
            return;
        }

        // Creating and detaching a thread for receiving packets
        syncThread = std::thread(&Server::receive, this);
        syncThread.detach();

        world.new_target(); // Creating first target
    }

    // Server destructor
    ~Server()
    {
        if (running)
        {
            // Stopping server, joining thread
            running = false;
            syncThread.join();
        }
    }

    // Is server running function
    bool isRunning() const
    {
        return running;
    }

    // Receiving packets from players
    void receive()
    {
        selector.add(listener);

        int currentPlayerId = 0;

        while (isRunning())
        {
            if (selector.wait())
            {
                if (selector.isReady(listener))
                {
                    // Creating the socket that will wait for new connections
                    auto tempSocket = std::make_unique<sf::TcpSocket>();

                    // New client connection
                    // If a new client is connected this code executes
                    if (listener.accept(*tempSocket) == sf::Socket::Done)
                    {
                        if (playersConnected < MaxPlayers) //if server is not full
                        {
                            std::lock_guard<std::mutex> guard(newPlayerMutex);

                            // Setting random spawn position
                            sf::Vector2f pos(float(rand() % World::Size.x), float(rand() % World::Size.y));
                            world.players[currentPlayerId] = { pos, sf::Vector2f() };

                            // Adding new client's socket
                            selector.add(*tempSocket);
                            ++playersConnected;

                            // Creating a spawn packet for the new client
                            sf::Packet outPacket;
                            outPacket << Message::ClientCreated << currentPlayerId << pos.x << pos.y << clock.getElapsedTime().asSeconds();

                            dirty = true; // Server dirty

                            // Sending a packet to a new client
                            if (tempSocket->send(outPacket) != sf::Socket::Done)
                                std::cout << "Error sending player index" << std::endl;
                            else
                            {
                                std::cout << "Player " << currentPlayerId << " connected\n";
                                std::cout << "Player id " << currentPlayerId << ", spawn pos: " << pos.x << " " << pos.y << "\n";
                            }

                            sockets[currentPlayerId] = std::move(tempSocket);
                            ++currentPlayerId;
                        }
                        else
                        {
                            std::cout << "Could not connect new player, server is full\n";
                        }
                    }
                }
                else
                {
                    // Receiving packets from all players
                    for (const std::pair<const PlayerId, Player>& elem : world.players)
                    {
                        auto& socket = *sockets[elem.first]; // Selecting player socket
                        if (selector.isReady(socket))
                        {
                            // Receiving a packet
                            sf::Packet received;
                            if (socket.receive(received) == sf::Socket::Done)
                            {
                                // Pushing received packet to safequeues
                                receivedPackets.enqueue(received);
                            }
                        }
                    }
                }
            }
        }
    }

    // Updating players with data from packets
    void update(float dt)
    {
        while (!receivedPackets.empty())
        {
            int messageType; // Message type
            int clientId; // Client id

            // Processing the "front" packet
            auto packet = receivedPackets.dequeue();
            packet >> messageType;
            if (messageType == Message::Movement)
            {
                sf::Vector2f v; // Velocity vector
                packet >> clientId >> v.x >> v.y; // Data from packet
                world.players[clientId].v = v; // Updating player velocity
                dirty = true; // Server dirty
            }
        }

        // Updating player's positions
        for (auto& it : world.players)
        {
            it.second.update(dt);
        }

        // Checking if anybody reached the target
        for (auto& it : world.players)
        {
            if ((world.target.pos.x > it.second.pos.x - 5) && (world.target.pos.x < it.second.pos.x + 5) && 
                (world.target.pos.y > it.second.pos.y - 5) && (world.target.pos.y < it.second.pos.y + 5))
            {
                world.new_target();
            }
        }
    }

    // Synchronizing with all clients
    // (sending up-to-date data from server)
    void synchronize()
    {
        // If server is not dirty
        if (!dirty)
            return;

        dirty = false; // Server not dirty
        std::lock_guard<std::mutex> guard(newPlayerMutex);

        // Forming a packet to send to clients
        sf::Packet toSend;
        toSend << Message::UpdateWorld << world.players.size();
        for (const auto &elem : world.players)
        {
            toSend << elem.first << elem.second.pos.x << elem.second.pos.y << elem.second.v.x << elem.second.v.y;
        }
        toSend << clock.getElapsedTime().asSeconds();

        // Sending packets to all clients
        for (const auto &elem : sockets)
        {
            if (elem.second->send(toSend) != sf::Socket::Done)
            {
                std::cout << "Can't send update to player " << elem.first << " \n";
            }
        }
    }
};


int main()
{
    World world; // Creating world

    // Viewer viewer("My server"); // Creating server viewer
    Server server(1234, world); // Creating server

    sf::Clock gameClock;
    sf::Time tick;

    // Main cycle
    while (server.isRunning())
    {
        auto dt = gameClock.restart(); // Calculating dt
        server.update(dt.asSeconds()); // Updating

        tick += dt;

        // Synchronizing server if needed
        if (tick.asMilliseconds() > 1000 || server.dirty)
        {
            server.synchronize();
            tick = sf::Time();
        }

        // Drawing world (server side)
        // viewer.draw(world);
    }

    return 0;
}
