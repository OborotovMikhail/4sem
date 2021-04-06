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
    while (viewer.isOpen() && client.isRunning())
    {
        viewer.handleEvents(); // Handling events

        sf::Vector2f v; // Velocity vector

        // Checking if keyboard buttons are pressed
        // Getting new velocity (client controls code)
        
        if (client.id() == 0)
        {
            // Arrow controls for client 0

            // zero movement
            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                && (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) == sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
            {
                v.x = 0;
                v.y = 0;
            }
            // left movement
            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
                && (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) == sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
            {
                v.x = -world.get_players()[client.id()].get_maxspeed();
                v.y = 0;
            }
            // right movement
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
                && (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) == sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
            {
                v.x = world.get_players()[client.id()].get_maxspeed();
                v.y = 0;
            }
            // down movement
            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                v.x = 0;
                v.y = world.get_players()[client.id()].get_maxspeed();
            }
            // up movement
            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                v.x = 0;
                v.y = -world.get_players()[client.id()].get_maxspeed();
            }
            // left-down movement
            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
                && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                v.x = -world.get_players()[client.id()].get_maxspeed();
                v.y = world.get_players()[client.id()].get_maxspeed();
            }
            // left-up movement
            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
                && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                v.x = -world.get_players()[client.id()].get_maxspeed();
                v.y = -world.get_players()[client.id()].get_maxspeed();
            }
            // right-down movement
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
                && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                v.x = world.get_players()[client.id()].get_maxspeed();
                v.y = world.get_players()[client.id()].get_maxspeed();
            }
            // right-up movement
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
                && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                v.x = world.get_players()[client.id()].get_maxspeed();
                v.y = -world.get_players()[client.id()].get_maxspeed();
            }

        }
        else
        {
            // WASD controls for client 1

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                v.x = -world.get_players()[client.id()].get_maxspeed();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                v.x = world.get_players()[client.id()].get_maxspeed();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                v.y = -world.get_players()[client.id()].get_maxspeed();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                v.y = world.get_players()[client.id()].get_maxspeed();
            }
        }
        
        // v = speed_controls_arrows(world.get_players()[client.id()].get_maxspeed());

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
        viewer.draw(world);
    }

    return 0;
}