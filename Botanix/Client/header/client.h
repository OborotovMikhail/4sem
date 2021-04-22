#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <atomic>

#include "network.h"
#include "viewer.h"

class Client
{
    const std::string ip; // Ip adress
    const int port; // Port
    float serverTime = 0.f; // Server time

    std::atomic<bool> running = false; // Is running, default = false

    World& world; // World pointer

    int clientId = -1; // Client id, default = -1

    sf::Clock clock; // Clock

    sf::TcpSocket socket; // Connection socket

    std::thread syncThread;

    // Client start function
    void recieve();

public:
    // Client constructor
    Client(const std::string& ip, int port, World& world);

    // Client destructor
    ~Client();

    // Notify server about controls function
    void notify_server();

    // Get client id function
    int id() const;

    // Is client running function
    bool isRunning() const;

    void disconnect();

    // Handle events in the lobby
    void events_lobby();
};
