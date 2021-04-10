#include <iostream>
#include <vector>
#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>

// Sound synthesizer
// DONE

int main()
{
    const int duration = 1000000; // Sound duration

    // Vector of frequencies
    std::vector<double> frequency = { 261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.00, 415.30, 440.00, 466.16, 493.88 };
    
    std::vector<std::vector<sf::Int16>> sound_samples(12); // Sound samples vector
    std::vector<sf::SoundBuffer> sound_buffer(12); // Sound buffer vector
    std::vector<sf::Sound> sound(12); // Sound player vector

    for (int j = 0; j < 12; j++)
    {
        for (double i = 0; i < duration; i += 1)
        {
            // Creating sound wave, pushing it to sound samples
            sound_samples[j].push_back(25000 * sin(2 * 3.141592 * frequency[j] * (i + 1) / 2 / 44100));
        }

        // Loading sample to buffer
        sound_buffer[j].loadFromSamples(&sound_samples[j][0], sound_samples[j].size(), 2, 44100);

        // Setting up sound player
        sound[j].setBuffer(sound_buffer[j]);
    }

    while (1) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && (sound[0].getStatus() != sf::Sound::Playing))
            sound[0].play();
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            sound[0].stop();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && (sound[1].getStatus() != sf::Sound::Playing))
            sound[1].play();
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            sound[1].stop();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && (sound[2].getStatus() != sf::Sound::Playing))
            sound[2].play();
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            sound[2].stop();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && (sound[3].getStatus() != sf::Sound::Playing))
            sound[3].play();
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            sound[3].stop();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::T) && (sound[4].getStatus() != sf::Sound::Playing))
            sound[4].play();
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::T))
            sound[4].stop();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y) && (sound[5].getStatus() != sf::Sound::Playing))
            sound[5].play();
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
            sound[5].stop();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::U) && (sound[6].getStatus() != sf::Sound::Playing))
            sound[6].play();
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::U))
            sound[6].stop();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::I) && (sound[7].getStatus() != sf::Sound::Playing))
            sound[7].play();
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::I))
            sound[7].stop();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::O) && (sound[8].getStatus() != sf::Sound::Playing))
            sound[8].play();
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::O))
            sound[8].stop();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && (sound[9].getStatus() != sf::Sound::Playing))
            sound[9].play();
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::P))
            sound[9].stop();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && (sound[10].getStatus() != sf::Sound::Playing))
            sound[10].play();
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            sound[10].stop();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && (sound[11].getStatus() != sf::Sound::Playing))
            sound[11].play();
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            sound[11].stop();
    }

    return 0;
}