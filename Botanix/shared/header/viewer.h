#pragma once

#include <SFML\Graphics.hpp>

#include "world.h"

class Viewer : public sf::RenderWindow
{
    // Some standart values: 800 800, 1920 1080, 3840 2160, 5760 3240, 7680 4320
    const int VIEWER_WIDTH = 800;
    const int VIEWER_HEIGHT = 800;

    const float HERO_SCALE_FACTOR = 0.7f;

    const int NUMBER_OF_HEROES = 3;
    std::map<int, sf::Texture> player_textures; // Map of player textures

    sf::Font font; // Text font

    const int NUMBER_OF_LOBBY_BUTTONS = 3; // Number of buttons in lobby scene
    const float SPACE_BETWEEN_LOBBY_BUTTONS = 0.1f; // Space between lobby buttons (relative to viewer size)
    std::map<int, sf::Text> lobby_buttons; // Map of buttons in lobby scene
    int lobby_selected_button = 0; // Currently selected button in lobby scene

    const int NUMBER_OF_GAMEOVER_BUTTONS = 2; // Number of buttons in gameover scene
    const float SPACE_BETWEEN_GAMEOVER_BUTTONS = 0.4f; // Space between gameover buttons (relative to viewer size)
    std::map<int, sf::Text> gameover_buttons; // Map of buttons in gameover scene
    int gameover_selected_button = 0; // Currently selected button in gameover scene

public:
    Viewer(const std::string& name); // Viewer constructor
    
    void handleEvents(); // Handling viewer events function
    
    void draw_lobby(World& world); // Draw lobby function
    void draw_hero_selection(World& world, int clientId); // Draw hero selection screen
    void draw_gameplay(World& world); // Draw world during the game
    void draw_gameover(World& world); // Draw endgame screen
    void draw_server_full(); // Draw server is full error screen
    void draw_ongoing_game(); // Draw ongoing game error screen
    
    int get_lobby_selected_button(); // Get selected lobby button
    void set_lobby_selected_button(int button); // Set selected lobby button
    
    std::map<int, sf::Text>& get_lobby_buttons(); // Get lobby buttons map

    int get_number_of_heroes();
};
