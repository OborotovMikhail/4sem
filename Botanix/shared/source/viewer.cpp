#pragma once

#include "viewer.h"
    
Viewer::Viewer(const std::string& name) : sf::RenderWindow(sf::VideoMode(800, 800), name)
{
    this->player_texture.loadFromFile("hero1.png"); // Loading player texture
    this->target_texture.loadFromFile("otl10_white.png"); // Loading target texture
    this->gameover_texture.loadFromFile("botanix_white.png"); // Loading target texture

    // Loading font
    if (!font.loadFromFile("PressStart2P.ttf"))
    {
        std::cout << "Could not load font" << std::endl;
    }

    setFramerateLimit(60);

    // Setting up lobby buttons
    for (int i = 0; i < 3; i++)
    {
        lobby_buttons[i].setFont(font);
        lobby_buttons[i].setOrigin(ready.getGlobalBounds().width / 2.0f, ready.getGlobalBounds().height / 2.0f);
        sf::Vector2f pos;
        pos.x = float(VIEWER_WIDTH) / 2.0f;
        pos.y = float(VIEWER_HEIGHT) / 2.0f + float(i) * float(VIEWER_HEIGHT) * 0.1f;
        lobby_buttons[i].setPosition(pos);
    }
    lobby_buttons[0].setString("ready");
    lobby_buttons[1].setString("pick hero");
    lobby_buttons[2].setString("disconnect");
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

    for (auto& it : lobby_buttons)
    {
        if (it.first == lobby_selected_button)
        {
            it.second.setColor(sf::Color::Red);
        }
        else
        {
            it.second.setColor(sf::Color::White);
        }

        sf::RenderWindow::draw(it.second);
    }

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

int Viewer::get_lobby_selected_button()
{
    return this->lobby_selected_button;
}

void Viewer::set_lobby_selected_button(int button)
{
    this->lobby_selected_button = button;
}

std::map<int, sf::Text>& Viewer::get_lobby_buttons()
{
    return this->lobby_buttons;
}
