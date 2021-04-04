#pragma once

#include "world.h"

void World::update(float dt)
{
    std::lock_guard<std::mutex> guard(m);

    // Updating players positions
    for (auto& it : players)
    {
        it.second.update(dt);
    }
}

int World::get_size_x()
{
    return this->Size.x;
}

int World::get_size_y()
{
    return this->Size.y;
}

std::mutex World::get_mutex()
{
    return ;
}
