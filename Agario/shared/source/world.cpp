#pragma once

#include "world.h"

// sf::Vector2i Size = { 800, 800 };

void World::update(float dt)
{
    std::lock_guard<std::mutex> guard(mutex);

    // Updating players positions
    for (auto& it : players)
    {
        it.second.update(dt);
    }
}

std::unordered_map<PlayerId, Player>& World::get_players()
{
    return this->players;
}

Target& World::get_target()
{
    return this->target;
}

sf::Vector2f World::get_random_pos()
{
    sf::Vector2f pos;
    int is_inside = 1;

    while (is_inside)
    {
        is_inside = 0;

        unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count(); // Random seed
        std::default_random_engine generator(seed); // Generator
        std::uniform_real_distribution<float> distribution_x(float(this->get_size().x) * 0.05, float(this->get_size().x) * 0.95); // Creating x distribution
        std::uniform_real_distribution<float> distribution_y(float(this->get_size().y) * 0.05, float(this->get_size().y) * 0.95); // Creating y distribution
        pos = { distribution_x(generator), distribution_y(generator) }; // Generating position

        for (auto& it : this->players)
        {
            if (sqrt(pow((it.second.get_x() - pos.x), 2) + pow((it.second.get_y()
                - pos.y), 2)) < it.second.get_rad())
            {
                is_inside = is_inside + 1;
            }
        }
    }

    return pos;
}

sf::Vector2i World::get_size()
{
    return this->Size;
}
