#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

/* Task 1 from presentation 8
* 
* We have N strings (words separated by spaces)
* We need to print out unique strings without spaces
* and all letters must be lower-case
* 
* DONE
*/

std::string PrepareString(std::string string) {
	// Making all letters lower-case
	std::transform(string.begin(), string.end(), string.begin(), tolower);
    // Removing all spaces
    // and cutting the string to it's new size
    string.erase(std::remove_if(string.begin(), string.end(), std::isspace), string.end());
	return string;
}

int main()
{
    // Getting a number of strings
    std::cout << "Input the number of strings:" << std::endl;
    int N;
    std::cin >> N;

    std::cout << std::endl << "Input " << N << " strings:" << std::endl;
    std::string new_string;
    // Getting rid of the first empty string
    std::getline(std::cin, new_string);

    // Getting strings, pushing them to a vector
    std::vector<std::string> strings;
    for (int i = 0; i < N; i++) {
        std::getline(std::cin, new_string); // Reading
        new_string = PrepareString(new_string); // Preparing
        strings.push_back(new_string); // Pushing to vector
    }

    std::sort(strings.begin(), strings.end()); // Sorting a vector of strings
    strings.erase(std::unique(strings.begin(), strings.end()), strings.end()); // Removing duplicates

    // Printing result
    std::cout << std::endl << "Unique strings:" << std::endl;
    for (int i = 0; i < strings.size(); i++) {
        std::cout << strings[i] << std::endl;
    }

    return 0;
}