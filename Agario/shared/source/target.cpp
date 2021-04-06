#pragma once

#include <mutex>
#include <random>
#include "target.h"

// Get target position func
sf::Vector2f Target::get_pos()
{
	return this->pos;
}

void Target::set_pos(sf::Vector2f new_pos)
{
    this->pos = new_pos;
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
