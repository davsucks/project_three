#include "Person.h"
#include "Utility.h"
using namespace std;

// Construct a Person object from a file stream in save format.
// Throw Error exception if invalid data discovered in file.
// No check made for whether the Person already exists or not.
// Input for a member variable value is read directly into the member variable.
Person::Person(ifstream& is)
{
	is >> firstname;
	is >> lastname;
	is >> phoneno;
	if (is.bad())
		throw Error("Invalid data found in file!");
}

// Write a Person's data to a stream in save format with final endl.
void Person::save(std::ostream& os) const
{
	os << lastname << endl;
}

std::ostream& operator<< (std::ostream& os, const Person& person)
{
	os << person.firstname << " " << person.lastname << " " << person.phoneno;
	return os;
}

std::ostream& operator<< (std::ostream& os, const Person* person)
{
	os << *person;
	return os;
}
