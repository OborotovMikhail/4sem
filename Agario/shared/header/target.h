#pragma once
#include <SFML\System.hpp>

class Target
{
	sf::Vector2f pos; // Target position

public:
	// Get target position
	sf::Vector2f get_pos();

	// Set new target position
	void set_pos(sf::Vector2f new_pos);

	// Get target's x coordinate
	float get_x();

	// Get target's y coordinate
	float get_y(); 
};