#pragma once
#include "SFML\System.hpp"

class Player
{
    sf::Vector2f pos; // Position
    sf::Vector2f v; // Velocity
    int rad = 20; // Radius

    int score = 0; // Player score

    float MaxSpeed = 300.0f; // Pixels per second, used as a reference to calculate velocity

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
    int get_rad();

    // Update player velocity to new value
    void set_vel(sf::Vector2f vel);

    // Increase player score
    void increase_score();

    // Get current player score
    int get_score();
};