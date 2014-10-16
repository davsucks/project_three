project_three
=============

To do
-----
1. Replace custom containers with stl containers.
2. Concatenate the two characters of thecommand into a single string, use this to index command map
3. Replace command dispatching to use map of function pointers (make sure to test for qq explicitly)
4. declare a simple struct that just holds the room and people list, and pass this (by reference) to every command function 

Restrictions
------------
- No inheritance
- No smart pointers
- No standard c libraries
- No declared or dynamically allocated built-in arrays (except for declaring const char* to point to string literals for error messages)

Container Requirements
----------------------
- Must use a `map`, `set`, `vector`, and `list` at least once, each with the following restrictions:
	- At least one of the vectors must be used with `binary_search` or `lower_bound`
	- Except for the list, searching any container must be logarithmic in time
	- People objects must be referred to with pointers in the containers
	- Containers that hold the current meetings, rooms, and people must be persistent between commands.

Algorithm Requirements
----------------------
- You cannot use any explicit `for`, `range for`, `while`, or `do-while` loops in the projects, use STL algorithms instead
	- Use algorithms like `find`, `copy`, and `for_each` to operate on containers
- There are exactly four (4) exceptions to this rule:
	- Your top level command loop must be in a `while` or `do-while` loop.
	- Your file restore code in the `ld` command must use explicit loops to control create of new data. (Note that the formats are unchanged).
	- The function that skips the rest of the input can be in a `while` loop.
	- **Exactly once** you have to use a `range-for`
		- **IMPORTANT:** put a comment before it that says "the one range for" so kieras can find it easily.
		- The body of the `range for` should be plain code

Specific Requirements for Algorithms and Containers
---------------------------------------------------
- Minus the `list` container, anytime a container is *searched* for a supplied Person, Meeting, or Room the search must be done in logarithmic time. ***Note:*** do not use a `binary_search` or `lower_bound` algorithm on the `list` container.
- You must use the `copy` algorithm with an output stream *at least once*. Remember you can define additional `operator<<` overloads to make this easy.
- You must use `std::bind` with bound values with an algorithm *at least once*
- You must use a custom functor that has member variables with getter/reader functions in combination with an algorithm and a container *at least once* in the project. *Hint*: Consider part of the **pa** command.
- You must use a lambda with a capture variable with an algorithm *at least once*.
- In all other places in your code where you use a Standard Library algorithm, you must *not* use your own helper function or custom function object class if `bind`, `mem_fn`, a lambda, an inserter a stream iterator or some other Standard Library facility will work just as well.