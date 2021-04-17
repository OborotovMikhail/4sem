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
                std::cout << "Client created, id: " << clientId << std::endl;
                
                // Printing currently online players
                std::cout << "Online players by their id\'s: ";
                int online_players_num;
                packet >> online_players_num;
                for (int i = 0; i < online_players_num; i++)
                {
                    int id;
                    packet >> id;
                    std::cout << id << " ";
                }
                std::cout << std::endl;

                // Getting target positions from the packet
                int HowManyTargets;
                packet >> HowManyTargets;
                for (int i = 0; i < HowManyTargets; i++)
                {
                    sf::Vector2f targ_pos;
                    packet >> targ_pos.x >> targ_pos.y;
                    world.get_targets()[i].set_pos(targ_pos);
                }

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

                // Getting target positions from the packet
                int HowManyTargets;
                packet >> HowManyTargets;
                for (int i = 0; i < HowManyTargets; i++)
                {
                    sf::Vector2f targ_pos;
                    packet >> targ_pos.x >> targ_pos.y;
                    world.get_targets()[i].set_pos(targ_pos);
                }

                // Getting elapsed server time from packet
                float ts;
                packet >> ts;

                // Updating world
                world.update(clock.getElapsedTime().asSeconds() - (ts - serverTime));
            }

            // Remove player packet processing
            if (type == Message::RemovePlayer)
            {
                int remove_id;
                packet >> remove_id; // Getting player id
                this->world.remove_player(remove_id); // Removing this player

                std::cout << "Player " << remove_id << " disconnected\n";

                this->world.show_players(); // Printing currently online players
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
    packet << Message::ClientMovement << clientId << v.x << v.y;
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
    packet << Message::ClientDisconnect << this->id();

    // Sending packet
    if (socket.send(packet) != sf::Socket::Done)
    {
        std::cout << "Can't send disconnect packet to server\n";
    }
}
