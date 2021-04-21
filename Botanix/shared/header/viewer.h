#pragma once

#include <SFML\Graphics.hpp>

#include "world.h"

class Viewer : public sf::RenderWindow
{
    const int VIEWER_WIDTH = 800;
    const int VIEWER_HEIGHT = 800;

    sf::Texture player_texture;
    sf::Texture target_texture;

public:
    // Viewer constructor
    Viewer(const std::string& name);

    // Handling viewer events function
    void handleEvents();

    // Draw (display) world function
    void draw(World& world);
};
