#pragma once
#include <SFML\System.hpp>

// Player structure
// (Only includes: position, velocity, update position func, max speed value)

struct Player
{
    sf::Vector2f pos; // Position
    sf::Vector2f v; // Velocity

    // Update player position
    void update(float dt)
    {
        pos += v * dt;
    }

    // Max pixels per second
    static const float MaxSpeed;
};
