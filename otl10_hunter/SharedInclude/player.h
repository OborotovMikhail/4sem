#pragma once
#include <SFML\System.hpp>

// Player structure
// Only includes:
// position, velocity, update position func, max speed value

struct Player
{
    sf::Vector2f pos;
    sf::Vector2f v;
    // Update player position
    void update(float dt)
    {
        pos += v * dt;
    }

    // Max pixels per second
    static const float MaxSpeed;
};
