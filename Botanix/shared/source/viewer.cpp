#pragma once

#include "viewer.h"
    
Viewer::Viewer(const std::string& name) : sf::RenderWindow(sf::VideoMode(800, 800), name)
{
    // Loading player textures
    this->player_textures[0].loadFromFile("hero0.png");
    this->player_textures[1].loadFromFile("hero1.png");
    this->player_textures[2].loadFromFile("hero2.png");

    // Loading font
    if (!font.loadFromFile("PressStart2P.ttf"))
    {
        std::cout << "Could not load font" << std::endl;
    }

    setFramerateLimit(60);

    // Setting up lobby buttons
    for (int i = 0; i < NUMBER_OF_LOBBY_BUTTONS; i++)
    {
        lobby_buttons[i].setFont(font);
    }
    lobby_buttons[0].setString("ready");
    lobby_buttons[1].setString("pick hero");
    lobby_buttons[2].setString("disconnect");
    for (int i = 0; i < NUMBER_OF_LOBBY_BUTTONS; i++)
    {
        float total_menu_height = (NUMBER_OF_LOBBY_BUTTONS - 1) * VIEWER_HEIGHT * 0.1f;
        for (int i = 0; i < NUMBER_OF_LOBBY_BUTTONS; i++)
        {
            total_menu_height += lobby_buttons[i].getGlobalBounds().height;
        }

        lobby_buttons[i].setOrigin(lobby_buttons[i].getGlobalBounds().width / 2.0f, lobby_buttons[i].getGlobalBounds().height / 2.0f);
        sf::Vector2f pos;
        pos.x = float(VIEWER_WIDTH) / 2.0f;
        pos.y = float(VIEWER_HEIGHT) / 2.0f + float(i) * float(VIEWER_HEIGHT) * 0.1f
            - total_menu_height / 2.0f;
        lobby_buttons[i].setPosition(pos);
    }
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

void Viewer::draw_hero_selection(World& world, int clientId)
{
    // Setting black color as a background
    clear(sf::Color::Black);

    // Calculatin total menu distance (for centering)
    float total_heroes_width = (NUMBER_OF_HEROES - 1) * VIEWER_WIDTH * 0.1f;
    for (int i = 0; i < (NUMBER_OF_HEROES - 1); i++)
    {
        sf::Sprite sample_hero_sprite(this->player_textures[0]);
        sample_hero_sprite.setScale(HERO_SCALE_FACTOR, HERO_SCALE_FACTOR ); // Scaling

        total_heroes_width += sample_hero_sprite.getGlobalBounds().width;
    }

    // Setting up hero sprites
    for (auto& it : player_textures)
    {
        sf::Sprite hero_sprite(this->player_textures[it.first]); // Set texture
        hero_sprite.scale(HERO_SCALE_FACTOR, HERO_SCALE_FACTOR); // Scaling
        auto hero_rect = hero_sprite.getGlobalBounds();
        hero_sprite.setOrigin(hero_rect.width / HERO_SCALE_FACTOR / 2.0f, 
            hero_rect.height / HERO_SCALE_FACTOR / 2.0f); // Set sprite origin
        
        // Calculating hero sprite position
        sf::Vector2f pos;
        pos.x = float(VIEWER_WIDTH) / 2.0f + float(it.first) * float(VIEWER_WIDTH) * 0.1f
            + float(it.first) * hero_rect.width - total_heroes_width / 2.0f;
        pos.y = float(VIEWER_HEIGHT) / 2.0f;

        hero_sprite.setPosition(pos);

        // Drawing selected hero rectangle
        if (it.first == world.get_players()[clientId].get_selected_hero())
        {
            // Creating outer rectangle
            sf::RectangleShape selected_rect_out;
            selected_rect_out.setSize({ hero_rect.width * 1.1f, hero_rect.height * 1.1f });
            selected_rect_out.setOrigin({ selected_rect_out.getSize().x / 2.0f, selected_rect_out.getSize().y / 2.0f });
            if (!world.get_players()[clientId].isHeroSelected())
            {
                // If selection is not confirmed - white
                selected_rect_out.setFillColor(sf::Color::White);
            }
            else
            {
                // If selection is confirmed - red
                selected_rect_out.setFillColor(sf::Color::Red);
            }   
            selected_rect_out.setPosition(pos);

            // Creating inner rectangle
            sf::RectangleShape selected_rect_in;
            selected_rect_in.setSize({ hero_rect.width * 1.07f, hero_rect.height * 1.07f });
            selected_rect_in.setOrigin({ selected_rect_in.getSize().x / 2.0f, selected_rect_in.getSize().y / 2.0f });
            selected_rect_in.setFillColor(sf::Color::Black);
            selected_rect_in.setPosition(pos);

            sf::RenderWindow::draw(selected_rect_out); // Drawing outer rectangle
            sf::RenderWindow::draw(selected_rect_in); // Drawing inner rectangle
        }

        sf::RenderWindow::draw(hero_sprite); // Draw
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
        sf::Sprite player_model(this->player_textures[it.second.get_selected_hero()]); // Set texture
        auto player_rect = player_model.getGlobalBounds();
        player_model.setOrigin(player_rect.width / 2.0f, player_rect.height / 2.0f); // Set sprite origin
        player_model.setPosition(it.second.get_pos()); // Set position
        player_model.setScale({ 0.7f, 0.7f });
        sf::RenderWindow::draw(player_model); // Draw
    }

    sf::Text target_text;
    target_text.setFont(font);
    target_text.setColor(sf::Color::White);
    target_text.setString("OTL10");
    target_text.setOrigin(target_text.getGlobalBounds().width / 2.0f, target_text.getGlobalBounds().height / 2.0f);
    target_text.setPosition(world.get_target().get_pos());
    sf::RenderWindow::draw(target_text); // Draw

    // Displaying
    display();
}

void Viewer::draw_gameover()
{
    // Setting black color as a background
    clear(sf::Color::Black);

    sf::Text gameover_text;
    gameover_text.setFont(font);
    gameover_text.setColor(sf::Color::White);
    gameover_text.setString("BOTANIX");
    gameover_text.setOrigin(gameover_text.getGlobalBounds().width / 2.0f, gameover_text.getGlobalBounds().height / 2.0f);
    gameover_text.setPosition({ float(VIEWER_WIDTH) / 2.0f, float(VIEWER_HEIGHT) / 2.0f });
    sf::RenderWindow::draw(gameover_text); // Draw

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

int Viewer::get_number_of_heroes()
{
    return this->NUMBER_OF_HEROES;
}
