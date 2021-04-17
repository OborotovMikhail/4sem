#pragma once

#include <mutex>
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

int Target::get_rad()
{
	return this->radius;
}
