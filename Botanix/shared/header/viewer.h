#pragma once

#include <SFML\Graphics.hpp>

#include "world.h"

class Viewer : public sf::RenderWindow
{
    const int VIEWER_WIDTH = 800;
    const int VIEWER_HEIGHT = 800;

    sf::Texture player_texture;
    sf::Texture target_texture;
    sf::Texture gameover_texture;

    sf::Font font;

    const int NUMBER_OF_LOBBY_BUTTONS = 3;
    std::map<int, sf::Text> lobby_buttons;
    int lobby_selected_button = 0;

public:
    // Viewer constructor
    Viewer(const std::string& name);

    // Handling viewer events function
    void handleEvents();

    // Draw lobby function
    void draw_lobby(World& world);

    // Draw world during the game
    void draw_gameplay(World& world);

    // Draw endgame screen
    void draw_gameover();
    
    // Get selected lobby button
    int get_lobby_selected_button();

    // Set selected lobby button
    void set_lobby_selected_button(int button);

    // Get lobby buttons map
    std::map<int, sf::Text>& get_lobby_buttons();
};
