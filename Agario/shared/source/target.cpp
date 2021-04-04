#pragma once
#include <mutex>
#include <random>
#include "target.h"

void Target::set_new_pos()
{
    unsigned int seed = std::chrono::steady_clock::now().time_since_epoch().count(); // Random seed
    std::default_random_engine generator(seed); // Generator
    std::uniform_real_distribution<float> distribution_x(float(World::Size.x) * 0.05, float(World::Size.x) * 0.95); // Creating x distribution
    std::uniform_real_distribution<float> distribution_y(float(World::Size.y) * 0.05, float(World::Size.y) * 0.95); // Creating y distribution

    this->pos.x = distribution_x(generator); // Generating new x target pos
    this->pos.y = distribution_y(generator); // Generating new y target pos
}

sf::Vector2f Target::get_pos()
{
	return sf::Vector2f();
}

float Target::get_x()
{
	return 0.0f;
}

float Target::get_y()
{
	return 0.0f;
}
