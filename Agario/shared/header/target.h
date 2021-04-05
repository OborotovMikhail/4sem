#pragma once
#include <SFML\System.hpp>
#include "world.h"

class Target
{
	sf::Vector2f pos; // Target position

public:
	// Set new target pos
	void new_pos();

	// Get target position
	sf::Vector2f get_pos();

	// Get target's x coordinate
	float get_x();

	// Get target's y coordinate
	float get_y(); 
};