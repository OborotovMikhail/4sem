#include <iostream>
#include <SFML\Graphics.hpp>

#include "client.h"
#include "controls.h"

// Client main function
int main()
{
    // Creating game world
    World world;

    // Creating client (args: ip, port, World)
    Client client("127.0.0.1", 1234, world);

    Viewer viewer("My client"); // Creating viewer
    sf::Clock gameClock; // Game clock
    sf::Vector2f prevVelocity; // Previous player velocity vector

    // Main cycle
    while (client.isRunning() && viewer.isOpen())
    {
        viewer.handleEvents(); // Handling events

        // Getting new velocity from keyboard input
        sf::Vector2f v;
        if (client.id() == 0)
        {
            // WASD for player 0
            v = controls_wasd(world.get_players()[client.id()].get_maxspeed());
        }
        else
        {
            // Arrow controls for others
            v = controls_arrows(world.get_players()[client.id()].get_maxspeed());
        }

        // Changing player's velocity to new velocity
        world.get_players()[client.id()].set_vel(v);
            
        // Restarting clock and updating world
        const auto dt = gameClock.restart();
        world.update(dt.asSeconds());

        // On any change (speed direction or magnitude) notify server
        if (v != prevVelocity)
        {
            client.notify_mov();
            prevVelocity = v;
        }

        // Drawing world
        viewer.draw(world, client.id());
    }

    return 0;
}