#pragma once

#include "viewer.h"

Viewer::Viewer(const std::string& name) : sf::RenderWindow(sf::VideoMode(800, 800), name)
{
    setFramerateLimit(60);
}

void Viewer::handleEvents()
{
    sf::Event event;
    while (pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            close();
    }
}

void Viewer::draw(World& world)
{
    // Colors
    static const sf::Color colors[] = { sf::Color::Red, sf::Color::Green, sf::Color::Blue };

    // Setting black color as a background
    clear(sf::Color::Black);

    // Drawing players
    for (auto& it : world.get_players())
    {
        // Creating circle (player)
        sf::CircleShape s(it.second.get_rad()); // Creating a circle
        s.setOrigin(s.getRadius(), s.getRadius()); // Moving the circle origin to it's center
        s.setFillColor(colors[it.first]); // Setting player color

        // Debug
        std::cout << "player pos when drawing (before): " << it.second.get_pos().x << ' '
            << it.second.get_pos().y << std::endl;

        s.setPosition(it.second.get_pos()); // Setting player position
        sf::RenderWindow::draw(s); // Drawing player in a window
    }

    // Creating target
    sf::CircleShape target_display(10);
    target_display.setOrigin(target_display.getRadius(), target_display.getRadius());
    target_display.setFillColor(sf::Color::White);
    target_display.setPosition(world.get_target().get_pos());
    sf::RenderWindow::draw(target_display);

    // Displaying
    display();
}
