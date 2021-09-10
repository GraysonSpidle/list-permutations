#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>

#include "Functions2.h"
using namespace Functions;

std::vector<std::string> read_string_list(const char* path) {
	std::vector<std::string> vec;
	std::ifstream ifs{ path };
	std::string s;
	while (std::getline(ifs, s, '\n')) {
		vec.push_back(s);
	}
	ifs.close();
	return vec;
}

void print_paths(const uchar_t n) {
	node_t** output = new node_t*[g(n)];
	generate_paths(n, output);
	for (uchar_t k = 0; k < n; k++) {
		std::cout << "(" << std::to_string(output[0][k][0]) << "," << std::to_string(output[0][k][1]) << ") ";
	}
	std::cout << std::endl;
	for (ulargest_t i = 1; i < g(n); i++) {
		for (uchar_t k = 0; k < n - (1 + depth(n, i)); k++) {
			std::cout << "(" << std::to_string(output[i][k][0]) << "," << std::to_string(output[i][k][1]) << ") ";
		}
		std::cout << std::endl;
	}
	for (ulargest_t i = 0; i < g(n); i++) {
		delete[] output[i];
	}
	delete[] output;
}

void print_unique_paths(const uchar_t n) {
	node_t** output = new node_t*[G(n)];
	generate_unique_paths(n, output);

	// printing the stuff

	for (uchar_t k = 0; k < n; k++) {
		std::cout << "(" << std::to_string(output[0][k][0]) << "," << std::to_string(output[0][k][1]) << ") ";
	}
	std::cout << std::endl;
	for (ulargest_t i = 1; i < G(n); i++) {
		for (uchar_t k = 0; k < n - (1 + depth(n, get_unique_path_index(n, i))); k++) {
			std::cout << "(" << std::to_string(output[i][k][0]) << "," << std::to_string(output[i][k][1]) << ") ";
		}
		std::cout << std::endl;
	}

	// deleting the arrays
	for (ulargest_t i = 0; i < G(n); i++) {
		delete[] output[i];
	}
	delete[] output;
}

void print_combos(const uchar_t n) {
	uchar_t* stuff;
	for (ulargest_t i = 1; i < g(n); i++) {
		stuff = combos(n, i);
		std::cout << i << ":";
		for (uchar_t k = 0; k < 1 + depth(n, i); k++) {
			std::cout << " " << std::to_string(stuff[k]);
		}
		std::cout << std::endl;
		delete[] stuff;
	}
}

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cerr << "Incorrect syntax. Supply the string to apply the algorithm to." << std::endl;
		return 1;
	}
	if (strlen(argv[1]) < 4) {
		std::cerr << "Only supply strings bigger than 3 characters." << std::endl;
		return 2;
	}
	else if (strlen(argv[1]) > 21) {
		std::cerr << "Don't put strings larger than 21 characters. 64-bit limitations prevent us from going past 21 characters." << std::endl;
		return 2;
	}
	auto n = strlen(argv[1]);
	std::cout << "going through " << G(n) << " possible paths..." << std::endl;
	char** stuff = apply_paths(argv[1]);

	for (ulargest_t i = 0; i < G(n); i++) {
		std::cout << stuff[i] << std::endl;
	}

	for (ulargest_t i = 0; i < G(n); i++) {
		delete[] stuff[i];
	}
	delete[] stuff;


	return 0;
}