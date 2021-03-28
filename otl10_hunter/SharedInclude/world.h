#pragma once

#include <mutex>
#include <unordered_map>

#include "player.h"

// Player id type
using PlayerId = int;

struct World
{
    // Players map
    std::unordered_map<PlayerId, Player> players;

    // 
    void update(float dt)
    {
        std::lock_guard<std::mutex> guard(m);
        for (auto& it : players)
            it.second.update(dt);
    }
    std::mutex m;

    static const sf::Vector2i Size;
};
