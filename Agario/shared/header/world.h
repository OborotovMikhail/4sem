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
    // World dimentions
    // Some standart values: 1920 1080, 3840 2160, 5760 3240, 7680 4320
    static const int WORLD_WIDTH = 5760; // World x span
    static const int WORLD_HEIGHT = 3240; // World y span

    std::unordered_map<PlayerId, Player> players; // Players map
    std::unordered_map<TargetId, Target> targets; // Targets map

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
    static sf::Vector2i get_size();

    // Remove player from the world
    void remove_player(PlayerId clientId);

    // Print out all current players
    void show_players();
};
