#pragma once
#include <SFML\System.hpp>

class Target
{
	sf::Vector2f pos; // Target position

public:
	void set_new_pos(); // Set new random target position func
	sf::Vector2f get_pos(); // Get target position
	float get_x(); // Get target's x coordinate
	float get_y(); // Get target's y coordinate
};