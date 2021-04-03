#include <SFML/Network.hpp>
#include <iostream>
#include <atomic>

#include "world.h"
#include "network.h"
#include "viewer.h"

// TODO

// Client class
class Client
{
    const std::string ip;
    const int port;
    float serverTime = 0.f;

    std::atomic<bool> running = false; // Is running, default = false

    World& world; // World pointer

    int clientId = -1; // Client id, default = -1

    sf::Clock clock; // Clock

    sf::TcpSocket socket; // Connection socket

    std::thread syncThread;

    // Client start function
    void start()
    {
        // Connecting socket to server
        auto status = socket.connect(ip, port);
        if (status != sf::Socket::Status::Done)
        {
            std::cout << "Can't connect to server: " << status << "\n";
            return;
        }

        // Main cycle
        while (running)
        {
            // Recieving packets
            sf::Packet packet;
            if (socket.receive(packet) == sf::Socket::Done)
            {
                // Packet type
                int type;
                packet >> type;

                // Client created packet processing
                if (type == Message::ClientCreated)
                {
                    sf::Vector2f pos;
                    packet >> clientId >> pos.x >> pos.y >> serverTime;
                    world.players[clientId] = { pos, sf::Vector2f() };
                    std::cout << "Client created: " << clientId << "\n";
                    clock.restart();
                }
                
                // Update world packet processing
                if (type == Message::UpdateWorld)
                {
                    {
                        int n; // Number of players
                        packet >> n; // Getting number of players from packet
                        std::lock_guard<std::mutex> m(world.m);

                        for (int i = 0; i < n; ++i)
                        {
                            int index; // Player id
                            sf::Vector2f pos, v; // Positon and velocity from server
                            int rad;

                            packet >> index >> pos.x >> pos.y >> v.x >> v.y >> rad;

                            world.players[index].pos = pos; // Updating position for players
                            world.players[index].rad = rad; // Updating radius for players

                            if (index != id())
                            {
                                // Updating velocity for other players
                                // Not for this client!
                                world.players[index].v = v;
                            }
                        }
                    }

                    // Updating target position
                    packet >> world.target.pos.x;
                    packet >> world.target.pos.y;

                    // Getting elapsed server time from packet
                    float ts;
                    packet >> ts;

                    // Updating world
                    world.update(clock.getElapsedTime().asSeconds() - (ts - serverTime));
                }
            }
        }
    }

public:
    // Client constructor
    Client(const std::string& ip, int port, World& world) :
        ip(ip),
        port(port),
        world(world)
    {
        running = true; // Client now running
        syncThread = std::thread(&Client::start, this); // Creating thread
        syncThread.detach(); // Detaching thread
    }

    // Client destructor
    ~Client()
    {
        running = false; // No longer running 
        syncThread.join(); // Joining thread
    }

    // Notify server about velocity function
    void notify()
    {
        // Creating a packet
        sf::Packet packet;

        // This client's player velocity
        const auto& v = world.players[id()].v; 

        // Creating a packet for sending velocity
        packet << Message::Movement << clientId << v.x << v.y;
        // Sending packet
        if (socket.send(packet) != sf::Socket::Done)
        {
            std::cout << "Can't send movement to server\n";
        }
    }

    // Get client id function
    int id() const
    {
        return clientId;
    }

    // Is client running function
    bool isRunning() const
    {
        return running;
    }
};


int main()
{
    // Creating game world
    World world;

    // Creating client (args: ip, port, World)
    Client client("127.0.0.1", 1234, world);

    Viewer viewer("My client"); // Creating viewer
    sf::Clock gameClock; // Game clock
    sf::Vector2f prevVelocity; // Previous player velocity vector

    // Main cycle
    while (viewer.isOpen() && client.isRunning())
    {
        viewer.handleEvents(); // Handling events

        sf::Vector2f v; // Velocity vector
        
        // Checking if keyboard buttons are pressed
        // Getting new velocity (client controls code)
        if (client.id() == 0)
        {
            // Arrow controls for client 0

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                v.x = -Player::MaxSpeed;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                v.x = Player::MaxSpeed;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                v.y = -Player::MaxSpeed;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                v.y = Player::MaxSpeed;
            }
        }
        else
        {
            // WASD controls for client 1

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                v.x = -Player::MaxSpeed;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                v.x = Player::MaxSpeed;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                v.y = -Player::MaxSpeed;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                v.y = Player::MaxSpeed;
            }
        }

        // Changing player's velocity to new velocity
        world.players[client.id()].v = v;

        // Restarting clock and updating world
        const auto dt = gameClock.restart();
        world.update(dt.asSeconds());

        // On any change (speed direction or magnitude) notify server
        if (v != prevVelocity)
        {
            client.notify();
            prevVelocity = v;
        }

        // Drawing world
        viewer.draw(world);
    }

    return 0;
}
