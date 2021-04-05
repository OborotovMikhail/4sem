#pragma once
#include "SFML\System.hpp"
#include "world.h"

// POSSIBLE PROBLEM WITH PLAYER CLASS
// UPD_VEL GIVES ACCESS TO VELOCITY

class Player
{
    sf::Vector2f pos; // Position
    sf::Vector2f v; // Velocity
    int rad = 20; // Radius
    float MaxSpeed; // Pixels per second, used as a reference to calculate velocity

    // friend void Server::upd_player_v(sf::Vector2f v, int clientId);

public:
    // Update player position
    void update(float dt);

    // Get player position
    sf::Vector2f get_pos();

    // Get player's x coordinate
    float get_x();

    // Get player's y coordinate
    float get_y();

    // Get player's x velocity
    float get_x_vel();

    // Get player's y velocity
    float get_y_vel();

    // Get player radius
    int get_rad();

    // Set new random player position
    void new_pos();

    // Update player velocity to new value
    void upd_vel(sf::Vector2f vel);

    // Increase player rad
    void increase_rad();
};