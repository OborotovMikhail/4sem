#pragma once

#include <mutex>
#include <unordered_map>
#include <iostream>
#include <random>

#include "player.h"

// Player id type
using PlayerId = int;
using TargetId = int;

// Game world structure
class World
{
    const int VIEWER_HEIGHT = 3840;
    const int VIEWER_LENGHT = 2160;

    std::unordered_map<PlayerId, Player> players; // Players map
    std::unordered_map<TargetId, Target> targets; // Targets map

    sf::Vector2i Size = { VIEWER_HEIGHT, VIEWER_LENGHT };; // World size

public:
    std::mutex mutex; // Mutex

    // Update world function
    // (Updates all players positions)
    void update(float dt);

    // Get players map
    std::unordered_map<PlayerId, Player>& get_players();

    // Get targets map
    std::unordered_map<TargetId, Target>& get_targets();
    
    // Get random position on map
    sf::Vector2f get_random_pos();

    // Get world size
    sf::Vector2i get_size();

    // Remove player from the world
    void remove_player(PlayerId clientId);

    // Print out all current players
    void show_players();
};
