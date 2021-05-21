#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <atomic>

#include "network.h"
#include "viewer.h"

class Client
{
    std::string ip; // Server ip adress
    int port; // Server ort
    float serverTime = 0.0f; // Server time

    std::atomic<bool> running = false; // Is running, default = false

    World& world; // World pointer

    int clientId = -1; // Client id, default = -1

    sf::Clock clock; // Clock

    sf::TcpSocket socket; // Connection socket

    std::thread syncThread;

    void recieve(); // Client start function

public:
    Client(const std::string& ip, int port, World& world); // Client constructor
    ~Client(); // Client destructor
    
    void notify_server(); // Notify server about new player controls

    int id() const; // Get client id
    
    bool isRunning() const; // Is client running

    void connect(); // Connect to server
    void disconnect(); // Disconnect from the server

    void events_connect(Viewer& viewer); // Handle events in the connect screen
    void events_lobby(Viewer& viewer); // Handle events in the lobby
    void events_hero_selection(Viewer& viewer); // Handle events in the hero selection screen
    void events_gameover(Viewer& viewer); // Handle events in the gameover screen
};
