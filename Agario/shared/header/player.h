#pragma once
#include "SFML\System.hpp"
#include "debug.h"

#include "target.h"

class Player
{
    const float StartingVelocity = 350.0f; // Pixels per second, used as a reference to calculate velocity
    const float StartingRadius = 50;

    sf::Vector2f pos; // Position
    sf::Vector2f v; // Velocity

    int score = 0; // Player score

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

    // Get player score
    int get_score();

    // Set player score
    void set_score(int score);

    // Update player velocity to new value
    void set_vel(sf::Vector2f vel);

    // Increase player score
    void increase_score();

    // Set initial player radius
    void set_initial_score();

    // Get current player radius
    float get_radius();
};