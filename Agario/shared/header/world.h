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

public:
    std::mutex mutex; // Mutex
    static const sf::Vector2i Size; // World size

    // Update world function
    // (Updates all players positions)
    void update(float dt);

    // Get players map
    std::unordered_map<PlayerId, Player> get_players();

    // Get target function
    Target get_target();
};
