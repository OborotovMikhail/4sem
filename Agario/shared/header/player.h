#pragma once
#include "SFML\System.hpp"

class Player
{
    sf::Vector2f pos; // Position
    sf::Vector2f v; // Velocity
    int rad = 20; // Radius
    float MaxSpeed; // Pixels per second, used as a reference to calculate velocity

public:
    void update(float dt); // Update player position
};