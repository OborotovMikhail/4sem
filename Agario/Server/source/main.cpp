#include <iostream>
#include <SFML\Graphics.hpp>

#include "server.h"

// Server main function
int main()
{
    World world; // Creating world

    Server server(1234, world); // Creating server
    Viewer viewer("My server"); // Creating server viewer

    sf::Clock gameClock;
    sf::Time tick;

    // Main cycle
    while (server.isRunning() && viewer.isOpen())
    {
        viewer.handleEvents(); // Handling events

        auto dt = gameClock.restart(); // Calculating dt
        server.update(dt.asSeconds()); // Updating

        tick += dt;

        // Synchronizing server if needed
        if (tick.asMilliseconds() > 1000 || server.dirty)
        {
            server.synchronize();
            tick = sf::Time();
        }

        // Drawing world (server side)
        viewer.draw(world, -1);
    }

    return 0;
}