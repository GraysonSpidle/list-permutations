# Concepts
**WORK IN PROGRESS**

So we know how we can make list permutations by just relying on the previous element in the series, but how do we make it indexable?

## Properties of Our Problem
Before we tackle the answer to this problem, we must first understand our problem a bit more. Namely, the *properties* of our problem. These properties will be introduced throughout this file when they are needed.

### Length Dependency/Commutative Property
Ignoring values, list permutations of equal size are the same. To preserve brevity, the notation of `n = a number` will represent the size of a given list. So `n = 5` refers to a list with 5 elements.

Take the list diagrams for the strings "abcd" and "1234":
```
a b c d
ab c d
	abc d
	ab cd
a bc d
	a bcd
	a b cd
```
and
```
1 2 3 4
12 3 4
	123 4
	12 34
1 23 4
	1 234
	1 2 34
```
They both have the same number of permutations with the same structures to each one (ignoring the values). 
However, if we look at the list diagram for the string "birds":
```
b i r d s
bi r d s
    bir d s
        bird s
        bir ds
    bi rd s
        bi rds
        bi r ds
    b ir d s
        b ird s
        b irds
b ir ds
    b i rd s
        b i rds
        b i r ds
```
This does not have the same amount of permutations nor does it seem to follow the same structure, which indicates that list permutations are **length dependent**. Meaning, all lists--of equal size--will have their permutations form the same structure every time.

This length dependency will be the dependent variable in all subsequent applications and the commutative nature will be very useful when discussing our abstractions.

## Begin tutorial
At its core, a list permutation is a series of slices you perform on the input which result in the output permutations. For example, take the string "cat" whose list diagram looks like this:
```
c a t
ca t
c at
```
The progenitor path can be expressed in slices like this: `(0, 1) (1, 2) (2, 3)` and, when applied, looks like this: `"c" "a" "t"`.
The subsequent paths can be expressed in a similar fashion like this:
```
(0, 2) (2, 3)
(0, 1) (1, 3)
```
We can apply this to *any* `n = 3` list and get the same (and correct) results every time.

So how can we translate this into instructions a human and (more importantly) a machine can interpret?

The simplest solution involves recursion.

### The Raw Series
Our solution, like code, has bugs. One big one to be specific, but we will get there eventually.

The solution brought forth is quite simple. To make all the necessary paths, we can use good old fashioned recursion.

First we write the progenitor path (we'll use the one for a `n = 4` list):
`(0, 1) (1, 2) (2, 3) (3, 4)`
Now we take the 0th node and combine it with its neighbor to the right. 

Combining nodes is very easy. Retain the left node's start index and set the left node's end index to the right node's end index. Like this:
`(0, 1) + (1, 2) -> (0, 2)` 

In Python code, it would look like this:
```python
def combine_nodes(left:tuple, right:tuple):
	left = (left[0], right[1])
```

Performing that operation on our example, we should have something that looks like this:
`(0, 2) (1, 2) (2, 3) (3, 4)`
Then we remove the node to the left of the 0th node and now we are left with this:
`(0, 2) (2, 3) (3, 4)`

Then we do the process again, so combine 0th and 1st nodes:
`(0, 3) (2, 3) (3, 4)`
and remove the neighboring node:
`(0, 3) (3, 4)`

We continue this process until we have a path that is 2 nodes in size. Once achieved, we go up 1 recursion layer and instead combine the 1st and 2nd nodes:
So we started with: `(0, 2) (2, 3) (3, 4)`
and end with: `(0, 2) (2, 4) (3, 4)`
then remove the 2nd node: `(0, 2) (2, 4)`

Continue this until the left with the last node in the path. When this happens, go up another recursion layer and repeat this process.

In Python code, this entire process can be expressed like so:
```python
def permute(path):
	yield path
	if len(path) > 2:
		for i in range(len(path) - 1):
			temp = path[:] # copying path
			temp[i] = (path[i][0], path[i + 1][1]) # here's the combining
			del temp[i + 1]
			yield from permute(temp)
```

And now, when we apply this to our example, we get this raw list diagram:
```
(0, 1) (1, 2) (2, 3), (3, 4)
(0, 2) (2, 3) (3, 4)
	(0, 3) (3, 4)
	(0, 2) (2, 4)
(0, 1) (1, 3) (3, 4)
	(0, 3) (3, 4)
	(0, 1) (1, 4)
(0, 1) (1, 2) (2, 4)
	(0, 2) (2, 4)
	(0, 1) (1, 4)
```
This process, while it is simple, there are 2 problems:
 1. There are duplicates.
 2. The paths are not indexable.

These 2 problems are unacceptable for high performance computing.

We will address these problems in order after we get a better understanding of this new animal we have created.

This animal will be referred to as the **raw series**, as it is a series of paths that needs to be tempered, hence the "raw" part.

#### g(n)
First off, we need to know how many paths are in each raw series (dependent on `n`). That equation will be referred to as `g(n)` and the mathematical formula is this:

![formula](https://raw.githubusercontent.com/GraysonSpidle/list-permutations/main/concepts-images/gn.png)

*A proof would be provided here if I (the author) understood how to write them. Perhaps a smarter individual will write a proof or find a better formula, who knows.*

#### Recursive Property
The raw series is recursive in nature and incorporates all lesser sized raw series structures within it.

We can express this property with an `n = 4` and an `n = 3` raw list diagrams:
```
n = 4
(0, 1) (1, 2) (2, 3) (3, 4)
(0, 2) (2, 3) (3, 4)
    (0, 3) (3, 4)
    (0, 2) (2, 4)
(0, 1) (1, 3) (3, 4)
    (0, 3) (3, 4)
    (0, 1) (1, 4)
(0, 1) (1, 2) (2, 4)
    (0, 2) (2, 4)
    (0, 1) (1, 4)
```
```
n = 3
(0, 1) (1, 2) (2, 3)
(0, 2) (2, 3)
(0, 1) (1, 3)
```
Upon comparing the two, we can see that the `n = 4` raw list diagram seems to *incorporate* the `n = 3` raw list diagram (albeit with different values). This is not coincidental.

We can see this happen when we use the permuting process detailed above on one of the header paths for the `n = 4` raw list diagram.

What this means is that every raw list diagram is composed of `n - 1` header paths and under each of those are an `n - 1` raw list diagram for that header path, thus resulting in a **recursive nature**. This behavior continues until `n == 2` where it is *only* comprised of header paths.

This recursive nature gives way to a concept of depth.

#### Depth Property
In a list diagram, all paths (except the progenitor path) have what is called a "depth", which is an integer value representing how "deep" in the recursion the path is.

In all list diagrams shown thus far, some paths have been indented for some unknown reason. The indentation represents the path's depth, which will be useful in upcoming algorithms involving the unique series.

*How do we calculate the depth of a path?*

Given that we are going to have these series be indexable, it would be sensible to calculate the depth in reference to an index.

We are given `n` (the size of the list) and `i` (the index of the path), both are unsigned integers. 

To calculate depth we effectively count every header path that is parent to the path at index `i`.
To do this, we subtract 1 from `i` and that modulo by `g(n - 1)`.
Then, we check if `i > 0` *and* if `n > 3`, if true then we add 1 to the depth and do the process again except with the new `i` and with `n - 1`.

In Python code, this process would look like this:
```python
def depth(n, i):
	I = (i - 1) % g(n - 1)
	if I and n > 3:
		return 1 + depth(n - 1, I)
	return 0
```

**Note that because the progenitor path (`i = 0`) does not have a depth, this function will give the wrong answer every time when `i = 0`**

### The Unique Series
Now we will address the first problem with the raw series: the fact that it has duplicates. We will address the second problem when we revisit the raw series.

To resolve this issue, we need a way of *only* getting the indices of the *unique* elements in the raw series. This will be referred to as the **unique series**.

First we will explore the iterative way *then* we will explore the indexable way. The concepts build upon each other.

#### G(n)
However, before we do any of that, we must make another equation that tells us how many paths are in the unique series. This equation will be referred to as `G(n)` and the mathematical formula for that is:

![formula](https://raw.githubusercontent.com/GraysonSpidle/list-permutations/main/concepts-images/_Gn.png)

Turns out, it's a Mersenne number, which is very nice for computers.

*A proof would be provided here if I (the author) understood how to write them. Perhaps a smarter individual will write a proof or find a better formula, who knows.*

#### Iterative Way
Onto the iterative way of generating indices for the unique elements in the raw series.

If we went through every path in an `n = 5` raw series and found the index of all the uniques, we would come up with a collection of indices as such:
`0, 1, 2, 3, 4, 5, 7, 8, 11, 15, 17, 18, 21, 28, 31`
and for an `n = 6` raw series we would get this collection of indices:
	`0, 1, 2, 3, 4, 5, 6, 8, 9, 12, 16, 18, 19, 22, 29, 32, 42, 53, 57, 59, 60, 63, 70, 73, 83, 104, 111, 114, 124, 155, 165`

If we analyzed these indices, we can identify a few patterns:
1. 0 is first.
2. the indices count up to `n` then skip `n + 1` and include `n + 2` and `n + 3`.
3. the indices after `n + 3` are irrelevant, as we will be generating them with this nifty trick.

##### Generating the rest of the indices
Note: this upcoming trick *only* works for indices when `n > 4`, which isn't that big of a deal since the ones it doesn't work for are super easy and can be hard-coded.

To generate the remaining indices, we must get the indices that we know for a fact. So in the case of `n = 5`, those indices would be `0, 1, 2, 3, 4, 5, 7, 8`.
- Add those indices to the `output` which will be a list.
- Take the last `G(n - 2)` indices from `output`: `5, 7, 8`.
- Prepend a `(n - 5) * g(n - 1) + 1` (which would be a 1 in this case): `1, 5, 7, 8`.
- Add `g(n - 1)` to every index: `11, 15, 17, 18` and append these indices to `output`.

We will repeat the process with some number changes until `G(n - k) = G(1)` (so 1 more time).
- Take the last `G(n - 3)` indices from `output`: `18`
- Prepend a `(n - 4) * g(n - 1) + 1`: `11, 18`
- Add `g(n - 1)` to each index: `21, 28` and append to `output`.

Finally, for the last time we skip the first step.
- ~~Take the last `G(n - 4)` indices from `output`, which erroneously gives us the entire thing.~~
- Prepend a `(n - 3) * g(n - 1) + 1`: `21`
- Add `g(n - 1)` to each index: `31` and append to `output`.

`output` will have the correct indices for each unique element.

*What if `n > 5`?*

This process evolves when `n` gets bigger. To demonstrate this, let's generate the remaining indices for `n = 6`.

Instead of starting with `0, 1, 2, 3, 4, 5, 6, 8, 9`, we start with the indices from `n = n - 1` which would be `n = 5` in this case: 
`0, 1, 2, 3, 4, 5, 7, 8, 11, 15, 17, 18, 21, 28, 31`
- Add `1` to every index and prepend a `0` : `0, 1, 2, 3, 4, 5, 6, 8, 9, 12, 16, 18, 19, 22, 29, 32`

And now we are ready to continue. Now we will build upon the process demonstrated for `n = 5`.
We do these steps until the first step tries to take the last `G(1)` elements. In other words, we do these steps `n - 3` times with `k` being the kth time (starting at `0`) we've iterated through the steps:
- Take the last `G(n - (2 + k))` indices from `output`: `5, 6, 8, 9, 12, 16, 18`
- Prepend a `k * g(n - 1) + 1`: `1, 5, 6, 8, 9, 12, 16, 18`
- Add `g(n - 1)` to each index: `42, 46, 47, 49, 50, 53, 57, 59`
- Append those to the `output`.
- increment `k`

In Python code, this process could be written like this:
```python
def iterative_way(n):
	if n == 4:
		return [0, 1, 2, 3, 4, 6, 7]
	elif n > 4:
		output = iterative_way(n - 1)
		for i in range(len(output)):
			output[i] += 1
		output.insert(0, 0)
		
		for k in range(n - 3):
			temp = output[-G(n - (2 + k)):]
			temp.insert(0, k * g(n - 1) + 1)
			for i in range(len(temp)):
				temp[i] += g(n - 1)
			output += temp
		output.append((n - 3) * g(n - 1) + 1 + g(n - 1))
		return output
```
 
I have an easier time understanding the process when looking at code, so *hopefully* I've done at least a passable job in explaining with words.

So that's the iterative way of enumerating the unique indices. It's effective, but still has the problem of not being indexable so we need an indexable version of this.

#### Indexable Way
This way is definitely better for speed, but for readability *and for my ability to name functions*, not so much. 

To aid in understanding what the awfully named functions do, We will dive into the new algorithm and how to do it by hand and I will point out the functions along the way. As always, I will have Python code snippets embedded (although they're in the Python file in the repo).

So the indexable way focuses on converting an index value for the unique series into an index value for the raw series.
We are given the size of the list `n` and the index `i` of the element **in the unique series** to be converted to an index for the raw series.

The conversion process is a synthesis of two series depending on the value of `i`
if `0 <= i <= n` we just return `i` because the first `n + 1` indices of every permutations in the raw series are **always** unique.

Otherwise `i > n` in which we go to the other series by subtracting `i` by `n` so we do: `i - n`.
Now set aside a variable to hold the sum of the numbers we're gonna get, we'll call it `output`.
Also, we're gonna be modifying `i`.

We do the next 2 steps while `i > 0` (when using unsigned numbers, consult my C++ version)

##### 1st Step
Before we do the 1st step, I want to visually show what the upcoming function is doing.

Here's a picture of that we will be using for reference.
![the image](https://raw.githubusercontent.com/GraysonSpidle/list-permutations/main/concepts-images/table.PNG)

Each number is a value for `i`. Once when the value for `i` is found, we look for what `g(n)` we add which is the first one when scanning the lines above the line `i` is on. For example, when `i = 27` we add `g(6)` to `output`.

Just to be clear, the first step is adding the appropriate `g(n)` number to `output`.

For a computer, this can be translated into a process that queries mathematical equations until a condition is met. There's likely a better way of achieving this, I've just found this one that works.

For an unsigned int `k`, where `k = 3`, if `(i - 1) < (2^k - (k + 2))` then add `g(k)` else increment `k`

In Python this is how it would look:
```python
from itertools import count
def _func(i):
	for k in count(start=3):
		if (i - 1) < 2**k - (k + 2):
			return g(k)
```
and yes I call it `_func()`, I don't know what to call this function.

##### 2nd Step


**TO BE CONTINUED**