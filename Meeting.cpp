#include "Meeting.h"
#include <algorithm>
#include <iterator>
#include <functional>
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
Meeting::Meeting(std::ifstream& is, const people_list_t& people, int room_number)
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
		person_itr = find_if_not(people.begin(), people.end(), [lastname](const Person* p){ return p->get_lastname() < lastname; });
		if (person_itr == people.end() || (*person_itr)->get_lastname() != lastname)
			throw Error("Invalid data found in file!");

		// person exists so insert!
		participant_itr = lower_bound(participants.begin(), participants.end(), *person_itr, comp_participants());
		// this is how load programatically creates commitments invisible to
		// client code
		(*person_itr)->add_Commitment(room_number, time, topic);
		participants.insert(participant_itr, *person_itr);
	}

	if(!is)
		throw Error("Invalid data found in file!");
}

Meeting::~Meeting()
{
	// just need to destroy all the commitments associated with everyone
	// attending this meeting

	for_each(participants.begin(), participants.end(), [this](Person* p){ p->remove_Commitment(this->time); });
}

// Meeting objects manage their own participant list. Participants
// are identified by a pointer to that individual's Person object.

// Add to the list, throw exception if participant was already there.
void Meeting::add_participant(Person* p, int room_number)
{
	// need to probe the participants list
	if (is_participant_present(p))
		throw Error("This person is already a participant!");

	// make sure the person isn't already committed
	if (p->is_committed_at(time))
		throw Error("Person is already committed at that time!");

	auto insert_pos = lower_bound(participants.begin(), participants.end(), p, comp_participants());

	// add the commitment
	p->add_Commitment(room_number, time, topic);
	// add the participant
	participants.insert(insert_pos, p);
}
// Return true if the person is a participant, false if not.
bool Meeting::is_participant_present(Person* p) const
{
	return binary_search(participants.begin(), participants.end(), p, comp_participants());
}
// Remove from the list, throw exception if participant was not found.
void Meeting::remove_participant(Person* p)
{
	if (!is_participant_present(p))
		throw Error("This person is not a participant in the meeting!" );

	auto remove_pos = lower_bound(participants.begin(), participants.end(), p, comp_participants());

	// remove the person's commitment
	p->remove_Commitment(time);
	// remove the person from participants
	participants.erase(remove_pos);
}

// returns true if any participants in this meeting are committed at the new time
bool Meeting::any_participants_committed(int time)
{
	return any_of(participants.begin(), participants.end(), [time](Person* p) { return p->is_committed_at(time); });
}

void Meeting::update_Commitments(int old_time, int new_room_no, int new_time)
{
	for_each(participants.begin(), participants.end(), [old_time, new_room_no, new_time](Person* p){ p->update_Commitment(old_time, new_room_no, new_time); });
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
