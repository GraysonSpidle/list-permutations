'''
This file contains functions that will aid in generating all permutations from [2 to n + 1) of a given string.
Take the word "cats" for example, which would yield:
c a t s
ca t s
cat s
ca ts
c at s
c ats
c a ts

It does this for ANY string size greater than 3. 

It is worth noting that EVERY time a variable/parameter is named "n", that represents how long the given string is.
The ONLY exception to this is when I'm expressing big O notation.

Author: Grayson Spidle
'''

from itertools import count
from math import log2
from operator import mul
from functools import reduce

# Core Functions ===============================================================

def f(n): # This isn't ever used in this file, but I'll leave it here in case it becomes useful later.
    ''' Number of nodes across all paths in the raw series. O(n). '''
    if n == 2:
        return 2
    return (n - 1) * f(n - 1) + n

def fu(n): # This isn't ever used in this file, but I'll leave it here in case it becomes useful later.
    ''' Number of nodes across all paths in the unique series. '''
    return sum(path_size(n, get_unique_path_index(n, i)) for i in range(G(n))) # I don't have an equation for this one

def F(n): # This isn't ever used in this file, but I'll leave it here in case it becomes useful later.
    ''' Number of unique nodes across all paths in the raw series (Yes the raw series not the unique series). O(1). '''
    return ((n - 1) * n // 2) - 1 # it's a triangle number - 1

def Fu(n): # This isn't ever used in this file, but I'll leave it here in case it becomes useful later.
    ''' Number of unique nodes across all paths in the unique series. O(n^2). '''
    return (n**2 + n - 2) // 2

def g(n):
    ''' Number of paths in the raw series. O(n). '''
    if 0 < n < 3:
        return n - 1
    return (n - 1) * g(n - 1) + 1

def G(n):
    ''' Number of paths in the unique series. O(1) '''
    return (1 << (n-1)) - 1 # it's a mersenne number

# Raw Series Functions =========================================================

def depth(n, i):
    ''' Returns the depth of the path at index i. Do not put 0 for i, as it will always return an incorrect number. O(n^2). '''
    I = (i - 1) % g(n - 1)
    if I and n > 3:
        return 1 + depth(n - 1, I)
    return 0

def path_size(n, i): # O(n^2)
    return n - (1 + depth(n, i)) * bool(i) # i = 0 returns n

def normalize(n, i):
    ''' Transforms the i value to its equivalent i value under the first header in the list diagram. '''
    gn = g(n)
    return i - ((i - 1) // gn) * gn

def normalize_to(n, i, to):
    ''' performs normalize(n, i) until n = to '''
    if n < to:
        return None
    elif n == to:
        return normalize(to, i)
    return normalize_to(n - 1, normalize(n, i), to)

def generate_transform_indices(n, i):
    ''' Returns a generator that spits out transformation indices for us to use on the
    progenitor path so we can make the path at index i.
    '''
    if i < 1: # 0 is the progenitor path and any negative number is undefined
        return None
    d = depth(n, i)
    yield (i - 1) // g(n - 1) # the first index
    if n > 4:
        if d >= 1: # all other indices
            # k is the index of the generated output we are on
            for k in range(1, min(d + 1, (n - 3))):
                l = (normalize_to(n, i - (k - 1), n - k) - 2)
                yield l // g(n - (k + 1))
        if d == n - 3: # the last index for the deepest paths
            yield int(d == depth(n, i - 1))
    elif n == 4:
        if d == 1:
            yield int(d == depth(n, i - 1))

def get_path(n, i):
    ''' Returns the ith path in the series for a sliceable of size n. '''
    output = list((i, i+1) for i in range(n)) # progenitor path
    for index in generate_transform_indices(n, i):
        output[index] = (output[index][0], output[index + 1][1])
        del output[index + 1]
    return output

# Unique Series Functions ======================================================

def _j(N, i):
    return (1 << (i + 2 + N)) - i  - (1 << (N + 1)) - (2 + N)
    #return 2**(i + 2 + N) - i - 2**(N + 1) - (2 + N)

def _func(i):
    ''' Returns the number to add to the total to convert a unique series index to a raw series index. '''
    for k in count(start=3):
        if i < (1 << k) - k - 1:
            return g(k)

def _func2(i):
    '''  How much to subtract the i value by. '''
    if 0 < i <= 2:
        return 2
    elif i == 3:
        return 1
    for p in count(3):
        if i < _j(p, 0):
            break
    for k in range(p):
        if i >= _j(k, p - k - 1):
            return 1 << k #2**k

def get_unique_path_index(n, i):
    ''' Returns an int that is the index param for get_path() for the ith element that is unique. Does include input. '''
    if n < 5:
        if n == 4:
            return (0,1,2,3,4,6,7)[i]
        return
    if 0 <= i <= n:
        return i
    # past this point we need to offset i by n.
    num = 0
    i -= n
    while i > 0:
        num += _func(i)
        i -= _func2(i)
    
    if i == -1:
        return num + n - 1
    elif i == 0:
        return num + n

# Endpoint Functions ===========================================================

def paths(n):
    ''' Returns a generator that yields every single path (with lots of duplicates) for a string of size n. '''
    for i in range(g(n)):
        yield get_path(n, i)

def unique_paths(n):
    ''' Returns a generator that yields every single path (with no duplicates) for a string of size n. '''
    for i in range(G(n)):
        yield get_path(n, get_unique_path_index(n, i))

def apply_path(sliceable, path):
    ''' Returns a generator that yields each sliced node the path creates with the sliceable object. '''
    for node in path:
        yield sliceable[slice(*node)]

def iterate_paths(sliceable):
    for upath in unique_paths(len(sliceable)):
        yield apply_path(sliceable, upath)

# Main =========================================================================

def make_list_diagram_raw(n):
    with open('%d.dat' % n, 'w') as file:
        for i in range(g(n)):
            if i:
                file.write("    " * depth(n, i))
            file.write(" ".join(str(e) for e in get_path(n, i)) + '\n')

def make_list_diagram_unique(n):
    with open('%d.dat' % n, 'w') as file:
        for i in range(G(n)):
            if i:
                file.write("    " * depth(n, i))
            file.write(" ".join(str(e) for e in get_path(n, i)) + '\n')

def indices(n, z):
    if z == n:
        yield 0
        return
    for i in range(1, G(n)):
        I = get_unique_path_index(n, i)
        if path_size(n, I) == z:
            yield i

if __name__ == "__main__":
    for e in iterate_paths("birds"):
        print(*e)
