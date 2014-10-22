#include "Meeting.h"
#include <algorithm>
#include <iterator>
using namespace std;
using namespace std::placeholders;

/* Meeting class - this class represents a Meeting in terms of a time, topic, and 
list of participants. 
Note: Meeting's interface and I/O functions assume that the time is expressed in 12-hour format.

Public functions provide for maintaining the list of participants,
but no direct access to it is allowed. 

An overloaded operator< is provided to simplify constructing a list of Meetings 
in order by time, and an output operator to simplify printing the Meeting information.

We let the compiler supply the destructor and the copy/move constructors and assignment operators.

*/

bool Meeting::comp_participants::operator() (const Person* lhs, const Person* rhs) const
{
	return lhs->get_lastname() < rhs->get_lastname();
}

// HELPER FUNCTION
void save_person(const Person*, std::ostream&);

// Construct a Meeting from an input file stream in save format
// Throw Error exception if invalid data discovered in file.
// No check made for whether the Meeting already exists or not.
// Person list is needed to resolve references to meeting participants
// Input for a member variable value is read directly into the member variable.
Meeting::Meeting(std::ifstream& is, const people_list_t& people)
{
	int num_participants;
	is >> time;
	is >> topic;
	is >> num_participants;

	// read in all participants
	std::string lastname;
	auto participant_itr = participants.begin();
	auto person_itr = people.begin();

	for(int i = 0; i < num_participants && is.good(); ++i) {
		is >> lastname;
		// try to find existing person
		Person probe(lastname);
		person_itr = lower_bound(people.begin(), people.end(), &probe, comp_participants());
		if (person_itr == people.end() || (*person_itr)->get_lastname() != probe.get_lastname())
			throw Error("Invalid data found in file!");

		// person exists so insert!
		participant_itr = lower_bound(participants.begin(), participants.end(), &probe, comp_participants());
		participants.insert(participant_itr, *person_itr);
	}

	if(!is)
		throw Error("Invalid data found in file!");
}

// Meeting objects manage their own participant list. Participants
// are identified by a pointer to that individual's Person object.

// Add to the list, throw exception if participant was already there.
void Meeting::add_participant(const Person* p)
{
	// need to probe the participants list
	if (is_participant_present(p))
		throw Error("This person is already a participant!");

	auto insert_pos = lower_bound(participants.begin(), participants.end(), p, comp_participants());
	participants.insert(insert_pos, p);
}
// Return true if the person is a participant, false if not.
bool Meeting::is_participant_present(const Person* p) const
{
	return binary_search(participants.begin(), participants.end(), p, comp_participants());
}
// Remove from the list, throw exception if participant was not found.
void Meeting::remove_participant(const Person* p)
{
	if (!is_participant_present(p))
		throw Error("This person is not a participant in the meeting!" );

	auto remove_pos = lower_bound(participants.begin(), participants.end(), p, comp_participants());
	participants.erase(remove_pos);
}
		
// Write a Meeting's data to a stream in save format with final endl.
void Meeting::save(std::ostream& os) const
{
	os << time << " " << topic << " " << participants.size() << endl;
	// bind save_person
	auto bound_save = bind(save_person, _1, ref(os));
	for_each(participants.begin(), participants.end(), bound_save);
}

void save_person(const Person* person, std::ostream& os)
{
	person->save(os);
}

// This operator defines the order relation between meetings, based just on the time
bool Meeting::operator< (const Meeting& other) const
{
	return normalize_time(time) < normalize_time(other.time);
}

// Print the Meeting data as follows:
// The meeting time and topic on one line, followed either by:
// the no-participants message with an endl
// or the partipcants, one per line, with an endl after each one
std::ostream& operator<< (std::ostream& os, const Meeting& meeting)
{
	os <<  "Meeting time: " << meeting.time << ", Topic: " << meeting.topic << "\nParticipants:";
	if (meeting.participants.empty()) {
		os << " None" << endl;
	} else {
		os << endl;
		ostream_iterator<const Person*> out_it(os, "\n");
		copy(meeting.participants.begin(), meeting.participants.end(), out_it);
	}
	return os;
}
