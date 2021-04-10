#pragma once

#include "client.h"

void Client::start()
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

                world.get_players()[clientId].set_pos(pos);
                std::cout << "Client created: " << clientId << "\n";

                clock.restart();
            }

            // Update world packet processing
            if (type == Message::UpdateWorld)
            {
                {
                    int n; // Number of players
                    packet >> n; // Getting number of players from packet
                    std::lock_guard<std::mutex> m(world.mutex);

                    for (int i = 0; i < n; ++i)
                    {
                        int index; // Player id
                        sf::Vector2f pos, v; // Positon and velocity from server
                        int rad;

                        packet >> index >> pos.x >> pos.y >> v.x >> v.y >> rad;

                        world.get_players()[index].set_pos(pos); // Updating position for players
                        world.get_players()[index].set_rad(rad); // Updating radius for players

                        if (index != id())
                        {
                            // Updating velocity for other players
                            // Not for this client!
                            world.get_players()[index].set_vel(v);
                        }
                    }
                }

                // Updating target position
                sf::Vector2f target_pos;
                packet >> target_pos.x >> target_pos.y;
                world.get_target().set_pos(target_pos);

                // Getting elapsed server time from packet
                float ts;
                packet >> ts;

                // Updating world
                world.update(clock.getElapsedTime().asSeconds() - (ts - serverTime));
            }
        }
    }
}

Client::Client(const std::string& ip, int port, World& world) :
    ip(ip),
    port(port),
    world(world)
{
    running = true; // Client now running
    syncThread = std::thread(&Client::start, this); // Creating thread
    syncThread.detach(); // Detaching thread
}

Client::~Client()
{
    this->disconnect();
    running = false; // No longer running 
    syncThread.join(); // Joining thread
}

void Client::notify_mov()
{
    // Creating a packet
    sf::Packet packet;

    // This client's player velocity
    const auto& v = world.get_players()[id()].get_vel();

    // Creating a packet for sending velocity
    packet << Message::Movement << clientId << v.x << v.y;
    // Sending packet
    if (socket.send(packet) != sf::Socket::Done)
    {
        std::cout << "Can't send movement to server\n";
    }
}

int Client::id() const
{
    return clientId;
}

bool Client::isRunning() const
{
    return running;
}

void Client::disconnect()
{
    // Creating a packet
    sf::Packet packet;

    // Creating a packet for sending velocity
    packet << Message::ClientDisconnect << clientId;

    // Sending packet
    if (socket.send(packet) != sf::Socket::Done)
    {
        std::cout << "Can't send disconnect packet to server\n";
    }
}
