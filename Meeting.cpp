#include "Meeting.h"
#include "Utility.h"
#include <stdexcept>
using namespace std;

/* Meeting class - this class represents a Meeting in terms of a time, topic, and 
list of participants. 
Note: Meeting's interface and I/O functions assume that the time is expressed in 12-hour format.

Public functions provide for maintaining the list of participants,
but no direct access to it is allowed. 

An overloaded operator< is provided to simplify constructing a list of Meetings 
in order by time, and an output operator to simplify printing the Meeting information.

We let the compiler supply the destructor and the copy/move constructors and assignment operators.

*/

// HELPER FUNCTION
void save_person(const Person*, std::ostream&);

// Construct a Meeting from an input file stream in save format
// Throw Error exception if invalid data discovered in file.
// No check made for whether the Meeting already exists or not.
// Person list is needed to resolve references to meeting participants
// Input for a member variable value is read directly into the member variable.
Meeting::Meeting(std::ifstream& is, const Ordered_list<const Person*, Less_than_ptr<const Person*> >& people)
{
	int num_participants;
	is >> time;
	is >> topic;
	is >> num_participants;
	// read in all participants
	std::string lastname;
	auto itr = people.begin();
	for(int i = 0; i < num_participants && is.good(); ++i) {
		is >> lastname;
		// construct probe and try to find existing person
		Person probe(lastname);
		itr = people.find(&probe);
		if (itr == people.end())
			throw runtime_error{"Invalid data found in file!"};
		// participant found, so insert!
		participants.insert(*itr);
	}

	if(is.bad())
		throw runtime_error{"Invalid data found in file!"};
}

// Meeting objects manage their own participant list. Participants
// are identified by a pointer to that individual's Person object.

// Add to the list, throw exception if participant was already there.
void Meeting::add_participant(const Person* p)
{
	// need to probe the participants list
	if (participants.find(p) != participants.end())
		throw runtime_error{"This person is already a participant!"};

	participants.insert(p);
}
// Return true if the person is a participant, false if not.
bool Meeting::is_participant_present(const Person* p) const
{
	if (participants.find(p) == participants.end())
		return false;
	return true;
}
// Remove from the list, throw exception if participant was not found.
void Meeting::remove_participant(const Person* p)
{
	auto itr = participants.find(p);
	if (itr == participants.end())
		throw runtime_error{"This person is not a participant in the meeting!"};

	participants.erase(itr);
}
		
// Write a Meeting's data to a stream in save format with final endl.
void Meeting::save(std::ostream& os) const
{
	os << time << " " << topic << " " << participants.size() << endl;
	apply_arg_ref(participants.begin(), participants.end(), save_person, os);
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
		assert(meeting.participants.begin() == meeting.participants.end());
		os << " None" << endl;
	} else {
		assert(meeting.participants.begin() != meeting.participants.end());
		os << endl;
		apply_arg_ref(meeting.participants.begin(), meeting.participants.end(), p_person, os);
	}
	return os;
}
