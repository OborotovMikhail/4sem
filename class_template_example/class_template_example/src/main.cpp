#include <iostream>
#include <algorithm>
#include <vector>

// Calculate asymptotic of the sorting algorithm
// Using template structure

// DONE

template <class T>
struct Comparator
{
    static size_t count;

    bool operator()(const T& x, const T& y)
    {
        ++count;
        return x < y;
    }

    void print_count()
    {
        std::cout << count << std::endl;
    }
};

template <class T>
size_t Comparator<T>::count = 0;

int main()
{
    std::vector<int> v;

    for (int i = 1; i < 6; i++)
    {
        std::cout << "For " << pow(10, i) << " numbers: ";
        for (int j = 0; j < pow(10, i); j++)
        {
            // Filling vector
            v.push_back(j);
        }

        std::random_shuffle(v.begin(), v.end()); // Unsorting
        std::sort(v.begin(), v.end(), Comparator<int>()); // Sorting

        std::cout << Comparator<int>().count / (pow(10, i) * log(pow(10, i))) << std::endl;
    }

    return 0;
}