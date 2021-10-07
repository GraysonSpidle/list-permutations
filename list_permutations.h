/* This file contains all the functions the end user will need.
* 
* If some of the terminology or thought processes don't make sense then I urge you to check out
* my markdown files on the repo. I try my best to explain everything.
* 
* Note that the functions that return a pointer are dynamically allocated and MUST be deallocated
* to avoid memory leaks.
* 
* Author: Grayson Spidle
*/


#ifndef __LIST_PERMUTATIONS_H__
#define __LIST_PERMUTATIONS_H__

#include <stdlib.h>
#include <cstring>
#include <cstdlib>

#define _MIN(a,b) ((a) < (b) ? (a) : (b))

typedef unsigned long long ulargest_t; // This should be the largest possible unsigned int the computer can use
typedef unsigned char uchar_t; // I use this type for the size of the list, this must be unsigned but can be bigger than a char.
using node_t = uchar_t[2]; // basically just a tuple<uchar_t, 2>

namespace ListPermutations {

	// Core Functions

	/* Returns the number of nodes across all paths in the raw series */
	constexpr ulargest_t f(const uchar_t n) noexcept;
	/* Returns the number of nodes across all paths in the unique series */
	constexpr ulargest_t F(const uchar_t n) noexcept;
	/* Returns the number of paths in the raw series */
	constexpr ulargest_t g(const uchar_t n) noexcept;
	/* Returns the number of paths in the unique series */
	constexpr ulargest_t G(const uchar_t n) noexcept;

	// Helper Functions

	/* Returns the depth of the path, in the raw series, at index i.
	*	Do not put 0 for i because you will get an incorrect answer since the progenitor path doesn't have a depth.
	*/
	constexpr uchar_t depth(uchar_t n, ulargest_t i) noexcept;

	/* Returns the size of the path at index i in the raw series. */
	constexpr uchar_t path_size(uchar_t n, ulargest_t i) noexcept;

	constexpr ulargest_t normalize(uchar_t n, ulargest_t i);
	constexpr ulargest_t normalize_to(uchar_t n, ulargest_t i, uchar_t to) noexcept;

	/* Returns the path index i in the raw series.
	* If you want the size of the path (you will), use path_size().
	*/
	node_t* get_path(uchar_t n, ulargest_t i);
	// Returns the ith index of an element in the raw series that is unique.
	ulargest_t get_unique_path_index(uchar_t n, ulargest_t i);

	// These "generate" functions are primarily useful for offloading work onto a GPU.

	void generate_paths(uchar_t n, node_t** const output);
	void generate_unique_paths(uchar_t n, node_t** const output);

	constexpr uchar_t applied_path_strlen(uchar_t n, ulargest_t i);
	char* apply_path(const char* cstring, const node_t* path, uchar_t path_size);
	char** apply_paths(const char* cstring);
};

#endif // __LIST_PERMUTATIONS_H__
