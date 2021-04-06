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

    world.get_target().set_pos(world.get_random_pos()); // Creating first target

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

                        // Setting random player spawn position
                        world.get_players()[currentPlayerId].set_pos(world.get_random_pos());

                        // Adding new client's socket
                        selector.add(*tempSocket);
                        ++playersConnected;

                        // Creating a spawn packet for the new client
                        sf::Packet outPacket;
                        outPacket << Message::ClientCreated << currentPlayerId << world.get_players()[currentPlayerId].get_x() 
                            << world.get_players()[currentPlayerId].get_y() << clock.getElapsedTime().asSeconds();

                        dirty = true; // Server dirty

                        // Sending a packet to a new client
                        if (tempSocket->send(outPacket) != sf::Socket::Done)
                            std::cout << "Error sending player index" << std::endl;
                        else
                        {
                            std::cout << "Player " << currentPlayerId << " connected\n";
                            std::cout << "Player id " << currentPlayerId << ", spawn pos: " << 
                                world.get_players()[currentPlayerId].get_x() << " " << 
                                world.get_players()[currentPlayerId].get_y() << "\n";
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

        // Processing the "front" packet
        sf::Packet packet = receivedPackets.dequeue();
        packet >> messageType;
        if (messageType == Message::Movement)
        {
            sf::Vector2f v; // Velocity vector
            packet >> clientId >> v.x >> v.y; // Data from packet
            world.get_players()[clientId].set_vel(v); // Updating player velocity
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
        toSend << elem.first << elem.second.get_x() << elem.second.get_x() <<
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

