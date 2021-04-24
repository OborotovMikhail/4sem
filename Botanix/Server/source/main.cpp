#include <iostream>
#include <SFML\Graphics.hpp>

#include "server.h"
#include "viewer.h"

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
        if (world.GetScene() != Scene::Gameover)
        {
            viewer.handleEvents(); // Handling events

            auto dt = gameClock.restart(); // Calculating dt
            server.update(dt.asSeconds()); // Updating

            tick += dt;

            // Synchronizing server if needed
            if (tick.asMilliseconds() > 1000 || server.IsDirty())
            {
                server.synchronize();
                tick = sf::Time();
            }

            // Drawing world (server side)
            viewer.draw_gameplay(world);
        }

        if (world.GetScene() == Scene::Gameover)
        {
            viewer.handleEvents(); // Handling events
            viewer.draw_gameover();
        }
    }

    return 0;
}