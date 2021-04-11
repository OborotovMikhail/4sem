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
                        outPacket << Message::ClientCreated << this->currentPlayerId << world.get_players()[this->currentPlayerId].get_x()
                            << world.get_players()[this->currentPlayerId].get_y() << clock.getElapsedTime().asSeconds();

                        dirty = true; // Server dirty

                        // Sending a packet to a new client
                        if (tempSocket->send(outPacket) != sf::Socket::Done)
                            std::cout << "Error sending player index" << std::endl;
                        else
                        {
                            std::cout << "Player " << this->currentPlayerId << " connected\n";
                            std::cout << "Player id " << this->currentPlayerId << ", spawn pos: " <<
                                world.get_players()[this->currentPlayerId].get_x() << " " <<
                                world.get_players()[this->currentPlayerId].get_y() << "\n";
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
        if (messageType == Message::Movement)
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

            // this->selector.remove(sockets[clientId].get());
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

    // Checking if anybody reached the target
    for (auto& it : world.get_players())
    {
        if (sqrt(pow((world.get_target().get_x() - it.second.get_x()), 2) 
            + pow((world.get_target().get_y() - it.second.get_y()), 2)) < it.second.get_rad())
        {
            world.get_target().set_pos(world.get_random_pos()); // Setting new target pos
            it.second.increase_rad(); // Increasing player radius

            dirty = true; // Server dirty now
        }
    }

    // Checking if any player ate other player
    for (auto& it : world.get_players())
    {
        for (auto& elem : world.get_players())
        {
            if (it.first != elem.first)
            {
                if (sqrt(pow((it.second.get_x() - elem.second.get_x()), 2)
                    + pow((it.second.get_y() - elem.second.get_y()), 2)) < it.second.get_rad())
                {
                    elem.second.set_pos(world.get_random_pos()); // Set random pos for eaten player
                    elem.second.set_initial_rad(); // Set his radius to default one

                    dirty = true; // Server dirty now
                }
            }
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
        toSend << elem.first << elem.second.get_x() << elem.second.get_y() <<
            elem.second.get_x_vel() << elem.second.get_y_vel() << elem.second.get_rad();
    }

    // Pushing target position to packet
    toSend << world.get_target().get_x();
    toSend << world.get_target().get_y();

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

