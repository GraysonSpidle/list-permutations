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
    ''' Number of unique nodes across all paths in the raw series (Yes the raw series not the unique series). O(n). '''
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
    ''' Returns the depth of the path at index i. Do not put 0 for i, as it will always return an incorrect number. '''
    I = (i - 1) % g(n - 1)
    if I and n > 3:
        return 1 + depth(n - 1, I)
    return 0

def path_size(n, i):
    ''' Returns the size of the ith path for a sliceable of size n. '''
    return n - (1 + depth(n, i)) * bool(i) # i = 0 returns n

def normalize(n, i):
    ''' Transforms the i value to its equivalent i value under the first header in the list diagram. '''
    gn = g(n)
    return i - ((i - 1) // gn) * gn

def normalize_to(n, i, to):
    ''' performs normalize(n - 1, i) until n = to, n >= to.'''
    if n < to:
        return None
    elif n == to:
        return normalize(to, i)
    return normalize_to(n - 1, normalize(n, i), to)

def generate_transform_indices(n, i):
    ''' Returns a generator that spits out transformation indices for us to use on the
    progenitor path so we can make the path at index i.

    O(n)?
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
    ''' Returns the ith index which has 2^N elements in its row in thing.dat 
    It also is the first in its row.
    '''
    return (1 << (i + 2 + N)) - i - (1 << (N + 1)) - (2 + N)
    #return 2**(i + 2 + N) - i - 2**(N + 1) - (2 + N)

def _func(i):
    ''' Returns the number to add to the total to convert a unique series index to a raw series index. '''
    for k in count(start=3): # optimize this!
        if i < (1 << k) - k - 1:
            return g(k)

def _func2(i):
    '''  How much to subtract the i value by. '''
    if 0 < i <= 2:
        return 2
    elif i == 3:
        return 1
    # optimize this!
    for p in count(3):
        if i < _j(p, 0):
            break
    for k in range(p):
        if i >= _j(k, p - k - 1):
            return 1 << k #2**k

def get_unique_path_index(n, i):
    ''' Returns an int that is the index param for get_path() for the ith element that is unique. Does include input. 
    O(n^2) '''
    if n < 5:
        if n == 4:
            return (0,1,2,3,4,6,7)[i]
        return
    if 0 <= i <= n:
        return i
    # past this point we need to offset i by n.
    num = 0
    i -= n
    while i > 0: # optimize this!
        num += _func(i)
        i -= _func2(i)
    
    if i == -1:
        return num + n - 1
    elif i == 0:
        return num + n

# Experimental Functions ======================================================

def end_indices():
    ''' 
    So this function will generate all the end indices as seen in thing.dat. 
    That is, the indices that are on the end of each row in thing.dat.

    If someone were to find the inverse of this, it would save sooooo
    much time during computations because the end indices are the slowest
    ones to calculate which would bring the big O down to linear scaling.

    Then you can use a special formula to go right to the answer.

    It appears that you must use something called a "lambert w function"
    and I'm not clear on what that is or, more importantly, can it be
    implemented using just integers.
    '''

    for i in count(0):
        y = int( ((8*i + 17)**0.5 - 5) // 2 + 1 )
        yield (1 << (y + 3)) - y - 4 - ( 1 << ( (y*(y+5)) // 2 + 1 - i ) )

# Endpoint Functions ===========================================================

def paths(n, r=None):
    ''' Returns a generator that yields every single path (with lots of duplicates) for a string of size n. '''
    for i in range(g(n)):
        if r is None or path_size(n, i) in r:
            yield get_path(n, i)

def unique_paths(n, r=None):
    ''' Returns a generator that yields every single path (with no duplicates) for a string of size n. '''
    for i in range(G(n)):
        I = get_unique_path_index(n, i)
        if r is None or path_size(n, I) in r:
            yield get_path(n, I)

def apply_path(sliceable, path):
    ''' Returns a generator that yields each sliced node the path creates with the sliceable object. '''
    for node in path:
        yield sliceable[slice(*node)]

def iterate_paths(sliceable, r=None):
    for upath in unique_paths(len(sliceable), r):
        yield apply_path(sliceable, upath)

def generate_templates_unique(n, r=None):
    for i in range(1, G(n)):
        I = get_unique_path_index(n, i)
        if r is None or path_size(n, I) in r:
            yield tuple(generate_transform_indices(n, I))

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
            I = get_unique_path_index(n, i)
            if i:
                file.write("    " * depth(n, I))
            file.write(" ".join(str(e) for e in get_path(n, I)) + '\n')

def make_list_diagram_transform_indices_unique(n):
    with open('%d.dat' % n, 'w') as file:
        for i in range(G(n)):
            I = get_unique_path_index(n, i)
            if not i:
                continue
            file.write("    " * depth(n, I))
            file.write(" ".join(str(e) for e in generate_transform_indices(n, I)) + '\n')

def indices(n, z):
    if z == n:
        yield 0
        return
    for i in range(1, G(n)):
        I = get_unique_path_index(n, i)
        if path_size(n, I) == z:
            yield i

def t(n):
    return (n**2) // 2 - (3 * n) // 2 - 1

def thing(i):
    if i:
        return ((1 << (i + 3)) - 1) ^ 3
    return 1

def is_based_on_zero(i) -> bool:
    return bool((log2(i + 3) - 2) % 1)

def generate_thing_dat(n):
    yield [0]
    i = 1
    for p in range(1, n):
        for P in range(p, -1, -1):
            temp = []
            for k in range(2**P):
                temp.append(i)
                i += 1
            yield temp

def write_thing_dat(n):
    length = len(str(2**(n+1) - (3+n)))
    with open("thing.dat", 'w') as file:
        for line in generate_thing_dat(n):
            formatted_line = " ".join((["{:%dd}" % (length)] * len(line)))
            file.write(formatted_line.format(*line) + "\n")



def get_children(i):
    output = []
    for k, line in enumerate(mat):
        if line[i]:
            output.append(k)
    return output

def find_parents(i):
    parents = []
    for k in range(i+1, len(mat)):
        if i in get_children(k):
            parents.append(k)
            if len(parents) == 2:
                return parents
    return parents

def is_it_one(i):
    parents = find_parents(i)
    if len(parents) == 1:
        return False
    thing = mat[i]
    while any(thing):
        if thing.count(True) > 1:
            return False
        i = thing.index(True)
        thing = mat[i]
    return True

H = lambda n,c: int(log2(n + 3 + c)) - int(log2(n+2+c))
def H2(k, n):
    if k < 2:
        return list(filter(lambda e: H(e, k), range(n)))[k:]
    return list(filter(lambda e: H(e, k), range(n)))[k-1:]

def is_pathological(i):
    int(log2(i + 3))

if __name__ == "__main__":
    write_thing_dat(9)
    print("done")
    n = G(15)

    # col to row
    mat = list(list(False for k in range(n)) for i in range(n))

    for i in range(2, n):
        mat[i - _func2(i)][i] = True
    

    print()