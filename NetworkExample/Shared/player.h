#pragma once
#include <SFML\System.hpp>

struct Player
{
  sf::Vector2f pos;
  sf::Vector2f v;
  void update(float dt)
  {
    pos += v * dt;
  }
  static const float MaxSpeed; // pixels per second
};
