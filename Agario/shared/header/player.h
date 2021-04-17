#pragma once
#include "SFML\System.hpp"
#include "debug.h"

class Player
{
    sf::Vector2f pos; // Position
    sf::Vector2f v; // Velocity
    int rad = 20; // Radius
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

    // Get player's x coordinate
    float get_x();

    // Get player's y coordinate
    float get_y();

    // Get player velocity
    sf::Vector2f get_vel();

    // Get player's x velocity
    float get_x_vel();

    // Get player's y velocity
    float get_y_vel();

    // Get player radius
    int get_rad();

    // Set new player radius
    void set_rad(int new_rad);

    // Update player velocity to new value
    void set_vel(sf::Vector2f vel);

    // Increase player rad
    void increase_rad();

    // Set initial player radius
    void set_initial_rad();
};