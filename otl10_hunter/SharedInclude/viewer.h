#pragma once

#include <SFML\Graphics.hpp>

#include "world.h"

// Creating world viewer (displayer) class
class Viewer : public sf::RenderWindow
{
public:
    Viewer(const std::string& name) : sf::RenderWindow(sf::VideoMode(World::Size.x, World::Size.y), name)
    {
        setFramerateLimit(60);
    }

    // Handling events function
    void handleEvents()
    {
        sf::Event event;
        while (pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                close();
        }
    }

    // Draw (display) world function
    void draw(const World& world)
    {
        // Colors
        static const sf::Color colors[] = { sf::Color::Red, sf::Color::Green, sf::Color::Blue };

        // Setting black color as a background
        clear(sf::Color::Black);

        // Drawing players
        for (const auto& it : world.players)
        {
            // Creating circle (player)
            sf::CircleShape s(20);

            s.setFillColor(colors[it.first]); // Setting player color
            s.setPosition(it.second.pos); // Setting player position
            sf::RenderWindow::draw(s); // Drawing player in a window
        }

        // Creating target
        sf::CircleShape target_display(10);
        target_display.setFillColor(sf::Color::White);
        target_display.setPosition(world.target.pos);
        sf::RenderWindow::draw(target_display);

        // Displaying
        display();
    }
};
