#pragma once

#include "client.h"

void Client::recieve()
{
    // Connecting socket to server
    auto status = socket.connect(ip, port);
    if (status != sf::Socket::Status::Done)
    {
        std::cout << "Can't connect to server: " << status << "\n";
        return;
    }

    // Main cycle
    while (running)
    {
        // Recieving packets
        sf::Packet packet;
        if (socket.receive(packet) == sf::Socket::Done)
        {
            // Packet type
            int type;
            packet >> type;

            // Client created packet processing
            if (type == Message::ClientCreated)
            {
                sf::Vector2f pos;
                packet >> clientId >> pos.x >> pos.y >> serverTime;

                world.get_players()[clientId].set_pos(pos);
                std::cout << "Client created, id: " << clientId << std::endl;

                // Recieving all world data
                int n;
                packet >> n; // Getting number of players from packet
                std::lock_guard<std::mutex> m(world.mutex);
                for (int i = 0; i < n; i++)
                {
                    int index, score, hero_id;
                    bool hero_selected;
                    sf::Vector2f pos, v; // Positon and velocity from server

                    packet >> index >> pos.x >> pos.y >> v.x >> v.y >> score >> hero_id >> hero_selected;

                    world.get_players()[index].set_pos(pos); // Updating position for players
                    world.get_players()[index].set_vel(v); // Updating velocity for players
                    world.get_players()[index].set_score(score); // Set player score
                    world.get_players()[index].set_selected_hero(hero_id); // Setting selected hero
                    world.get_players()[index].setHeroSelectionConfirm(hero_selected); // Setting hero confirmation
                }

                // Updating target position
                sf::Vector2f target_pos;
                packet >> target_pos.x >> target_pos.y;
                world.get_target().set_pos(target_pos);

                // Printing currently online players
                std::cout << "Online players by their id\'s: ";
                for (auto& it : world.get_players())
                {
                    std::cout << it.first << " ";
                }
                std::cout << std::endl;

                clock.restart();
            }

            // Update world packet processing
            if (type == Message::UpdateWorld)
            {
                {
                    int n; // Number of players
                    packet >> n; // Getting number of players from packet
                    std::lock_guard<std::mutex> m(world.mutex);

                    for (int i = 0; i < n; ++i)
                    {
                        int index, score; // Player id
                        sf::Vector2f pos, v; // Positon and velocity from server

                        packet >> index >> pos.x >> pos.y >> v.x >> v.y >> score;

                        world.get_players()[index].set_pos(pos); // Updating position for players
                        world.get_players()[index].set_vel(v); // Updating velocity for players
                        world.get_players()[index].set_score(score); // Set player score
                    }
                }

                // Updating target position
                sf::Vector2f target_pos;
                packet >> target_pos.x >> target_pos.y;
                world.get_target().set_pos(target_pos);

                // Getting elapsed server time from packet
                float ts;
                packet >> ts;

                // Updating world
                world.update(clock.getElapsedTime().asSeconds() - (ts - serverTime));
            }

            // Remove player packet processing
            if (type == Message::RemovePlayer)
            {
                int remove_id;
                packet >> remove_id; // Getting player id
                this->world.remove_player(remove_id); // Removing this player

                std::cout << "Player " << remove_id << " disconnected\n";

                this->world.show_players(); // Printing currently online players
            }

            // Start game packet processing
            if (type == Message::SceneGameplay)
            {
                world.SetScene(Scene::Gameplay);

                std::cout << "Started the game" << std::endl;
            }

            // Gameover scene packet processing
            if (type == Message::SceneGameover)
            {
                int id;
                packet >> id;

                world.SetScene(Scene::Gameover);
                world.get_players()[id].won_the_game();

                world.setWinnerId(id); // Setting winner player id

                std::cout << "Player " << id << " won the game\n";
            }

            // Gameover scene packet processing
            if (type == Message::PlayerHeroSelected)
            {
                int id, selected_hero;
                packet >> id >> selected_hero;

                world.get_players()[id].set_selected_hero(selected_hero);
                world.get_players()[id].setHeroSelectionConfirm(true);
            }

            // Server is full packet processing
            if (type == Message::ErrorServerFull)
            {
                world.SetScene(Scene::ErrorServerFull);
                std::cout << "Could not connect to server, server is full" << std::endl;
            }

            // Server is full packet processing
            if (type == Message::ErrorOngoingGame)
            {
                world.SetScene(Scene::ErrorOngoingGame);
                std::cout << "Could not connect to server, there is an ongoing game" << std::endl;
            }
        }
    }
}

Client::Client(const std::string& ip, int port, World& world) :
    ip(ip),
    port(port),
    world(world)
{
    running = true; // Client now running
}

Client::~Client()
{
    this->disconnect();
    running = false; // No longer running 
    syncThread.join(); // Joining thread
}

void Client::notify_server()
{
    // Creating a packet
    sf::Packet packet;

    // This client's player controls vector
    const auto& controls = world.get_players()[id()].get_controls();

    // Creating a packet for sending controls
    packet << Message::Movement << clientId << controls.x << controls.y;
    
    // Sending packet
    if (socket.send(packet) != sf::Socket::Done)
    {
        std::cout << "Can't send movement to server\n";
    }
}

int Client::id() const
{
    return clientId;
}

bool Client::isRunning() const
{
    return running;
}

void Client::connect()
{
    syncThread = std::thread(&Client::recieve, this); // Creating thread
    syncThread.detach(); // Detaching thread
}

void Client::disconnect()
{
    // Creating a packet
    sf::Packet packet;

    // Creating a packet for sending velocity
    packet << Message::ClientDisconnect << this->id();

    // Sending packet
    if (socket.send(packet) != sf::Socket::Done)
    {
        std::cout << "Can't send disconnect packet to server\n";
    }
}

void Client::events_connect(Viewer& viewer)
{
    // Changing buttons in connect scene
    // Going down
    if ((viewer.get_connect_selected_button() < (viewer.get_connect_menu_size() - 1)) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        viewer.set_connect_selected_button(viewer.get_connect_selected_button() + 1);
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {}
    }
    // Going up
    if ((viewer.get_connect_selected_button() > 0) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        viewer.set_connect_selected_button(viewer.get_connect_selected_button() - 1);
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {}
    }

    // Pressing connect button
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && (viewer.get_lobby_selected_button() == 0))
    {
        this->connect();
        world.SetScene(Scene::Lobby);
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {}
    }

    // Inputing to textboxes
    if (viewer.get_connect_selected_button() < viewer.get_number_of_textboxes())
    {
        sf::Event my_event;

        while (viewer.pollEvent(my_event))
        {
            switch (my_event.type)
            {
            case sf::Event::Closed:
                viewer.close();
            case sf::Event::TextEntered:
                viewer.getTextbox()[viewer.get_connect_selected_button()].typedOn(my_event);
            }
        }
    }
}

void Client::events_lobby(Viewer& viewer)
{
    // Changing buttons in lobby
    // Going down
    if ((viewer.get_lobby_selected_button() < (viewer.get_lobby_buttons().size() - 1)) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        viewer.set_lobby_selected_button(viewer.get_lobby_selected_button() + 1);
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {}
    }
    // Going up
    if ((viewer.get_lobby_selected_button() > 0) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        viewer.set_lobby_selected_button(viewer.get_lobby_selected_button() - 1);
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {}
    }

    // Pressing ready button
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && (viewer.get_lobby_selected_button() == 0))
    {
        if (!world.get_players()[clientId].IfReady())
        {
            debug("Ready");
            world.get_players()[this->clientId].player_ready();

            // Creating a packet
            sf::Packet packet;

            // Creating a packet for sending velocity
            packet << Message::ClientReady << this->id();

            // Sending packet
            if (socket.send(packet) != sf::Socket::Done)
            {
                std::cout << "Can't send ready packet to server\n";
            }
        }
    }

    // Pressing hero selection button
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && (viewer.get_lobby_selected_button() == 1))
    {
        world.SetScene(Scene::HeroSelection);
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {}
    }

    // Pressing disconnect button
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && (viewer.get_lobby_selected_button() == 2))
    {
        this->disconnect();
        this->running = false;
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {}
    }
}

void Client::events_hero_selection(Viewer& viewer)
{
    // ESC to get back to lobby
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        world.SetScene(Scene::Lobby);
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {}
    }

    // Going left in hero selection
    if ((world.get_players()[this->clientId].get_selected_hero() > 0) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        if (!world.get_players()[this->clientId].isHeroSelected())
        {
            int current_hero = world.get_players()[this->clientId].get_selected_hero();
            current_hero -= 1;
            world.get_players()[this->clientId].set_selected_hero(current_hero);
            while (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {}
        }
    }
        
    // Going right in hero selection
    if ((world.get_players()[this->clientId].get_selected_hero() < (viewer.get_number_of_heroes() - 1)) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        if (!world.get_players()[this->clientId].isHeroSelected())
        {
            int current_hero = world.get_players()[this->clientId].get_selected_hero();
            current_hero += 1;
            world.get_players()[this->clientId].set_selected_hero(current_hero);
            while (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {}
        }
    }

    // Confirming hero selection
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    {
        if (!world.get_players()[this->clientId].isHeroSelected())
        {
            // Creating a packet
            sf::Packet packet;

            // Forming a packet for sending selected hero id
            packet << Message::ClientHeroSelected << this->id() << world.get_players()[this->clientId].get_selected_hero();

            // Sending packet
            if (socket.send(packet) != sf::Socket::Done)
            {
                std::cout << "Can't send hero selection packet to server\n";
            }

            while (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {}
        }
    }
}

void Client::events_gameover(Viewer& viewer)
{
    // Changing buttons in lobby
    // Going right
    if ((viewer.get_gameover_selected_button() < (viewer.get_gameover_buttons().size() - 1)) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        viewer.set_gameover_selected_button(viewer.get_gameover_selected_button() + 1);
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {}
    }
    // Going left
    if ((viewer.get_gameover_selected_button() > 0) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        viewer.set_gameover_selected_button(viewer.get_gameover_selected_button() - 1);
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {}
    }

    // Pressing back to lobby button
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && (viewer.get_lobby_selected_button() == 0))
    {
        world.SetScene(Scene::Lobby);
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {}
    }

    // Pressing disconnect button
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && (viewer.get_lobby_selected_button() == 1))
    {
        this->disconnect();
        this->running = false;
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {}
    }
}
