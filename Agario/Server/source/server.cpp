#pragma once
#include "server.h"

Server::Server(int port, World& world) :
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

    world.get_target().new_pos(); // Creating first target

    // Creating and detaching a thread for receiving packets
    syncThread = std::thread(&Server::receive, this);
    syncThread.detach();
}

Server::~Server()
{
    if (running)
    {
        // Stopping server, joining thread
        running = false;
        syncThread.join();
    }
}

bool Server::isRunning() const
{
    return running;
}

void Server::receive()
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
                        unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count(); // Random seed
                        std::default_random_engine generator(seed); // Generator
                        std::uniform_real_distribution<float> distribution_x(float(World::Size.x) * 0.05, float(World::Size.x) * 0.95); // Creating x distribution
                        std::uniform_real_distribution<float> distribution_y(float(World::Size.y) * 0.05, float(World::Size.y) * 0.95); // Creating y distribution
                        sf::Vector2f pos(distribution_x(generator), distribution_y(generator)); // Generating position
                        world.players[currentPlayerId] = { pos, sf::Vector2f() }; // Setting player pos to just generated one

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

void Server::update(float dt)
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
        if (sqrt(pow((world.target.pos.x - it.second.pos.x), 2) + pow((world.target.pos.y - it.second.pos.y), 2)) < it.second.rad)
        {
            world.new_target();
            it.second.rad += 10;
        }
    }
}

void Server::synchronize()
{
    // If server is not dirty
    if (!dirty)
        return;

    dirty = false; // Server not dirty
    std::lock_guard<std::mutex> guard(newPlayerMutex);

    // Forming a packet to send to clients
    sf::Packet toSend;
    toSend << Message::UpdateWorld << world.players.size();
    for (const auto& elem : world.players)
    {
        // Players position and velocity to packet
        toSend << elem.first << elem.second.pos.x << elem.second.pos.y << elem.second.v.x << elem.second.v.y << elem.second.rad;
    }

    // Pushing target position to packet
    toSend << world.target.pos.x;
    toSend << world.target.pos.y;

    // Pushing server elapsed time to packet
    toSend << clock.getElapsedTime().asSeconds();

    // Sending packets to all clients
    for (const auto& elem : sockets)
    {
        if (elem.second->send(toSend) != sf::Socket::Done)
        {
            std::cout << "Can't send update to player " << elem.first << " \n";
        }
    }
}

