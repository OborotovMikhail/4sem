#pragma once
#include <SFML\System.hpp>

class Target
{
	const float RADIUS = 5.0f; // Target radius

	sf::Vector2f pos; // Target position

public:
	Target();

	// Get target position
	sf::Vector2f get_pos();

	// Set new target position
	void set_pos(sf::Vector2f new_pos);

	// Get target radius
	float get_rad();
};