#pragma once

#include <mutex>
#include <unordered_map>
#include <iostream>
#include <random>

#include "player.h"
#include "target.h"

// Player id type
using PlayerId = int;

// Game world structure
class World
{
    static const int WORLD_WIDTH = 800; // World x span
    static const int WORLD_HEIGHT = 800; // World y span

    std::unordered_map<PlayerId, Player> players; // Players map
    Target target; // Target class object

public:
    std::mutex mutex; // Mutex

    // Update world function
    // (Updates all players positions)
    void update(float dt);

    // Get players map
    std::unordered_map<PlayerId, Player>& get_players();

    // Get target function
    Target& get_target();
    
    // Get random position on map
    sf::Vector2f get_random_pos();

    // Get world size
    static sf::Vector2i get_size();

    void remove_player(PlayerId clientId);

    void show_players();

    void check_controls();
};
