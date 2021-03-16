#include <cmath>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

/*
* Draws field lines of the field created by some charges
* 
* DONE
*/

#define WINDOW_LENGHT 1920
#define WINDOW_HEIGHT 1080

#define LINE_LENGHT 5
#define LINE_HEIGHT 1

#define X_STEP 5
#define Y_STEP 5

// Charge class
class Charge {
public:
	float x_, y_, value_;
	sf::CircleShape circle_;

	// Charge constructor
	Charge(float x, float y, float value, int size) {
		// Setting values
		x_ = x;
		y_ = y;
		value_ = value;

		// Setting circle settings
		circle_ = sf::CircleShape(size);
		circle_.setPosition(x_, y_);

		// Setting color
		if (value > 0) {
			// Red
			circle_.setFillColor({ 255, 0, 0 });
		}
		else {
			// Blue
			circle_.setFillColor({ 0,51,255 });
		}
	};

	// Charge draw function
	void Draw(sf::RenderWindow& window) {
		window.draw(circle_);
	}
};

// Field line (single) class
class Line {
public:
	float x_, y_, angle_;
	float lenght_ = LINE_LENGHT, height = LINE_HEIGHT;
	sf::RectangleShape rect_;

	// Line constructor
	Line(float angle, float x, float y) {
		// Setting values
		x_ = x;
		y_ = y;
		angle_ = angle;

		// Setting line settings
		rect_ = sf::RectangleShape(sf::Vector2f(lenght_, height));
		rect_.setFillColor({ 255,255,255 });
		rect_.setOrigin({ lenght_ / 2, height / 2 });
		rect_.setPosition(x_, y_);
		rect_.setRotation(angle_ * 180 / 3.1415);
	};

	// Line draw function
	void Draw(sf::RenderWindow& window) {
		window.draw(rect_);
	}
};

// Field calculation function
std::vector<Line> CalculateField(const std::vector<Charge>& charges) {
	// Field lines
	std::vector<Line> lines;

	// Running through x coordinate
	for (int i = 0; i < WINDOW_LENGHT; i += X_STEP) {
		// Running through y coordinate
		for (int j = 0; j < WINDOW_HEIGHT; j += Y_STEP) {
			// Calculating effect from all charges in current point
			sf::Vector2f Eij(0, 0);
			float x, y, mod;
			for (const auto& single_charge : charges) {
				// Distance
				x = i - single_charge.x_;
				y = j - single_charge.y_;

				// Calculating effect
				mod = single_charge.value_ / (std::pow((x * x + y * y), 1.5));
				sf::Vector2f Eij_elem(x * mod, y * mod);
				
				// Adding to summary
				Eij += Eij_elem;
			}

			// Saving field line from current coordinates
			Line local_line(std::atan2(Eij.y, Eij.x), i, j);
			lines.push_back(local_line);
		}
	}

	return lines;
}

int main()
{
	// Creating charges
	Charge charge_positive_1(WINDOW_LENGHT * 0.3, WINDOW_HEIGHT * 0.2, 1, 6);
	Charge charge_positive_2(WINDOW_LENGHT * 0.4, WINDOW_HEIGHT * 0.4, 1, 6);
	Charge charge_negative_1(WINDOW_LENGHT * 0.5, WINDOW_HEIGHT * 0.9, -1, 6);
	Charge charge_negative_2(WINDOW_LENGHT * 0.1, WINDOW_HEIGHT * 0.6, -1, 6);
	Charge charge_negative_3(WINDOW_LENGHT * 0.8, WINDOW_HEIGHT * 0.3, -1, 6);

	// Adding charges to a vector
	std::vector<Charge> charges;
	charges.push_back(charge_positive_1);
	charges.push_back(charge_positive_2);
	charges.push_back(charge_negative_1);
	charges.push_back(charge_negative_2);
	charges.push_back(charge_negative_3);

	// Calculating field
	auto lines = CalculateField(charges);

	// Creating window
	sf::RenderWindow window(sf::VideoMode(WINDOW_LENGHT, WINDOW_HEIGHT), "Field Lines");

	while (window.isOpen()) {
		// Window close button
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Drawing charges
		for (auto& local_charge : charges) {
			local_charge.Draw(window);
		}
		// Drawing field lines
		for (auto& local_line : lines) {
			local_line.Draw(window);
		}
		
		// Displaying
		window.display();
	}

	return 0;
}