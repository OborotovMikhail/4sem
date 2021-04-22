#pragma once

#include "viewer.h"
    
Viewer::Viewer(const std::string& name) : sf::RenderWindow(sf::VideoMode(800, 800), name)
{
    this->player_texture.loadFromFile("hero1.png"); // Loading player texture
    this->target_texture.loadFromFile("otl10_white.png"); // Loading target texture
    this->gameover_texture.loadFromFile("botanix_white.png"); // Loading target texture

    if (!font.loadFromFile("PressStart2P.ttf"))
    {
        std::cout << "Could not load font" << std::endl;
    }

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

void Viewer::draw_lobby(World& world)
{
    // Setting black color as a background
    clear(sf::Color::Black);

    ready.setFont(font);
    ready.setColor(sf::Color::White);
    ready.setString("ready");
    ready.setPosition({ float(VIEWER_WIDTH) / 2.0f, float(VIEWER_HEIGHT) / 2.0f });
    sf::RenderWindow::draw(ready);

    // Displaying
    display();
}

void Viewer::draw_gameplay(World& world)
{
    // Setting black color as a background
    clear(sf::Color::Black);

    // Drawing players
    for (auto& it : world.get_players())
    {
        sf::Sprite player_model(this->player_texture); // Set texture
        auto player_rect = player_model.getGlobalBounds();
        player_model.setOrigin(player_rect.width / 2.0f, player_rect.height / 2.0f); // Set sprite origin
        player_model.setPosition(it.second.get_pos()); // Set position
        player_model.setScale({ 0.7f, 0.7f });
        sf::RenderWindow::draw(player_model); // Draw
    }

    sf::Sprite target_model(this->target_texture); // Set texture
    auto target_rect = target_model.getGlobalBounds();
    target_model.setOrigin(target_rect.width / 2.0f, target_rect.height / 2.0f); // Set sprite origin
    target_model.setPosition(world.get_target().get_pos()); // Set position
    target_model.setScale({ 0.7f, 0.7f });
    sf::RenderWindow::draw(target_model); // Draw

    // Displaying
    display();
}

void Viewer::draw_gameover()
{
    // Setting black color as a background
    clear(sf::Color::Black);

    sf::Sprite gameover_sprite(this->gameover_texture); // Set texture
    auto gameover_rect = gameover_sprite.getGlobalBounds();
    gameover_sprite.setOrigin(gameover_rect.width / 2.0f, gameover_rect.height / 2.0f); // Set sprite origin
    gameover_sprite.setPosition({float(this->VIEWER_WIDTH ) / 2.0f, float(this->VIEWER_HEIGHT) / 2.0f }); // Set position
    gameover_sprite.setScale({ 1.0f, 1.0f }); // Set scale
    sf::RenderWindow::draw(gameover_sprite); // Draw

    // Displaying
    display();
}
