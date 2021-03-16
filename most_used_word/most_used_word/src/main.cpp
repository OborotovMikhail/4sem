#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <sstream>

/*
* Program shows a list of most used words in a file
* 
* DONE
*/

// Path to the file
// Relative path does not seem to work, so using absolute path
#define FILE_PATH "D:/STUDY/MIPT/4_SEM/PROGA/4sem/most_used_word/most_used_word/bin/Release/WarPeace.txt"

// How many top used words will be showed
#define NUMBER_OF_WORDS 50

bool CompareUses(std::pair<std::string, size_t> a, std::pair<std::string, size_t> b) {
	// Compares the number of uses of 2 words

	return a.second > b.second;
}
bool IfPunctuation(char c) {
	// Checks if char is a punctuation symbol

	return std::ispunct(c) || std::isspace(c) || std::isdigit(c);
}

std::string MakeStringFromFile(std::ifstream& file) {
	// Makes a string of text from a file
	// then prepares it for easy use

	// Creating a string of text (from the file) to prepare text for word counting
	std::string string((std::istreambuf_iterator<char>(file)),
		(std::istreambuf_iterator<char>()));

	// Preparing a string of text for word counting
	// Replacing all punctuations with spaces
	std::replace_if(string.begin(), string.end(), IfPunctuation, ' ');
	std::transform(string.begin(), string.end(), string.begin(), tolower);
	
	return string;
}
std::map<std::string, size_t> MapOfWordUses(std::string string) {
	// Makes a map of words and its number of uses
	// from a text string

	// Creating a map of words and numbers of uses
	std::map<std::string, size_t> map;

	// Creating a stream from the prepared string of text
	std::istringstream text_stream(string);

	// Filling a map of words with a number of uses (basically just counting words)
	std::string key_word;
	while (text_stream >> key_word) {
		map[key_word]++;
	}

	return map;
}
std::vector<std::pair<std::string, size_t>> VectorOfMostUsedWords(std::map<std::string, size_t> map) {
	// Converting a map of words and uses to a vector of words and uses

	// Creating a vector
	std::vector<std::pair<std::string, size_t>> words_with_uses;
	// Pushing a map to vector
	for (std::pair<std::string, size_t> elem : map) {
		words_with_uses.push_back(elem);
	}

	// Sorting most used words
	std::sort(words_with_uses.begin(), words_with_uses.end(), CompareUses);

	return words_with_uses;
}

std::vector<std::pair<std::string, size_t>> MostUsedWords(std::ifstream& file) {
	// Function returns a vector of most used words (with a number of uses) from a file
	// The algorithm is as following:
	// INPUT ---> text in file --->
	// ---> text as string --->
	// ---> preparing --->
	// ---> text as stream --->
	// ---> a map of words and uses --->
	// ---> a vector of words and uses ---> 
	// ---> sorting by most used ---> OUTPUT

	// text in file ---> text as string + preparing
	std::string text_string = MakeStringFromFile(file);

	// text as string ---> text as stream ---> a map of words and uses
	std::map<std::string, size_t> word_uses_map = MapOfWordUses(text_string);

	// a map of words and uses ---> a vector of words and uses ---> sorting by most used
	std::vector<std::pair<std::string, size_t>> words_with_uses = VectorOfMostUsedWords(word_uses_map);

	return words_with_uses;
}

int main() {
	setlocale(LC_ALL, "rus");

	// Setting a number of most used words that will be printed (as size_t)
	size_t words_num = NUMBER_OF_WORDS;
	
	// Opening file
	std::ifstream file;
	file.open(FILE_PATH);
	if (file.is_open() != 1) {
		std::cout << "Could not open file\n" << std::endl;
	}

	// Creating a vector of most used words from file
	std::vector<std::pair<std::string, size_t>> words = MostUsedWords(file);

	// Printing a selected number (NUMBER_OF_WORDS) of most used words
	for (size_t i = 0; i < std::min(words_num, words.size()); i++) {
		std::cout << "Rank " << i + 1 << " --- \"" << words[i].first << "\" --- "
			<< words[i].second << " times used" << "\n";
	}

	return 0;
}