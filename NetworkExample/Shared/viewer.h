#pragma once

#include <SFML\Graphics.hpp>

#include "world.h"

class Viewer : public sf::RenderWindow
{
public:
  Viewer(const std::string& name) : sf::RenderWindow(sf::VideoMode(World::Size.x, World::Size.y), name)
  {
    setFramerateLimit(60);
  }

  void handleEvents()
  {
    sf::Event event;
    while (pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        close();
    }
  }

  void draw(const World& world)
  {
    static const sf::Color colors[] = { sf::Color::Red, sf::Color::Green, sf::Color::Blue };

    clear(sf::Color::Black);

    for (const auto& it : world.players)
    {
      sf::CircleShape s(25);

      s.setFillColor(colors[it.first]);
      s.setPosition(it.second.pos);
      sf::RenderWindow::draw(s);
    }

    display();
  }
};
