#pragma once
#include <mutex>
#include <random>
#include "target.h"

// Set new target pos
void Target::new_pos()
{
    unsigned int seed = std::chrono::steady_clock::now().time_since_epoch().count(); // Random seed
    std::default_random_engine generator(seed); // Generator
    std::uniform_real_distribution<float> distribution_x(float(World::Size.x) * 0.05, float(World::Size.x) * 0.95); // Creating x distribution
    std::uniform_real_distribution<float> distribution_y(float(World::Size.y) * 0.05, float(World::Size.y) * 0.95); // Creating y distribution

    this->pos.x = distribution_x(generator); // Generating new x target pos
    this->pos.y = distribution_y(generator); // Generating new y target pos
}

// Get target position func
sf::Vector2f Target::get_pos()
{
	return this->pos;
}

// Get x target coordinate func
float Target::get_x()
{
	return this->pos.x;
}

// Get y target coordinate func
float Target::get_y()
{
	return this->pos.y;
}
