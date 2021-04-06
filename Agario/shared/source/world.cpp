#pragma once

#include "world.h"

const sf::Vector2i Size = { 800, 800 };

void World::update(float dt)
{
    std::lock_guard<std::mutex> guard(mutex);

    // Updating players positions
    for (auto& it : players)
    {
        it.second.update(dt);
    }
}

std::unordered_map<PlayerId, Player> World::get_players()
{
    return this->players;
}

Target World::get_target()
{
    return this->target;
}
