#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <functional>

bool IsPunct(char c) {
	return std::ispunct(c) || std::isspace(c) || std::isdigit(c);
}

std::string PrepareString(std::string string) {
	// Preparing a string of text for word counting
	// Replacing all punctuations with spaces
	std::remove_if(string.begin(), string.end(), IsPunct);
	std::transform(string.begin(), string.end(), string.begin(), tolower);

	return string;
}

bool IsSame(std::string first, std::string second) {
    return (first == second);
}

int main()
{
    // Getting a number of strings
    std::cout << "Input the number of strings:" << std::endl;
    int N;
    std::cin >> N;

    // Getting strings, preparing them, pushing to vector
    std::cout << std::endl << "Input " << N << " strings:" << std::endl;
    std::vector<std::string> strings;
    for (int i = 0; i < N; i++) {
        std::string new_string;
        // std::getline(std::cin, new_string, '\n');
        new_string = PrepareString(new_string);
        strings.push_back(new_string);
    }

    std::unique(strings.begin(), strings.end());

    // Getting a number of strings
    std::cout << std::endl << "Result:" << std::endl;
    for (int i = 0; i < strings.size(); i++) {
        std::cout << strings[i] << std::endl;
    }

    return 0;
}