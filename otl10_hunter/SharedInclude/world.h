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
struct World
{
    // Players map
    std::unordered_map<PlayerId, Player> players;

    // Target struct
    Target target;

    // Mutex
    // (a primitive for controlling threads accessing shared memory)
    std::mutex m;

    // World size
    static const sf::Vector2i Size;

    // Update world function
    // (Updates all players positions)
    void update(float dt)
    {
        std::lock_guard<std::mutex> guard(m);

        // Updating players positions
        for (auto& it : players)
        {
            it.second.update(dt);
        }
    }
};
