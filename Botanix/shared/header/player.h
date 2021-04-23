#pragma once
#include "SFML\System.hpp"
#include "debug.h"

class Player
{
    const float RADIUS = 100.0f; // Player radius
    const int FINAL_SCORE = 1; // Score needed to win the game
    const float SPEED = 300.0f; // Pixels per second, used as a reference to calculate velocity

    int score = 0; // Player score, starting score is 0
    bool winner = false; // Did the player reach endgame score
    bool ready = false; // If player is ready to play (in game lobby)

    sf::Vector2f pos; // Position
    sf::Vector2f v; // Velocity

    sf::Vector2f controls; // Current player controls status

public:
    // Update player position
    void update(float dt);

    // Get maximum player speed
    float get_maxspeed();

    // Get player position
    sf::Vector2f get_pos();

    // Set new player position
    void set_pos(sf::Vector2f new_pos);

    // Get player velocity
    sf::Vector2f get_vel();

    // Get player radius
    float get_rad();

    // Update player velocity to new value
    void set_vel(sf::Vector2f vel);

    // Increase player score
    void increase_score();

    int get_score(); // Get player score
    void set_score(int score); // Set player score

    // Did the player reach the score needed to win
    bool IfWinner();

    // If player is ready to play (in game lobby)
    bool IfReady();

    // Get player controls
    sf::Vector2f get_controls();

    // Set player controls
    void set_controls(sf::Vector2f controls);

    void player_ready(); // Player is ready
    void player_not_ready(); // Player is not ready
};