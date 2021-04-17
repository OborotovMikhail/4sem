#pragma once

#include <SFML\Graphics.hpp>

#include "world.h"

class Viewer : public sf::RenderWindow
{
    sf::Texture mapTexture;
    sf::Sprite background;

public:
    // Viewer constructor
    Viewer(const std::string& name);

    // Handling viewer events function
    void handleEvents();

    // Draw (display) world function
    void draw(World& world, int my_client_id);
};
