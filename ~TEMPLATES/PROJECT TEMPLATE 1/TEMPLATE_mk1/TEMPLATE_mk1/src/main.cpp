#include <iostream>
#include <SFML\Graphics.hpp>

int main()
{
    std::cout << "Hello, World!" << std::endl;

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    while (1)
    {
        window.clear(sf::Color::Red);
        window.display();
    }

    return 0;
}