#pragma once

#include <mutex>
#include <unordered_map>
#include <iostream>
#include <random>

#include "player.h"
#include "target.h"

// Player id type
using PlayerId = int;

// Game world structure
class World
{
    std::unordered_map<PlayerId, Player> players; // Players map
    Target target; // Target class object
    std::mutex m; // Mutex
    static const sf::Vector2i Size; // World size

public:
    // Update world function
    // (Updates all players positions)
    void update(float dt);

    // Get world width (x dim)
    int get_size_x();

    // Get world height (y dim)
    int get_size_y();

    // Get mutex func
    std::mutex get_mutex();
};
