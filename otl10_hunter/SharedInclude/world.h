#pragma once

#include <mutex>
#include <unordered_map>
#include <iostream>

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
        // sf::Vector2f pos(float(rand() % 800), float(rand() % 800));
        // std::cout << pos.x << ' ' << pos.y;
        // target.pos = pos;

        target.pos.x = float(rand() * World::Size.x / RAND_MAX);
        target.pos.y = float(rand() * World::Size.y / RAND_MAX);
        std::cout << "target pos: " << target.pos.x << ' ' << target.pos.y << std::endl;
        std::cout << rand() << ' ' << rand() << ' ' << rand() << ' ' << RAND_MAX;
    }
};
