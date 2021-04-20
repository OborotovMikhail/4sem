#pragma once

#include "controls.h"

sf::Vector2f controls_arrows()
{
	sf::Vector2f v;

    // zero movement
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        && (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) == sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
    {
        v.x = 0.0f;
        v.y = 0.0f;
    }
    // left movement
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
        && (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) == sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
    {
        v.x = - 1.0f;
        v.y = 0.0f;
    }
    // right movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
        && (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) == sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
    {
        v.x = 1.0f;
        v.y = 0.0f;
    }
    // down movement
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        v.x = 0.0f;
        v.y = 1.0f;
    }
    // up movement
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        v.x = 0;
        v.y = - 1.0f;
    }
    // left-down movement
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
        && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        v.x = - 1.0f;
        v.y = 1.0f;
    }
    // left-up movement
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
        && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        v.x = - 1.0f;
        v.y = - 1.0f;
    }
    // right-down movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
        && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        v.x = 1.0f;
        v.y = 1.0f;
    }
    // right-up movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
        && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        v.x = 1.0f;
        v.y = - 1.0f;
    }
    
    return v;
}

sf::Vector2f controls_wasd()
{
    sf::Vector2f v;

    // zero movement
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) == sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        && (sf::Keyboard::isKeyPressed(sf::Keyboard::W) == sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
    {
        v.x = 0.0f;
        v.y = 0.0f;
    }
    // left movement
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::A)
        && (sf::Keyboard::isKeyPressed(sf::Keyboard::W) == sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
    {
        v.x = - 1.0f;
        v.y = 0.0f;
    }
    // right movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !sf::Keyboard::isKeyPressed(sf::Keyboard::A)
        && (sf::Keyboard::isKeyPressed(sf::Keyboard::W) == sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
    {
        v.x = 1.0f;
        v.y = 0.0f;
    }
    // down movement
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) == sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        && !sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        v.x = 0.0f;
        v.y = 1.0f;
    }
    // up movement
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) == sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        && sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        v.x = 0.0f;
        v.y = - 1.0f;
    }
    // left-down movement
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::A)
        && !sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        v.x = - 1.0f;
        v.y = 1.0f;
    }
    // left-up movement
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::A)
        && sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        v.x = - 1.0f;
        v.y = - 1.0f;
    }
    // right-down movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !sf::Keyboard::isKeyPressed(sf::Keyboard::A)
        && !sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        v.x = 1.0f;
        v.y = 1.0f;
    }
    // right-up movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !sf::Keyboard::isKeyPressed(sf::Keyboard::A)
        && sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        v.x = 1.0f;
        v.y = - 1.0f;
    }

    return v;
}
