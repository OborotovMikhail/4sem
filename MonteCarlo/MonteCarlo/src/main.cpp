#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <cmath>
#include <future>

// Calculate circle area using Monte Carlo method
// (for 2D)

// DONE

// Creates random dot and checks if it is inside the circle
// Returns 1 if inside, 0 if outside
int IfInside(float R) {
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count(); // Random seed
    std::default_random_engine generator(seed); // Generator

    std::uniform_real_distribution<float> distr(-R, R); // Creating distribution

    float x = distr(generator); // Random x
    float y = distr(generator); // Random y
    float dist = std::sqrt(x * x + y * y); // Distance to dot
    
    if (dist < R)
    {
        // If dot is inside the circle
        return 1;
    }
    else
    {
        // If dot is outside the circle
        return 0;
    }
}

int main()
{
    float R = 1; // Radius
    int N = 100000; // Number of threads, dots

    std::vector<std::future<int>> futures; // Vector of future int values

    for (int i = 0; i < N; i++)
    {
        // Pushing new futures
        futures.push_back(std::async(IfInside, R));
    }

    int k = 0; // Number of dots inside
    for (int i = 0; i < N; i++)
    {
        // Calculating a total number of dots inside
        // from the vector of futures
        k = k + futures[i].get();
    }

    // Printing result
    std::cout << "Number of dots inside: " << k << std::endl;
    std::cout << "Total number of dots: " << N << std::endl << std::endl;
    std::cout << "Area by Monte Carlo: " << float(k) / float(N) * (R * 2) * (R * 2) << std::endl;

    return 0;
}