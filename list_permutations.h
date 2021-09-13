#ifndef __LIST_PERMUTATIONS_H__
#define __LIST_PERMUTATIONS_H__

#include <stdlib.h>
#include <cstring>
#include <cstdlib>

#define _MIN(a,b) ((a) < (b) ? (a) : (b))

typedef unsigned long long ulargest_t;
typedef unsigned char uchar_t;
using node_t = uchar_t[2];

node_t* erase(node_t* arr, unsigned long size, unsigned long i) {
	node_t* temp = reinterpret_cast<node_t*>(malloc((size - 1) * sizeof(node_t)));
	if (!temp)
		abort();
	memcpy(temp, arr, i * sizeof(node_t));
	memcpy(temp + i, arr + i + 1, (size - i - 1) * sizeof(node_t));
	delete[] arr;
	arr = temp;
	return temp;
}


uchar_t ulog2(ulargest_t arg) {
	for (uchar_t i = 0; i < sizeof(arg) * (ulargest_t)8; i++) {
		if (arg & ((ulargest_t)1 << i))
			return i;
	}
	return 0;
}

constexpr ulargest_t better_fac(const uchar_t i, const uchar_t n) {
	ulargest_t output = 1;
	for (uchar_t k = i; k < n; k++) {
		output *= k;
	}
	return output;
}

namespace ListPermutations {

	// Core Functions

	constexpr ulargest_t f(const uchar_t n) noexcept {
		if (n == 2)
			return 2;
		return (n - 1) * f(n - 1) + n;
	}

	 constexpr ulargest_t F(const uchar_t n) noexcept {
		return (((ulargest_t)n - 1) * (ulargest_t)n) / (ulargest_t)2 - 1;
	}
	 constexpr ulargest_t g(const uchar_t n) noexcept {
		ulargest_t output = 0;
		for (uchar_t i = 1; i < n - 2; i++) {
			output += better_fac(3 + i, n);
		}
		return output + 3 * better_fac(3, n);
		/*
		if (0 < n && n <= 3)
			return n - 1;
		else if (n > 3)
			return (n - 1) * (g(n - 1) + 1);
		else
			return 0;*/
	}
	  constexpr ulargest_t G(const uchar_t n) noexcept {
		return ((ulargest_t)1 << (n - 1)) - 1; // 2**(n-1) - 1
	}

	// Helper Functions

	 
	constexpr uchar_t depth(uchar_t n, ulargest_t i) noexcept {
		ulargest_t I = (i - 1) % (g(n - 1));
		if (I && n > 3)
			return 1 + depth(n - 1, I);
		else
			return 0;
	}

	 
	constexpr uchar_t applied_path_strlen(uchar_t n, ulargest_t i) {
		if (!i)
			return n * 2 + 1;
		return (n - (1 + depth(n, i))) + n + 1;
	}

	
	constexpr ulargest_t normalize(uchar_t n, ulargest_t i) {
		/*ulargest_t I = i % (g(n - 1));
		if (I)
			return I;
		else
			return g(n - 1);*/
		return i - ((i - 1) / g(n)) * g(n);
	}

	
	constexpr ulargest_t normalize_to(uchar_t n, ulargest_t i, uchar_t to) noexcept {
		if (n < to)
			return 0;
		else if (n == to)
			return normalize(to, i);
		else
			return normalize_to(n - 1, normalize(n, i), to);
	}

	
	uchar_t* combos(uchar_t n, ulargest_t i) {
		if (!i)
			return nullptr;
		const uchar_t d = depth(n, i);
		uchar_t* output = new uchar_t[1 + d];
		output[0] = (i - 1) / g(n - 1); // the first number
		if (n > 4) {
			uchar_t k = 1;
			if (d >= 1) { // everything in between
				for (; k < _MIN(d + 1, n - 3); k++) {
					output[k] = (normalize_to(n, i - (k - 1), n - k) - 2) / g(n - (k + 1));
				}
			}
			if (d == n - 3)
				output[k] = (d == depth(n, i - 1));
		}
		else if (n == 4) {
			if (d == 1)
				output[1] = (d == depth(n, i - 1));
		}
		return output;
	}

	
	node_t* get_path(uchar_t n, ulargest_t i) {
		node_t* output = new node_t[n];
		for (uchar_t k = 0; k < n; k++) {
			output[k][0] = k;
			output[k][1] = k + 1;
		}
		if (!i)
			return output;

		uchar_t* _combos = combos(n, i);
		for (uchar_t k = 0; k < 1 + depth(n, i); k++) {
			output[_combos[k]][1] = output[_combos[k] + 1][1];
			output = erase(output, n - k, _combos[k] + 1);
		}
		delete[] _combos;
		return output;
	}

	constexpr ulargest_t func(const ulargest_t i) noexcept {
		for (uchar_t k = 3; k < (uchar_t)-1; k++) {
			if ((i - 1) < ((ulargest_t)1 << k) - ((ulargest_t)k + 2))
				return g(k);
		}
	}

	ulargest_t j(ulargest_t N, ulargest_t i) {
		uchar_t offset;
		if (N == 1)
			offset = 2;
		else
			offset = 1;

		ulargest_t output = 1;
		for (ulargest_t k = 0; k < i + offset; k++) {
			output += G(k + 2 + ulog2(N));
		}
		output -= ((ulargest_t)2 + ulog2(N));
		return output;
	}

	ulargest_t func2(ulargest_t i) noexcept {
		if (0 < i && i <= 2)
			return 2;
		if (i == 3)
			return 1;
		ulargest_t p = 3;
		for (; p < (ulargest_t)-1; p++) {
			if (i < j((ulargest_t)1 << p, 0))
				break;
		}
		for (ulargest_t k = 0; k < p; k++) {
			if (i >= j((ulargest_t)1 << k, p - k - (k ? 1 : 2)))
				return (ulargest_t)1 << k;
		}
		return 0;
	}

	ulargest_t get_unique_path_index(uchar_t n, ulargest_t i) {
		if (n < 5) {
			if (n == 4) {
				uchar_t temp[] = { 0,1,2,3,4,6,7 };
				return temp[i];
			}
			return 0;
		}
		if (i <= n)
			return i;
		ulargest_t output = 0;
		i -= n;
		while (i > 1) {
			output += func(i);
			i -= func2(i);
		}
		switch (i) {
		case 0:
			output += n;
			break;
		case 1:
			output += func(1) + (n - 1);
			break;
		}
		return output;
	}
	
	char* apply_path(const char* cstring, const node_t* path, uchar_t path_size) {
		auto n = strlen(cstring);
		n = n * 2 + 1 - (n - path_size);
		char* output = new char[n+1];

		output[0] = '^';
		uchar_t offset = 1;
		for (uchar_t i = 0; i < path_size; i++) {
			memcpy(output + offset, cstring + path[i][0], path[i][1] - path[i][0]);	
			output[offset + (path[i][1] - path[i][0])] = ' ';
			offset += path[i][1] - path[i][0];
			offset += 1;
		}
		output[n - 1] = '$';
		output[n] = '\0';
		return output;
	}

	void generate_paths(uchar_t n, node_t** const output) {
		for (ulargest_t i = 0; i < g(n); i++) {
			output[i] = get_path(n, i);
		}
	}

	void generate_unique_paths(uchar_t n, node_t** const output) {
		for (ulargest_t i = 0; i < G(n); i++) {
			output[i] = get_path(n, get_unique_path_index(n, i));
		}
	}

	char** apply_paths(const char* cstring) {
		const auto n = strlen(cstring);
		node_t** paths = new node_t*[G(n)];
		generate_unique_paths(n, paths);

		char** output = new char*[G(n)];
		node_t* temp = get_path(n, 0);
		output[0] = apply_path(cstring, temp, n);
		delete[] temp;

		for (ulargest_t i = 1; i < G(n); i++) {
			output[i] = apply_path(cstring, paths[i], n - (1 + depth(n, get_unique_path_index(n, i))));
		}

		// deleting the arrays
		for (ulargest_t i = 0; i < G(n); i++) {
			delete[] paths[i];
		}
		delete[] paths;

		return output;
	}
};

#endif // __LIST_PERMUTATIONS_H__
