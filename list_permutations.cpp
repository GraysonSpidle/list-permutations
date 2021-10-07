#include "list_permutations.h"

#define _MIN(a,b) ((a) < (b) ? (a) : (b))


/* Erases node_t at i and shifts elements over
* 
* changes where arr is pointing.
*/
node_t* erase(node_t* arr, unsigned long size, unsigned long i) {
	// Allocating an array 1 element smaller than arr
	node_t* temp = reinterpret_cast<node_t*>(malloc((size - 1) * sizeof(node_t)));
	if (!temp) // Malloc returned nullptr which means there's not enough space
		abort();

	// Copying all elements but the element at i
	memcpy(temp, arr, i * sizeof(node_t));
	memcpy(temp + i, arr + i + 1, (size - i - 1) * sizeof(node_t));

	// Deleting arr and replacing it with the new one.
	delete[] arr;
	arr = temp;
	return temp;
}

constexpr ulargest_t func(const ulargest_t i) noexcept {
	// I know this is a shitty name for this function. I honestly don't know what to call this.
	for (uchar_t k = 3; k < (uchar_t)-1; k++) {
		if (i < ((ulargest_t)1 << k) - k + 1)
			return ListPermutations::g(k);
	}
}

ulargest_t j(ulargest_t N, ulargest_t i) {
	// I know this is a shitty name for this function. I honestly don't know what to call this.
	return ((ulargest_t)1 << (i + 2 + N)) - i - ((ulargest_t)1 << (N + 1)) - (2 + N);
}

ulargest_t func2(ulargest_t i) noexcept {
	// I know this is a shitty name for this function. I honestly don't know what to call this.
	if (0 < i && i <= 2)
		return 2;
	if (i == 3)
		return 1;
	ulargest_t p = 3;
	for (; p < (ulargest_t)-1; p++) {
		if (i < j(p, 0)) {
			break;
		}
	}
	for (ulargest_t k = 0; k < p; k++) {
		if (i >= j(k, p - k - 1)) {
			return (ulargest_t)1 << k;
		}
	}
	return 0;
}

// Definitions 

constexpr ulargest_t ListPermutations::f(const uchar_t n) noexcept {
	if (n == 2)
		return 2;
	return (n - 1) * f(n - 1) + n;
}

constexpr ulargest_t ListPermutations::F(const uchar_t n) noexcept {
	return (((ulargest_t)n - 1) * (ulargest_t)n) / (ulargest_t)2 - 1;
}

constexpr ulargest_t ListPermutations::g(const uchar_t n) noexcept {
	if (0 < n && n < 3) {
		return n - 1;
	}
	return (n - 1) * g(n - 1) + 1;
}

constexpr ulargest_t ListPermutations::G(const uchar_t n) noexcept {
	return ((ulargest_t)1 << (n - 1)) - 1; // 2**(n-1) - 1
}

constexpr uchar_t ListPermutations::depth(uchar_t n, ulargest_t i) noexcept {
	ulargest_t I = (i - 1) % (g(n - 1));
	if (I && n > 3)
		return 1 + depth(n - 1, I);
	else
		return 0;
}

constexpr uchar_t ListPermutations::path_size(uchar_t n, ulargest_t i) noexcept {
	if (i)
		return n - (1 + depth(n, i));
	else
		return 0;
}


constexpr uchar_t ListPermutations::applied_path_strlen(uchar_t n, ulargest_t i) {
	if (!i)
		return n * 2 + 1;
	return (n - (1 + depth(n, i))) + n + 1;
}


constexpr ulargest_t ListPermutations::normalize(uchar_t n, ulargest_t i) {
	/*ulargest_t I = i % (g(n - 1));
	if (I)
		return I;
	else
		return g(n - 1);*/
	auto gn = g(n);
	return i - ((i - 1) / gn) * gn;
}


constexpr ulargest_t ListPermutations::normalize_to(uchar_t n, ulargest_t i, uchar_t to) noexcept {
	if (n < to)
		return 0;
	else if (n == to)
		return normalize(to, i);
	else
		return normalize_to(n - 1, normalize(n, i), to);
}


uchar_t* combos(uchar_t n, ulargest_t i) {
	if (!i)
		return nullptr; // Progenitor path doesn't have any combinations for the indices.
	const uchar_t d = ListPermutations::depth(n, i);
	uchar_t* output = new uchar_t[1 + d];
	output[0] = (i - 1) / ListPermutations::g(n - 1); // the first number
	if (n > 4) {
		uchar_t k = 1;
		if (d >= 1) { // everything in between
			for (; k < _MIN(d + 1, n - 3); k++) {
				output[k] = (ListPermutations::normalize_to(n, i - (k - 1), n - k) - 2) / ListPermutations::g(n - (k + 1));
			}
		}
		if (d == n - 3)
			output[k] = (d == ListPermutations::depth(n, i - 1));
	}
	else if (n == 4) {
		if (d == 1)
			output[1] = (d == ListPermutations::depth(n, i - 1));
	}
	return output;
}

node_t* ListPermutations::get_path(uchar_t n, ulargest_t i) {
	// Make the progenitor path

	node_t* output = new node_t[n];
	for (uchar_t k = 0; k < n; k++) {
		output[k][0] = k;
		output[k][1] = k + 1;
	}

	if (!i) // The progenitor path is at i = 0
		return output;

	// Combining the slices into the appropriate path for i

	uchar_t* _combos = combos(n, i);
	for (uchar_t k = 0; k < 1 + depth(n, i); k++) {
		output[_combos[k]][1] = output[_combos[k] + 1][1];
		output = erase(output, n - k, _combos[k] + 1);
	}
	delete[] _combos;
	return output;
}

ulargest_t ListPermutations::get_unique_path_index(uchar_t n, ulargest_t i) {
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

char* ListPermutations::apply_path(const char* cstring, const node_t* path, uchar_t path_size) {
	auto n = strlen(cstring);
	n = n * 2 + 1 - (n - path_size);
	char* output = new char[n + 1];

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

void ListPermutations::generate_paths(uchar_t n, node_t** const output) {
	// Pretty straightforward implementation.

	for (ulargest_t i = 0; i < g(n); i++) {
		output[i] = get_path(n, i);
	}
}

void ListPermutations::generate_unique_paths(uchar_t n, node_t** const output) {
	for (ulargest_t i = 0; i < G(n); i++) {
		output[i] = get_path(n, get_unique_path_index(n, i));
	}
}

char** ListPermutations::apply_paths(const char* cstring) {
	const auto n = strlen(cstring);
	node_t** paths = new node_t*[G(n)];
	generate_unique_paths(n, paths);

	char** output = new char*[G(n)];

	// Gotta do the progenitor separately

	output[0] = apply_path(cstring, paths[0], n);

	for (ulargest_t i = 1; i < G(n); i++) {
		output[i] = apply_path(cstring, paths[i], path_size(n,get_unique_path_index(n, i)));
	}

	// deleting the arrays

	for (ulargest_t i = 0; i < G(n); i++) {
		delete[] paths[i];
	}
	delete[] paths;

	return output;
}
