#pragma once

#include <mutex>
#include <random>
#include "target.h"

Target::Target()
{
	this->pos.x = 0;
	this->pos.y = 0;
}

// Get target position func
sf::Vector2f Target::get_pos()
{
	return this->pos;
}

void Target::set_pos(sf::Vector2f new_pos)
{
	sf::Vector2f x = new_pos;
    pos = x;
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
