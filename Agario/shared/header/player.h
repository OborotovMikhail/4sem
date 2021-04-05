#pragma once
#include "SFML\System.hpp"

class Player
{
    sf::Vector2f pos; // Position
    sf::Vector2f v; // Velocity
    int rad = 20; // Radius
    float MaxSpeed; // Pixels per second, used as a reference to calculate velocity

public:
    // Update player position
    void update(float dt);

    // Get player position
    sf::Vector2f get_pos();

    // Get player radius
    int get_rad();
};