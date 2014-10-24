#include "Person.h"
#include "Utility.h"
#include <algorithm>
#include <cassert>
using namespace std;

// Commitment-related headers
// this compares commitment_t, first based on room number, and then based on time
bool Person::Comp_Commitments::operator() (const Person::Commitment_t* lhs, const Person::Commitment_t* rhs)
{
	if (lhs->room_no < rhs->room_no) return true;
	if (lhs->room_no == rhs->room_no) return normalize_time(lhs->time) < normalize_time(rhs->time);
	return false;
}

bool Person::Comp_Commitments::operator() (const int& lhs, const int& rhs)
{
	return normalize_time(lhs) < normalize_time(rhs);
}

// Commitment constructors
// this one is used for actually inserting commitments
Person::Commitment_t::Commitment_t(int _room_no, int _time, string _topic)
: room_no(_room_no), time(_time), topic(_topic)
{}

// this one is geared towards creating probes to search the commitments container
Person::Commitment_t::Commitment_t(int _room_no, int _time)
: room_no(_room_no), time(_time)
{}

Person::Person(ifstream& is)
{
	is >> firstname;
	is >> lastname;
	is >> phoneno;
	if (is.bad())
		throw Error("Invalid data found in file!");
}

Person::~Person()
{
	// people shouldn't be allowed to be deleted if they are in meetings 
	// aka they have commitments
	assert(commitment_times.empty());
	assert(commitments.empty());
}

// Write a Person's data to a stream in save format with final endl.
void Person::save(std::ostream& os) const
{
	os << lastname << endl;
}

bool Person::is_committed_at(int time)
{
	auto itr = commitment_times.find(time);
	if (itr == commitment_times.end())
		return false;
	return true;
}

// adds a commitment to this person commitments container
void Person::add_Commitment(int room_no, int time, string topic)
{
	Commitment_t* new_commitment;
	try {
		new_commitment = new Commitment_t(room_no, time, topic);
	} catch (...) {
		cerr << "There wasn't enough memory" << endl;
		exit(1);
	}
	commitments.insert(new_commitment);
	commitment_times.insert(pair<int, Commitment_t*>(time, new_commitment));
}

// removes the commitment at the specified time from the persons commitments
void Person::remove_Commitment(int time)
{
	// need to remove the commitment pointer from both containers
	// and then delete the pointer
	auto commitment = commitment_times.find(time);
	commitments.erase(commitment->second);
	commitment_times.erase(time);
	delete commitment->second;
}

void Person::print_Commitments()
{
	if (commitments.empty())
		cout << "No commitments" << endl;
	else {
		for_each(commitments.begin(), commitments.end(), [](const Commitment_t* c) { cout << *c << endl; });
	}
}

void Person::update_Commitment(int old_time, int new_room_no, int new_time)
{
	// grab the topic string from the old commitment
	auto commitment = commitment_times.find(old_time);
	string topic = commitment->second->topic;
	// remove the old commitment and add the new one
	remove_Commitment(old_time);
	add_Commitment(new_room_no, new_time, topic);
}


ostream& operator<< (ostream& os, const Person& person)
{
	os << person.firstname << " " << person.lastname << " " << person.phoneno;
	return os;
}

ostream& operator<< (ostream& os, const Person* person)
{
	os << *person;
	return os;
}

ostream& operator<< (ostream& os, const Person::Commitment_t& commitment)
{
	os << "Room:" << commitment.room_no << " Time: " << commitment.time << " Topic: " << commitment.topic;
	return os;
}