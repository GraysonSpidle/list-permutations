# list-permutations

After some digging, what I made is basically this https://newbedev.com/find-all-list-permutations-of-splitting-a-string-in-python except mine is indexable, whereas his is reliant on the previous element.

Adding the capability of indexing our elements is a huge pro because we can easily parallelize this operation *and* we can more easily translate this over to other (faster) languages like C.

I plan on making a video series that sheds light on this. I feel that a video is a far superior medium than text. Also I strongly dislike reading/writing papers.

I have a Python and C++ version of this.

- The Python version should be useful in understanding what's going on. 
- The C++ version is far better performing *and* is written in such a way for you to convert it into CUDA code.

Compiling `list_permutations_exe.cpp` will give you an exe that you must use on the command line.
