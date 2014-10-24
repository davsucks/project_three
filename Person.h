/* A Person object simply contains std::strings for a person's data.
Once created, the data cannot be modified. */

#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <fstream>
#include <set>
#include <map>

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

	// destroys all commitments associated with this person
	~Person();
	
	// Accessors
	std::string get_lastname() const
		{return lastname;}
	
	// Write a Person's data to a stream in save format with final endl.
	void save(std::ostream& os) const;

	// returns true if a person is committed at the provided time
	bool is_committed_at(int);

	// adds a commitment to this person commitments container
	void add_Commitment(int room_no, int time, std::string topic);

	// removes the commitment at the specified time from the persons commitments
	void remove_Commitment(int time);
	// prints all the commitments for a person
	void print_Commitments();

	void update_Commitment(int old_time, int new_room_no, int new_time);

	// This operator defines the order relation between Persons, based just on the last name
	bool operator< (const Person& rhs) const
		{return lastname < rhs.lastname;}

	bool operator== (const Person& rhs) const
		{return lastname == rhs.lastname;}

	friend std::ostream& operator<< (std::ostream& os, const Person& person);
	friend std::ostream& operator<< (std::ostream& os, const Person* person);
	

private:
	struct Commitment_t {
		Commitment_t(int, int, std::string);
		Commitment_t(int, int);
		int room_no;
		int time;
		std::string topic;
	};
	struct Comp_Commitments {
		bool operator() (const Commitment_t*, const Commitment_t*);
		bool operator() (const int&, const int&);
	};

	// two different containers, while slightly redundant, allows for commitment
	// lookup in logarithmic time since only using one container would
	// result in a linear search for is_committed_at, which is foribben as per the spec
	using Commitment_times_container_t = std::map<int, Commitment_t*, Comp_Commitments>;
	Commitment_times_container_t commitment_times;

	using Commitments_container_t = std::set<Commitment_t*, Comp_Commitments>;
	Commitments_container_t commitments;

	std::string firstname;
	std::string lastname;
	std::string phoneno;

public:

	friend std::ostream& operator<< (std::ostream& os, const Person::Commitment_t& commitment);
};

// output firstname, lastname, phoneno with one separating space, NO endl
std::ostream& operator<< (std::ostream& os, const Person& person);
std::ostream& operator<< (std::ostream& os, const Person* person);
// this outputs the commitment according to the style present in string.txt
std::ostream& operator<< (std::ostream& os, const Person::Commitment_t& commitment);

#endif
