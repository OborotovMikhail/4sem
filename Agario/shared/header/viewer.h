#pragma once

#include <SFML\Graphics.hpp>

#include "world.h"

class Viewer : public sf::RenderWindow
{
    const int VIEWER_WIDTH = 1920;
    const int VIEWER_HEIGHT = 1080;

public:
    // Viewer constructor
    Viewer(const std::string& name);

    // Handling viewer events function
    void handleEvents();

    // Draw (display) world function
    // Second argument is client id, or -1 for server viewer
    void draw(World& world, int my_client_id);
};
