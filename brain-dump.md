# Glossary
This whole thing revolves around the size of your sliceable object. I will **always** refer to the size of the sliceable object as `n`.

## List Diagram
- A **list diagram** is the human friendly way of viewing the series we will be dealing with. It is also a focal point that most functions use to do their operations. Note that these operations don't actually make list diagrams, but they use the concepts that list diagrams express.
- A list diagram's structure is dependent on `n`.
- A list diagram's structure can be described as such:
	- Every path in the list diagram gets its own line.
		- This means that line 0 is the 0th element in the series and line 1 is the 1st element and so on.
	- The 0th line is the **progenitor path** which is the path that makes every subsequent element in the sliceable object. 
		- This always has `n` nodes inside it.
	- Every list diagram has `n - 1` header paths which always have `n - 1` nodes inside them
	- After each header path is a copy of the `n - 1` list diagram structure (with values based on its header which acts like its progenitor path). This recursive behavior ends at `n = 3` where there are *only* header paths.
	- This recursive behavior also introduces a concept of **depth** for each path.
		- Except for the progenitor path, every path has a depth which represents how deep in the recursion that path is.
	- Every path is indented by its depth. 
		- I use 4 spaces per depth, but that isn't super important.
		- This is more or less optional, but I like to do this for the clarity it brings.

## Node
 - A **node** is a tuple which contains 2 unsigned integers a begin (inclusive) and end (exclusive).
 - Think of nodes as slices from languages like Python.
 - Nodes eventually are applied to any sliceable object (e.g, a string).
	 - nodes that have been applied to a sliceable object are referred to as **applied nodes**.
 - String representations of nodes are formatted as follows: `(first_number, second_number)`

## Path
 - A **path** is a collection of nodes that can be applied to a sliceable object.
	 - Paths are size dependent, which means they only work for a sliceable object of size n.
	 - Paths have a **depth** which refers to a phenomenon that occurs in list diagrams.
 - Paths primary use is to be applied to a sliceable object.
	 - paths that are applied to any sliceable object are referred to as **applied paths**.
 - String representations of paths are string representations of all their nodes delimited by a space.

## Series
### The Raw Series
- The **raw series** is a finite ordered series that contains all our paths. 
- There are a lot of duplicate paths, which is no good especially as `n` increases.
	- Despite this flaw, we still need this series so we can extract *only* the unique paths. 
### The Unique Series
- The **unique series** is a finite ordered series that contains *only* the unique paths from the raw series.
- This uniqueness is preserved **regardless** of the value of `n`.
- Indices in this series **do not** translate over to the raw series without conversion.
