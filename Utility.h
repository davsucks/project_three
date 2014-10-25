#ifndef UTILITY_H
#define UTILITY_H
/* Utility functions, constants, and classes used by more than one other modules */

#include <iostream>
#include <vector>
#include <list>

// a simple class for error exceptions - msg points to a C-string error message
struct Error {
	Error(const char* msg_ = "") :
		msg(msg_)
		{}
	const char* msg;
};

int normalize_time(int);

// Declare types 
// forward declare class Person
class Person;
using People_list_t = std::list<Person*>;

// forward declare class Room
class Room;
using Room_list_t = std::vector<Room>;

void print_message_and_quit();

#endif
