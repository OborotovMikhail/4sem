#pragma once

#include <mutex>
#include <unordered_map>
#include <iostream>
#include <random>

#include "player.h"
#include "target.h"

// Player id type
using PlayerId = int;
using TargetId = int;

// Game world structure
class World
{
    std::unordered_map<PlayerId, Player> players; // Players map
    std::unordered_map<TargetId, Target> targets; // Targets map

    Target target; // Target class object
    sf::Vector2i Size = { 800, 800 };; // World size

public:
    std::mutex mutex; // Mutex

    // Update world function
    // (Updates all players positions)
    void update(float dt);

    // Get players map
    std::unordered_map<PlayerId, Player>& get_players();

    // Get targets map
    std::unordered_map<TargetId, Target>& get_targets();

    // Get target function
    Target& get_target();
    
    // Get random position on map
    sf::Vector2f get_random_pos();

    // Get world size
    sf::Vector2i get_size();

    // Remove player from the world
    void remove_player(PlayerId clientId);

    // Print out all current players
    void show_players();
};
