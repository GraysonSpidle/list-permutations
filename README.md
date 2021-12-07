# list-permutations

## What Does This Do?

This repo supplies a collection of functions that will aid the user in enumerating permutations of a list or a sliceable object.

## Usage

This is designed to be used for large scale applications such as preparing training data for a neural network.

If you're just looking for a one-off solution I recommend looking at this: https://newbedev.com/find-all-list-permutations-of-splitting-a-string-in-python

What sets this code apart from the code in that link is:
- It allows you to pick the kth permutation **without needing to iterate through the previous elements**.
  -  Makes the code easily parallelizeable (perfect for offloading work to a GPU).
- It abstracts the problem and generates a "template" that can be applied to **any** list or sliceable object of a specific size.
  -  This is what would ultimately save the user computation time when in a large scale setting.
  -  The abstraction allows the user to pick/exclude (based on r in `P(n,r)`) permutations without needing to load all the permutations.
  -  The templates are easy to apply to the original sliceable object.

`list_permutations.py` is the python version, just import like any module.

`list_permutations.h` is the C++ version, include it like any header file all functions that you need will be inside the namespace.
`list_permutations_exe.cpp` is not 100% needed. It's really just a demonstration of what the library can do.

If you're just interested in making the templates, don't use the Python version (unless you like waiting), instead use the C++ version.

## Dependencies
For the Python version:
- `itertools`

For the C++ version:
- `C++14`
- if you want to get past the 64-bit limitations (lists bigger than 21 elements) you'll need to get a large numbers library.

## Future Plans

- I plan on making a video series that sheds light on this. I feel that a video is a far superior medium than text. Also I strongly dislike reading/writing papers.
- I might make my own rudimentary large numbers implementation. If I do, I'll make it so that you can substitute it for something that's likely better.
- Make an example of a CUDA program with this.
- Optimizations for speed wherever possible. 
