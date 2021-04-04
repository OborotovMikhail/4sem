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

    // Set new target position
    void new_target() {
        unsigned int seed = std::chrono::steady_clock::now().time_since_epoch().count(); // Random seed
        std::default_random_engine generator(seed); // Generator
        std::uniform_real_distribution<float> distribution_x(float(World::Size.x) * 0.05, float(World::Size.x) * 0.95); // Creating x distribution
        std::uniform_real_distribution<float> distribution_y(float(World::Size.y) * 0.05, float(World::Size.y) * 0.95); // Creating y distribution

        target.pos.x = distribution_x(generator); // Generating new x target pos
        target.pos.y = distribution_y(generator); // Generating new y target pos
    }
};
