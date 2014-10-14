project_three
=============

To do
-----
1. Concatenate the two characters of thecommand into a single string, use this to index command map
2. Replace command dispatching to use map of function pointers (make sure to test for qq explicitly)
3. declare a simple struct that just holds the room and people list, and pass this (by reference) to every command function
4. 

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