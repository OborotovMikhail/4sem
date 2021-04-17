#pragma once
#include <SFML\System.hpp>

class Target
{
	sf::Vector2f pos; // Target position
	const int radius = 10;

public:
	Target();

	// Get target position
	sf::Vector2f get_pos();

	// Set new target position
	void set_pos(sf::Vector2f new_pos);

	// Get target radius function
	int get_rad();
};