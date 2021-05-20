#pragma once

#include "viewer.h"
    
Viewer::Viewer(const std::string& name) : sf::RenderWindow(sf::VideoMode(800, 800), 
    name)
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

    // Setting frame limit
    setFramerateLimit(60);

    // DEBUG
    debugcirc.setFillColor(sf::Color::Blue);
    debugcirc.setRadius(5.0f);
    debugcirc.setOrigin(debugcirc.getRadius(), debugcirc.getRadius());
    //debugcirc.setPosition(0.0f, 0.0f);

    // Setting up connect scene textboxes
    for (int i = 0; i < NUMBER_OF_CONNECT_TEXTBOXES; i++)
    {
        connect_textboxes[i].setFont(font);

        connect_textboxes[i].setText("init");

        connect_textboxes[i].setOrigin({ 0.0f, connect_textboxes[i].getGlobalBounds().height / 2.0f });

        sf::Vector2f pos;
        pos.x = float(VIEWER_WIDTH) / 2.0f + float(VIEWER_WIDTH) * SPACE_BETWEEN_CONNECT_BUTTONS / 4.0f;
        pos.y = float(VIEWER_HEIGHT) / 2.0f + float(i) * float(VIEWER_HEIGHT) * SPACE_BETWEEN_CONNECT_BUTTONS
            - float(VIEWER_HEIGHT) * float(CONNECT_MENU_SIZE - 1) * SPACE_BETWEEN_CONNECT_BUTTONS / 2.0f;

        connect_textboxes[i].setPosition(pos);
        connect_textboxes[i].setLimit(true, 10);

        connect_textboxes[i].setText("");
    }

    // Setting up connect scene buttons (buttons near the textboxes)
    connect_buttons[0].setString("nickname:");
    connect_buttons[1].setString("server ip:");
    for (int i = 0; i < 2; i++)
    {
        connect_buttons[i].setFont(font);

        connect_buttons[i].setOrigin(connect_buttons[i].getGlobalBounds().width, connect_buttons[i].getGlobalBounds().height / 2.0f);

        sf::Vector2f pos;
        pos.x = float(VIEWER_WIDTH) / 2.0f - float(VIEWER_WIDTH) * SPACE_BETWEEN_CONNECT_BUTTONS / 4.0f;
        pos.y = float(VIEWER_HEIGHT) / 2.0f + float(i) * float(VIEWER_HEIGHT) * SPACE_BETWEEN_CONNECT_BUTTONS
            - float(VIEWER_HEIGHT) * float(CONNECT_MENU_SIZE - 1) * SPACE_BETWEEN_CONNECT_BUTTONS / 2.0f;

        connect_buttons[i].setPosition(pos);
    }

    // Setting up connect scene buttons (buttons by themselfes)
    connect_buttons[2].setString("connect");
    for (int i = 2; i < NUMBER_OF_CONNECT_BUTTONS; i++)
    {
        connect_buttons[i].setFont(font);
        
        connect_buttons[i].setOrigin(connect_buttons[i].getGlobalBounds().width / 2.0f, connect_buttons[i].getGlobalBounds().height / 2.0f);
        
        sf::Vector2f pos;
        pos.x = float(VIEWER_WIDTH) / 2.0f;
        pos.y = float(VIEWER_HEIGHT) / 2.0f + float(i) * float(VIEWER_HEIGHT) * SPACE_BETWEEN_CONNECT_BUTTONS
            - float(VIEWER_HEIGHT) * float(CONNECT_MENU_SIZE - 1) * SPACE_BETWEEN_CONNECT_BUTTONS / 2.0f;
        
        connect_buttons[i].setPosition(pos);
    }

    // Setting up lobby buttons
    for (int i = 0; i < NUMBER_OF_LOBBY_BUTTONS; i++)
    {
        lobby_buttons[i].setFont(font);
    }
    lobby_buttons[0].setString("ready");
    lobby_buttons[1].setString("pick hero");
    lobby_buttons[2].setString("disconnect");

    float total_lobby_height = (NUMBER_OF_LOBBY_BUTTONS - 1) * VIEWER_HEIGHT * 0.1f;
    for (int i = 0; i < NUMBER_OF_LOBBY_BUTTONS; i++)
    {
        total_lobby_height += lobby_buttons[i].getGlobalBounds().height;
    }

    for (int i = 0; i < NUMBER_OF_LOBBY_BUTTONS; i++)
    {
        lobby_buttons[i].setOrigin(lobby_buttons[i].getGlobalBounds().width / 2.0f, lobby_buttons[i].getGlobalBounds().height / 2.0f);
        sf::Vector2f pos;
        pos.x = float(VIEWER_WIDTH) / 2.0f;
        pos.y = float(VIEWER_HEIGHT) / 2.0f + float(i) * float(VIEWER_HEIGHT) * SPACE_BETWEEN_LOBBY_BUTTONS
            - float(VIEWER_HEIGHT) * float(NUMBER_OF_LOBBY_BUTTONS - 1) * SPACE_BETWEEN_LOBBY_BUTTONS / 2.0f;
        lobby_buttons[i].setPosition(pos);
    }

    // Setting up gameover scene buttons
    for (int i = 0; i < NUMBER_OF_GAMEOVER_BUTTONS; i++)
    {
        gameover_buttons[i].setFont(font);
    }
    gameover_buttons[0].setString("lobby");
    gameover_buttons[1].setString("disconnect");

    float total_gameover_width = float(NUMBER_OF_GAMEOVER_BUTTONS - 1) * VIEWER_WIDTH * 0.1f;
    for (int i = 0; i < NUMBER_OF_GAMEOVER_BUTTONS; i++)
    {
        total_gameover_width += gameover_buttons[i].getGlobalBounds().width;
    }

    for (int i = 0; i < NUMBER_OF_GAMEOVER_BUTTONS; i++)
    {
        gameover_buttons[i].setOrigin(gameover_buttons[i].getGlobalBounds().width / 2.0f, gameover_buttons[i].getGlobalBounds().height / 2.0f);
        
        sf::Vector2f pos;   
        pos.x = float(VIEWER_WIDTH) / 2.0f + float(i) * float(VIEWER_WIDTH) * SPACE_BETWEEN_GAMEOVER_BUTTONS
            - SPACE_BETWEEN_GAMEOVER_BUTTONS / 2.0f * float(NUMBER_OF_GAMEOVER_BUTTONS - 1) * float(VIEWER_WIDTH);
        pos.y = float(VIEWER_HEIGHT) * 0.95f;
        gameover_buttons[i].setPosition(pos);
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

void Viewer::draw_connect(World& world)
{
    // Setting black color as a background
    clear(sf::Color::Black);

    // Drawing textboxes
    for (auto& it : connect_textboxes)
    {
        if (it.first == connect_selected_button)
        {
            it.second.setColor(sf::Color::Red);
        }
        else
        {
            it.second.setColor(sf::Color::White);
        }

        sf::RenderWindow::draw(it.second.getDrawable());
    }

    // Drawing buttons
    for (auto& it : connect_buttons)
    {
        if (it.first == connect_selected_button)
        {
            it.second.setColor(sf::Color::Red);
        }
        else
        {
            it.second.setColor(sf::Color::White);
        }

        sf::RenderWindow::draw(it.second);
    }

    // DEBUG
    sf::RenderWindow::draw(this->debugcirc);

    // Displaying
    display();
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

void Viewer::draw_gameover(World& world)
{
    // Setting black color as a background
    clear(sf::Color::Black);

    sf::Text gameover_text_1;
    gameover_text_1.setFont(font);
    gameover_text_1.setColor(sf::Color::White);
    gameover_text_1.setString("PLAYER " + std::to_string(world.getWinnerId()));
    gameover_text_1.setOrigin(gameover_text_1.getGlobalBounds().width / 2.0f, gameover_text_1.getGlobalBounds().height / 2.0f);
    gameover_text_1.setPosition({ float(VIEWER_WIDTH) / 2.0f, float(VIEWER_HEIGHT) / 2.0f * 0.95f });
    sf::RenderWindow::draw(gameover_text_1); // Draw

    sf::Text gameover_text_2;
    gameover_text_2.setFont(font);
    gameover_text_2.setColor(sf::Color::White);
    gameover_text_2.setString("BOTANIX");
    gameover_text_2.setOrigin(gameover_text_2.getGlobalBounds().width / 2.0f, gameover_text_2.getGlobalBounds().height / 2.0f);
    gameover_text_2.setPosition({ float(VIEWER_WIDTH) / 2.0f, float(VIEWER_HEIGHT) / 2.0f * 1.05f });
    sf::RenderWindow::draw(gameover_text_2); // Draw

    // Drawing gameover scene buttons
    for (auto& it : gameover_buttons)
    {
        if (it.first == gameover_selected_button)
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

void Viewer::draw_server_full()
{
    // Setting black color as a background
    clear(sf::Color::Black);

    sf::Text gameover_text;
    gameover_text.setFont(font);
    gameover_text.setColor(sf::Color::White);
    gameover_text.setString("server is full");
    gameover_text.setOrigin(gameover_text.getGlobalBounds().width / 2.0f, gameover_text.getGlobalBounds().height / 2.0f);
    gameover_text.setPosition({ float(VIEWER_WIDTH) / 2.0f, float(VIEWER_HEIGHT) / 2.0f });
    sf::RenderWindow::draw(gameover_text); // Draw

    // Displaying
    display();
}

void Viewer::draw_ongoing_game()
{
    // Setting black color as a background
    clear(sf::Color::Black);

    sf::Text gameover_text;
    gameover_text.setFont(font);
    gameover_text.setColor(sf::Color::White);
    gameover_text.setString("there is an ongoing game");
    gameover_text.setOrigin(gameover_text.getGlobalBounds().width / 2.0f, gameover_text.getGlobalBounds().height / 2.0f);
    gameover_text.setPosition({ float(VIEWER_WIDTH) / 2.0f, float(VIEWER_HEIGHT) / 2.0f });
    sf::RenderWindow::draw(gameover_text); // Draw

    // Displaying
    display();
}

int Viewer::get_connect_menu_size()
{
    return CONNECT_MENU_SIZE;
}

int Viewer::get_number_of_textboxes()
{
    return NUMBER_OF_CONNECT_TEXTBOXES;
}

int Viewer::get_connect_selected_button()
{
    return this->connect_selected_button;
}

void Viewer::set_connect_selected_button(int button)
{
    this->connect_selected_button = button;
}

std::map<int, sf::Text>& Viewer::get_connect_buttons()
{
    return this->connect_buttons;
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

int Viewer::get_gameover_selected_button()
{
    return this->gameover_selected_button;
}

void Viewer::set_gameover_selected_button(int button)
{
    this->gameover_selected_button = button;
}

std::map<int, sf::Text>& Viewer::get_gameover_buttons()
{
    return this->gameover_buttons;
}

int Viewer::get_number_of_heroes()
{
    return this->NUMBER_OF_HEROES;
}

std::map<int, Textbox>& Viewer::getTextbox()
{
    return this->connect_textboxes;
}
