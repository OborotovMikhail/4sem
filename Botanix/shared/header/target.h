#pragma once
#include <SFML\System.hpp>

class Target
{
	const float RADIUS = 10.0f; // Target radius

	sf::Vector2f pos = { 0.0f, 0.0f }; // Target position

public:
	// Get target position
	sf::Vector2f get_pos();

	// Set new target position
	void set_pos(sf::Vector2f new_pos);

	// Get target radius
	float get_rad();
};