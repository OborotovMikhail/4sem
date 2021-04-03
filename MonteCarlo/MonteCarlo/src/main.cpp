#include <iostream>
#include <vector>
#include <thread>
#include <random>

// Calculate circle area using Monte Carlo method
// (for 2D)

// TODO

int IfInside(int R) {
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count(); // Random seed
    std::default_random_engine generator(seed); // Generator

    std::uniform_real_distribution<float> distr(0, 2 * R); // Creating distribution

    float x = distr(generator);
    float y = distr(generator);


}

int main()
{
    int R = 1; // Radius
    int N = 100; // Number of threads

    std::vector<std::thread> threads;

    // Debug print
    for (int i = 0; i < 10; i++)
    {
        float dice_roll = distr(generator);
        std::cout << dice_roll << std::endl;
    }
    
    for (int i = 0; i < N; i++)
    {
        // threads.push_back();
    }

    return 0;
}