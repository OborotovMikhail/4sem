#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <math.h>

/*
* Bouncing particles with basic physics
* with ability to spawn them using mouse
* 
* CONTROLS:
* Left click with mouse to spawn balls (drag while clicking to set direction)
* Right click to change ball type (two ball types available)
* 
* DONE
*/

// Playworld dimentions
#define WORLD_LENGHT 1280
#define WORLD_HEIGHT 720

// Gravity parameter
#define GRAVITY 500

// Energy loss coefficient (from 0 to 1)
#define ENERGY_LOSS 0.5

// 1st type ball settings
#define RADIUS_1 50.f 
#define MASS_1 250.f
#define NUMBER_1 1

// 2nd type ball settings
#define RADIUS_2 20.f
#define MASS_2 100.f
#define NUMBER_2 10

class Vector2 {
    // 2D vector class
public:
    float x, y;

    // Constructor
    Vector2() = default;
    Vector2(float m_x, float m_y) {
        x = m_x;
        y = m_y;
    }

    // Operations with the vector
    Vector2 operator+(const Vector2& other)const {
        return Vector2(x + other.x, y + other.y);
    }
    Vector2 operator-(const Vector2& other)const {
        return Vector2(x - other.x, y - other.y);
    }
    Vector2 operator*(const float a)const {
        return Vector2(x * a, y * a);
    }
    Vector2 operator/(const float a)const {
        return Vector2(x / a, y / a);
    }

    // Vector lenght
    float len() {
        return sqrt(x * x + y * y);
    }
};

float dot(Vector2 a, Vector2 b) {
    // vector dot vector operation
    return (a.x * b.x + a.y * b.y);
}

class Particle {
    // Particle class
public:
    std::size_t t;
    Vector2 pos;
    Vector2 vel;
    float r;
    float m;

    // Constructor
    Particle() = default;
    Particle(Vector2 pos_, Vector2 vel_, float radius, float mass, std::size_t type) {
        pos = pos_;
        vel = vel_;
        r = radius;
        m = mass;
        t = type;
    }
};

class World {
    // Play world class
public:
    // Vector of all particles
    std::vector<Particle> particles;

    // Add a new particle
    void add(Particle particle) {
        particles.push_back(particle);
    }

    // Update all particles
    void update(float dt) {
        // for each particle
        for (auto& elem : particles) {
            // Gravity effect
            elem.vel.y = elem.vel.y + GRAVITY * dt;
            elem.pos.y = elem.pos.y + GRAVITY * dt * dt / 2;

            if ((elem.pos.x <= 0) && (elem.vel.x < 0)) {
                // Bouncing off left X boud
                elem.vel.x = elem.vel.x * (-1);
                elem.pos.x = 0;
            }
            else if ((elem.pos.x >= (WORLD_LENGHT - 2 * elem.r)) && (elem.vel.x > 0)) {
                // Bouncing off right X boud
                elem.vel.x = elem.vel.x * (-1) * ENERGY_LOSS;
                elem.pos.x = WORLD_LENGHT - 2 * elem.r;
            }
            else if ((elem.pos.y >= (WORLD_HEIGHT - 2 * elem.r)) && (elem.vel.y > 0)) {
                // Bouncing off left Y boud
                elem.vel.y = elem.vel.y * (-1) * ENERGY_LOSS;
                elem.pos.y = WORLD_HEIGHT - 2 * elem.r;
            }

            // Checking for collisions
            for (auto& elem2 : particles) {
                Vector2 unit_vect(1.f, 1.f);
                Vector2 dist = (elem2.pos + unit_vect * elem2.r) - (elem.pos + unit_vect * elem.r);
                Vector2 relative_vel = elem2.vel - elem.vel;

                // Collision
                if ((dist.len() <= (elem.r + elem2.r)) && (dist.len() != 0) && (dot(dist, relative_vel) <= 0)) {
                    Vector2 normal_vect = dist / dist.len();
                    Vector2 impulse = normal_vect * dot(relative_vel * (2 / (1 / elem.m + 1 / elem2.m)), normal_vect);
                    float dl = elem.r + elem2.r - dist.len();

                    // Changing velocity
                    elem2.vel = elem2.vel - impulse / elem2.m;
                    elem.vel = elem.vel + impulse / elem.m;

                    // Changing coordinates
                    elem2.pos = elem2.pos + normal_vect * dl / 2;
                    elem.pos = elem.pos - normal_vect * dl / 2;
                }
            }

            // Updating position
            elem.pos = elem.pos + elem.vel * dt;
        }
    }

    // Number of particles
    std::size_t count() {
        return particles.size();
    }
};

sf::Color velocity_color(float vel, std::size_t type) {
    // Changing color depending on velocity
    sf::Uint16 col = vel / 2;
    sf::Color color_result;
    if (col > 255) {
        col = 255;
    }

    // Type 1 ball coloring
    if (type == 1) {
        color_result.r = col;
        color_result.g = 255 - col;
        color_result.b = 0;
    }
    else {
        // Type 2 ball coloring
        color_result.r = col;
        color_result.g = 0;
        color_result.b = 255 - col / 2;
    }

    return color_result;
}

void change_type(std::size_t& type) {
    // Change parrticle type
    if (type == 0) {
        type = 1;
    }
    else {
        type = 0;
    }
}

class Mouse {
    // Mouse class for tracking mouse movement and actions

    std::size_t mode_r = 0;
    std::size_t mode = 0;
    Particle first;
    Particle second;
    std::vector <Particle> ball;

public:
    // Constructor
    Mouse(float radius1, float mass1, float radius2, float mass2) {
        first.r = radius1;
        first.m = mass1;
        first.t = 0;
        second.r = radius2;
        second.m = mass2;
        second.t = 1;
    };

    sf::CircleShape phantom;
    sf::Vector2f particle_pos;
    sf::Vector2f cursor_pos;
    std::size_t ball_type = 1; // Default ball type is 1

    void tracking(World& W, sf::RenderWindow& window) {
        sf::Vector2f unit_vector(1.f, 1.f);
        ball.push_back(first);
        ball.push_back(second);

        // Right click changes ball type
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            if (mode_r == 0) {
                change_type(ball_type);
                mode_r = 1;
            }
        }
        else {
            mode_r = 0;
        }

        // Left click spawns a ball
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i m_particle_pos;
            sf::Vector2i m_cursor_pos;
            if (mode == 0) {
                m_particle_pos = sf::Mouse::getPosition(window);
                particle_pos = window.mapPixelToCoords(m_particle_pos);
                mode = 1;
            }
            else {
                m_cursor_pos = sf::Mouse::getPosition(window);
                cursor_pos = window.mapPixelToCoords(m_cursor_pos);
                sf::Vertex line[] =
                {
                    sf::Vertex(particle_pos),
                    sf::Vertex(cursor_pos)
                };
                window.draw(line, 2, sf::Lines);
            }
            phantom.setRadius(ball[ball_type].r);
            phantom.setPosition(particle_pos - unit_vector * ball[ball_type].r);
            ball[ball_type].vel.x = 3.f * (cursor_pos.x - particle_pos.x);
            ball[ball_type].vel.y = 3.f * (cursor_pos.y - particle_pos.y);
            sf::Color color = velocity_color(ball[ball_type].vel.len(), ball_type);
            phantom.setFillColor(color);
            window.draw(phantom);
        }
        else {
            if (mode == 1) {
                ball[ball_type].pos.x = particle_pos.x - ball[ball_type].r;
                ball[ball_type].pos.y = particle_pos.y - ball[ball_type].r;

                // Spawning 1st type balls
                if (ball_type == 0) {
                    for (size_t i = 0; i < NUMBER_2; i++) {
                        W.add(ball[ball_type]);
                    }
                }

                // Spawning 2st type balls
                if (ball_type == 1) {
                    for (size_t i = 0; i < NUMBER_1; i++) {
                        W.add(ball[ball_type]);
                    }
                }
                
                mode = 0;
            }
        }
    }
};

void DrawWorld(World& W, sf::RenderWindow& window) {
    // Draw playworld inside window function

    // Vector of balls
    std::vector<sf::CircleShape> balls(W.count());

    // Vector of coordinates
    std::vector<sf::Vector2f> new_pos(W.count());

    for (std::size_t i = 0; i < W.count(); i++) {
        // Setting ball coordinates
        new_pos[i].x = W.particles[i].pos.x;
        new_pos[i].y = W.particles[i].pos.y;
        balls[i].setPosition(new_pos[i]);
        balls[i].setRadius(W.particles[i].r);

        // Setting ball colors
        sf::Color color = velocity_color(W.particles[i].vel.len(), W.particles[i].t);
        balls[i].setFillColor(color);

        // Drawing world
        window.draw(balls[i]);
    }
}

int main() {
    World W;
    Mouse mouse(RADIUS_2, MASS_2, RADIUS_1, MASS_1);

    // Creating window
    sf::RenderWindow window(sf::VideoMode(WORLD_LENGHT, WORLD_HEIGHT), "Bounce Particles");
    window.setFramerateLimit(60);

    // Loading and setting background texture
    sf::Texture texture;
    if (!texture.loadFromFile("hackerman.jpg")) {
        std::cout << "Can not load background texture" << std::endl;
    }
    sf::Sprite sprite;
    sprite.setTexture(texture);

    // Setting time parameters
    sf::Clock clock;
    sf::Time prev_time;

    while (window.isOpen()) {
        // Calculating dt
        sf::Time time = clock.getElapsedTime();
        float dt = time.asSeconds() - prev_time.asSeconds();
        prev_time = time;

        // Window closing button
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(); // Clearing window
        window.draw(sprite); // Drawing background
        mouse.tracking(W, window); // Checking mouse actions
        W.update(dt); // Updating world
        DrawWorld(W, window); // Drawing world to window
        window.display(); // Displaying window
    }
}
