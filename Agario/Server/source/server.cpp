#pragma once

#include "server.h"

Server::Server(int port, World& world) :
    port(port),
    world(world)
{
    this->currentPlayerId = 0;

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

    sf::Vector2f new_pos = world.get_random_pos();

    world.get_target().set_pos(new_pos); // Creating first target

    // Creating targets
    for (int i = 0; i < TargetsNum; i++)
    {
        world.get_targets()[i].set_pos(world.get_random_pos());
    }

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

bool Server::isDirty()
{
    return this->dirty;
}

void Server::receive()
{
    selector.add(listener);

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
                    if (this->world.get_players().size() < MaxPlayers) //if server is not full
                    {
                        std::lock_guard<std::mutex> guard(newPlayerMutex);

                        // Setting random player spawn position
                        sf::Vector2f new_pos = world.get_random_pos();
                        world.get_players()[this->currentPlayerId].set_pos(new_pos);

                        // Adding new client's socket
                        selector.add(*tempSocket);

                        // Creating a spawn packet for the new client
                        sf::Packet outPacket;
                        outPacket << Message::ClientCreated << this->currentPlayerId << world.get_players()[this->currentPlayerId].get_pos().x
                            << world.get_players()[this->currentPlayerId].get_pos().y << clock.getElapsedTime().asSeconds();

                        // Sending online player id's to the new player
                        outPacket << this->world.get_players().size();
                        for (auto& it : this->world.get_players())
                        {
                            outPacket << it.first;
                        }

                        // Sending targets to new player
                        outPacket << world.get_targets().size();
                        for (auto& it : this->world.get_targets())
                        {
                            outPacket << it.second.get_pos().x << it.second.get_pos().y;
                        }

                        dirty = true; // Server dirty

                        // Sending a packet to a new client
                        if (tempSocket->send(outPacket) != sf::Socket::Done)
                            std::cout << "Error sending player index" << std::endl;
                        else
                        {
                            std::cout << "Player " << this->currentPlayerId << " connected (spawn position: "
                            << world.get_players()[this->currentPlayerId].get_pos().x << " " <<
                                world.get_players()[this->currentPlayerId].get_pos().y << ")\n";
                        }

                        sockets[this->currentPlayerId] = std::move(tempSocket);
                        this->currentPlayerId++;

                        this->world.show_players();
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
                for (const std::pair<const PlayerId, Player>& elem : world.get_players())
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

        // Getting the "front" packet
        sf::Packet packet = receivedPackets.dequeue();
        packet >> messageType;

        // Processing movement packets
        if (messageType == Message::ClientMovement)
        {
            sf::Vector2f v; // Velocity vector
            packet >> clientId >> v.x >> v.y; // Data from packet
            world.get_players()[clientId].set_vel(v); // Updating player velocity
            dirty = true; // Server dirty
        }

        // Processing disconnect packets
        if (messageType == Message::ClientDisconnect)
        {
            packet >> clientId;

            sf::Packet toSend;
            toSend << Message::RemovePlayer << clientId; // Forming packet

            // Sending to everybody except the player who is disconnecting
            for (const auto& elem : sockets)
            {
                if (elem.first != clientId)
                {
                    if (elem.second->send(toSend) != sf::Socket::Done)
                    {
                        std::cout << "Can't send disconnect packet to player " << elem.first << " \n";
                    }
                }
            }

            this->selector.remove(*sockets[clientId].get());
            this->world.remove_player(clientId); // Removing player from the world
            this->sockets.erase(clientId); // Removing player socket

            std::cout << "Player " << clientId << " disconnected\n";
            this->world.show_players();

            dirty = true; // Server dirty
        }
    }

    // Updating player's positions
    for (auto& it : world.get_players())
    {
        it.second.update(dt);
    }

    // Checking for players eating targets and other players
    // (inside the world and through the world borders)
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            // Players eating targets
            for (auto& it : world.get_players())
            {
                for (auto& elem : world.get_targets())
                {
                    if (sqrt(pow((elem.second.get_pos().x - it.second.get_pos().x
                        + float(i * world.get_size().x)), 2)
                        + pow((elem.second.get_pos().y - it.second.get_pos().y
                            + float(j * world.get_size().y)), 2)) < it.second.get_radius())
                    {
                        elem.second.set_pos(world.get_random_pos()); // Setting new target pos
                        it.second.increase_score(); // Increasing player radius
                        debug("Ate targ, new score/rad:", it.second.get_score(), it.second.get_radius(),
                            it.second.get_maxspeed());

                        dirty = true; // Server dirty now
                    }
                }
            }

            // Players eating other players
            for (auto& it : world.get_players())
            {
                for (auto& elem : world.get_players())
                {
                    if (it.first != elem.first)
                    {
                        if (sqrt(pow((it.second.get_pos().x - elem.second.get_pos().x
                            + float(i * world.get_size().x)), 2)
                            + pow((it.second.get_pos().y - elem.second.get_pos().y
                                + float(j * world.get_size().y)), 2)) < it.second.get_radius())
                        {
                            elem.second.set_pos(world.get_random_pos()); // Set random pos for eaten player
                            elem.second.set_initial_score(); // Set his radius to default one

                            dirty = true; // Server dirty now
                        }
                    }
                }
            }
        }
    }

    // Checking if any player is crossing the world border
    for (auto& it : world.get_players())
    {
        // Checking left border
        if (it.second.get_pos().x < 0)
        {
            sf::Vector2f new_pos;
            new_pos.x = it.second.get_pos().x + float(world.get_size().x);
            new_pos.y = it.second.get_pos().y;
            it.second.set_pos(new_pos);

            dirty = true; // Server dirty now
        }

        // Checking right border
        if (it.second.get_pos().x > float(world.get_size().x))
        {
            sf::Vector2f new_pos;
            new_pos.x = it.second.get_pos().x - float(world.get_size().x);
            new_pos.y = it.second.get_pos().y;
            it.second.set_pos(new_pos);

            dirty = true; // Server dirty now
        }

        // Checking top border
        if (it.second.get_pos().y < 0)
        {
            sf::Vector2f new_pos;
            new_pos.x = it.second.get_pos().x;
            new_pos.y = it.second.get_pos().y + float(world.get_size().y);
            it.second.set_pos(new_pos);

            dirty = true; // Server dirty now
        }

        // Checking bottom border
        if (it.second.get_pos().y > float(world.get_size().y))
        {
            sf::Vector2f new_pos;
            new_pos.x = it.second.get_pos().x;
            new_pos.y = it.second.get_pos().y - float(world.get_size().y);
            it.second.set_pos(new_pos);

            dirty = true; // Server dirty now
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
    toSend << Message::UpdateWorld << world.get_players().size();
    for (auto& elem : world.get_players())
    {
        // Players position and velocity to packet
        toSend << elem.first << elem.second.get_pos().x << elem.second.get_pos().y <<
            elem.second.get_vel().x << elem.second.get_vel().y << elem.second.get_score();
    }

    // Sending targets to new player
    toSend << world.get_targets().size();
    for (auto& it : this->world.get_targets())
    {
        toSend << it.second.get_pos().x << it.second.get_pos().y;
    }

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

