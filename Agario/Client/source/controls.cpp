#include "controls.h"

sf::Vector2f speed_controls_arrows(float maxspeed)
{
	sf::Vector2f v;

    // zero movement
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        && (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) == sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
    {
        v.x = 0;
        v.y = 0;
    }
    // left movement
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
        && (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) == sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
    {
        v.x = -maxspeed;
        v.y = 0;
    }
    // right movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
        && (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) == sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
    {
        v.x = maxspeed;
        v.y = 0;
    }
    // down movement
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        v.x = 0;
        v.y = maxspeed;
    }
    // up movement
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        v.x = 0;
        v.y = -maxspeed;
    }
    // left-down movement
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
        && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        v.x = -maxspeed;
        v.y = maxspeed;
    }
    // left-up movement
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
        && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        v.x = -maxspeed;
        v.y = -maxspeed;
    }
    // right-down movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
        && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        v.x = maxspeed;
        v.y = maxspeed;
    }
    // right-up movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
        && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        v.x = maxspeed;
        v.y = -maxspeed;
    }

    return v;
}
