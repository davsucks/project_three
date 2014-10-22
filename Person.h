/* A Person object simply contains std::strings for a person's data.
Once created, the data cannot be modified. */

#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <fstream>

class Person {
public:
	Person(const std::string& firstname_, const std::string& lastname_, const std::string& phoneno_)
		: firstname(firstname_), lastname(lastname_), phoneno(phoneno_)
		{}
	// construct a Person object with only a lastname
	Person(const std::string& lastname_)
		: lastname(lastname_)
		{}

	// Disallow all forms of copy/move construction or assignment
	Person(Person& original) = delete;
	Person(Person&& original) = delete;

	Person& operator=(Person& original) = delete;
	Person& operator=(Person&& original) = delete;
	// These declarations help ensure that Person objects are unique,
	// like they are in the problem domain

	// Construct a Person object from a file stream in save format.
	// Throw Error exception if invalid data discovered in file.
	// No check made for whether the Person already exists or not.
	// Input for a member variable value is read directly into the member variable.
	Person(std::ifstream& is);
	
	// Accessors
	std::string get_lastname() const
		{return lastname;}
	
	// Write a Person's data to a stream in save format with final endl.
	void save(std::ostream& os) const;

	// This operator defines the order relation between Persons, based just on the last name
	bool operator< (const Person& rhs) const
		{return lastname < rhs.lastname;}

	bool operator== (const Person& rhs) const
		{return (!(this < &rhs) && !(&rhs < this));}

	friend std::ostream& operator<< (std::ostream& os, const Person& person);
	friend std::ostream& operator<< (std::ostream& os, const Person* person);

private:
	std::string firstname;
	std::string lastname;
	std::string phoneno;

};

// output firstname, lastname, phoneno with one separating space, NO endl
std::ostream& operator<< (std::ostream& os, const Person& person);
std::ostream& operator<< (std::ostream& os, const Person* person);

#endif
