#pragma once

#include "viewer.h"
    
Viewer::Viewer(const std::string& name) : sf::RenderWindow(sf::VideoMode(800, 800), name)
{
    this->player_texture.loadFromFile("zubko.png");
    this->target_texture.loadFromFile("otl10_white.png");

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
        sf::Sprite player_model(this->player_texture);
        auto player_rect = player_model.getGlobalBounds();
        player_model.setOrigin(player_rect.width / 2.0f, player_rect.height / 2.0f);
        player_model.setPosition(it.second.get_pos());
        sf::RenderWindow::draw(player_model);
    }

    sf::Sprite target_model(this->target_texture);
    auto target_rect = target_model.getGlobalBounds();
    target_model.setOrigin(target_rect.width / 2.0f, target_rect.height / 2.0f);
    target_model.setPosition(world.get_target().get_pos());
    sf::RenderWindow::draw(target_model);

    // Displaying
    display();
}
