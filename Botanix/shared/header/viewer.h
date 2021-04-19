#pragma once

#include <SFML\Graphics.hpp>

#include "world.h"

class Viewer : public sf::RenderWindow
{
    const int VIEWER_WIDTH = 800;
    const int VIEWER_HEIGHT = 800;

public:
    // Viewer constructor
    Viewer(const std::string& name);

    // Handling viewer events function
    void handleEvents();

    // Draw (display) world function
    void draw(World& world);
};
