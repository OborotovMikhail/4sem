#pragma once

#include "world.h"

sf::Vector2i Size = { 800, 800 };

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

sf::Vector2f World::get_random_pos()
{
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count(); // Random seed
    std::default_random_engine generator(seed); // Generator
    std::uniform_real_distribution<float> distribution_x(float(World::Size.x) * 0.05, float(World::Size.x) * 0.95); // Creating x distribution
    std::uniform_real_distribution<float> distribution_y(float(World::Size.y) * 0.05, float(World::Size.y) * 0.95); // Creating y distribution
    sf::Vector2f pos(distribution_x(generator), distribution_y(generator)); // Generating position
    return pos;
}

sf::Vector2i World::get_size()
{
    return this->Size;
}
