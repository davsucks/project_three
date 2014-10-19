#ifndef UTILITY_H
#define UTILITY_H
/* Utility functions, constants, and classes used by more than one other modules */

#include <iostream>
#include <vector>
#include <list>
#include <string>

// a simple class for error exceptions - msg points to a C-string error message
struct Error {
	Error(const char* msg_ = "") :
		msg(msg_)
		{}
	const char* msg;
};

int normalize_time(int);

// forward declare class Person
class Person;
void p_person(const Person*, std::ostream&);

// Declare types 
using people_list_t = std::list<const Person*>;
using people_itr_t = people_list_t::iterator;

// used in main as well as meeting.cpp
people_itr_t get_Person_itr(people_list_t&, std::string);

// forward declare class Room
class Room;
using room_list_t = std::vector<Room>;
using room_iterator_t = room_list_t::iterator;

#endif
