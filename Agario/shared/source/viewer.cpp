#pragma once

#include "viewer.h"

Viewer::Viewer(const std::string& name) : sf::RenderWindow(sf::VideoMode(800, 800), name)
{
    // Loading background sprite
    this->mapTexture;
    if (!this->mapTexture.loadFromFile("kit.jpg"))
    {
        std::cout << "Could not load background texture" << std::endl;
    }
    mapTexture.setSmooth(true);
    this->background.setTexture(mapTexture);

    //Setting frame limit
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

void Viewer::draw(World& world, int my_client_id)
{
    // Clearing window, setting black color
    clear(sf::Color::Black);

    // Drawing background
    sf::RenderWindow::draw(this->background);

    // Colors
    static const sf::Color colors[] = { sf::Color::Red, sf::Color::Green, sf::Color::Blue };

    // Drawing players and targets
    // Real (inside the world) and fake ones (outside the world borders)
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            // Drawing players
            for (auto& it : world.get_players())
            {
                sf::CircleShape s(it.second.get_radius()); // Creating a circle
                s.setOrigin(s.getRadius(), s.getRadius()); // Moving the circle origin to it's center
                int color_number = it.first % 3;
                s.setFillColor(colors[color_number]); // Setting player color

                sf::Vector2f fake_player_pos; // Position for the fake player
                fake_player_pos.x = it.second.get_pos().x + float(i * world.get_size().x);
                fake_player_pos.y = it.second.get_pos().y + float(j * world.get_size().y);

                s.setPosition(fake_player_pos); // Setting fake player position
                sf::RenderWindow::draw(s); // Drawing player in a window
            }

            // Drawing targets
            for (auto& it : world.get_targets())
            {
                sf::CircleShape s(it.second.get_rad()); // Creating a circle
                s.setOrigin(s.getRadius(), s.getRadius()); // Moving the circle origin to it's center
                s.setFillColor(sf::Color::White); // Setting target color

                sf::Vector2f fake_targ_pos; // Position for the fake target
                fake_targ_pos.x = it.second.get_pos().x + float(i * world.get_size().x);
                fake_targ_pos.y = it.second.get_pos().y + float(j * world.get_size().y);

                s.setPosition(fake_targ_pos); // Setting fake target position
                sf::RenderWindow::draw(s); // Drawing fake target in a window
            }
        }
    }

    // Centering view to the player (client code)
    if (my_client_id > -1)
    {
        sf::View gameView;
        gameView.setCenter(world.get_players()[my_client_id].get_pos());
        this->setView(gameView);
    }

    // Displaying the window
    display();
}
