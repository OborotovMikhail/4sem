#include <SFML/Network.hpp>
#include <iostream>
#include <atomic>

#include "world.h"
#include "network.h"
#include "viewer.h"

// TODO

class Client
{
public:
    Client(const std::string& ip, int port, World& world) :
        ip(ip),
        port(port),
        world(world)
    {
        running = true;
        syncThread = std::thread(&Client::start, this);
        syncThread.detach();
    }

    ~Client()
    {
        running = false;
        syncThread.join();
    }

    void notify()
    {
        sf::Packet packet;
        const auto& v = world.players[id()].v;

        packet << Message::Movement << clientId << v.x << v.y;
        if (socket.send(packet) != sf::Socket::Done)
        {
            std::cout << "Can't send movement\n";
        }
    }

    int id() const
    {
        return clientId;
    }

    bool isRunning() const
    {
        return running;
    }

private:
    const std::string ip;
    const int port;
    float serverTime = 0.f;

    std::atomic<bool> running = false;
    World& world;

    int clientId = -1;

    sf::Clock clock;

    sf::TcpSocket socket;

    std::thread syncThread;

    void start()
    {
        auto status = socket.connect(ip, port);
        if (status != sf::Socket::Status::Done)
        {
            std::cout << "Can't connect to server: " << status << "\n";
            return;
        }

        while (running)
        {
            sf::Packet packet;
            if (socket.receive(packet) == sf::Socket::Done)
            {
                int type;
                packet >> type;
                if (type == Message::ClientCreated)
                {
                    sf::Vector2f pos;
                    packet >> clientId >> pos.x >> pos.y >> serverTime;
                    world.players[clientId] = { pos, sf::Vector2f() };
                    std::cout << "Client created: " << clientId << "\n";
                    clock.restart();
                }
                else
                    if (type == Message::UpdateWorld)
                    {
                        {
                            int n;
                            packet >> n;
                            std::lock_guard<std::mutex> m(world.m);

                            for (int i = 0; i < n; ++i)
                            {
                                int index;
                                sf::Vector2f pos, v;

                                packet >> index >> pos.x >> pos.y >> v.x >> v.y;

                                world.players[index].pos = pos;

                                if (index != id())
                                    world.players[index].v = v;
                            }
                        }
                        float ts;
                        packet >> ts;

                        world.update(clock.getElapsedTime().asSeconds() - (ts - serverTime));
                    }
            }
        }
    }
};


int main()
{
    World world;
    Viewer viewer("My client");

    Client client("127.0.0.1", 1234, world);
    // ip from example - 192.168.43.119

    sf::Clock gameClock;
    sf::Vector2f prevVelocity;

    while (viewer.isOpen() && client.isRunning())
    {
        viewer.handleEvents();

        sf::Vector2f v;
        if (client.id() == 0)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                v = { -Player::MaxSpeed, 0.f };
            }
            else
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    v = { Player::MaxSpeed, 0.f };
                }
                else
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                    {
                        v = { 0.f, -Player::MaxSpeed };
                    }
                    else
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                        {
                            v = { 0.f, Player::MaxSpeed };
                        }
        }
        else
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                v = { -Player::MaxSpeed, 0.f };
            }
            else
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                {
                    v = { Player::MaxSpeed, 0.f };
                }
                else
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                    {
                        v = { 0.f, -Player::MaxSpeed };
                    }
                    else
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                        {
                            v = { 0.f, Player::MaxSpeed };
                        }
        }

        world.players[client.id()].v = v;

        const auto dt = gameClock.restart();
        world.update(dt.asSeconds());

        // On any change (speed direction or magnitude) notify server
        if (v != prevVelocity)
        {
            client.notify();
            prevVelocity = v;
        }

        viewer.draw(world);
    }

    return 0;
}
