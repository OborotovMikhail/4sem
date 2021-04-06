#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "network.h"
#include "viewer.h"
#include "safequeue.h"

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

    std::unordered_map<PlayerId, std::unique_ptr<sf::TcpSocket>> sockets; // Sockets map

    SafeQueue<sf::Packet> receivedPackets; // Recieved packets (from clients)

    std::thread syncThread; // Thread
    std::mutex newPlayerMutex; // Player mutex
    sf::Clock clock; // Clock

    std::atomic<bool> dirty = false; // Is server dirty, default = false

    // Server constructor
    Server(int port, World& world);

    // Server destructor
    ~Server();

    // Is server running function
    bool isRunning() const;

    // Receiving packets from players
    void receive();

    // Updating players with data from packets
    void update(float dt);

    // Synchronizing with all clients
    // (sending up-to-date data from server)
    void synchronize();
};