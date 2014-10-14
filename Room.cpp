#include "Room.h"
#include <stdexcept>
using namespace std;

/* A Room object contains a room number and a list containing Meeting objects stored with
meeting times as the key.  When created, a Room has no Meetings. When destroyed, the Meeting
objects in a Room are automatically destroyed.

Rooms manage the Meetings contained in them; functions are present for finding, adding,
or removing a Meeting specified by time.  The get_Meeting function returns a reference to the
specified Meeting, so that client code can modify the meeting - e.g. by adding a participant.
Note that modifying the time for a meeting in the container will disorder the meeting container, 
and so should not be attempted.

In addition, a Room can be asked to search for a particular Person being a participant 
in any of the Meetings in the Room. This makes it unnecessary for client code 
to be able to access the Meeting container in order to search for a specific participant.

We let the compiler supply the destructor and copy/move constructors and assignment operators.
*/ 

bool is_participant_in_meeting(const Meeting&, const Person*);
void print_meeting(const Meeting, ostream&);
void save_meeting(const Meeting, ostream&);

// Construct a Room from an input file stream in save format, using the people list,
// restoring all the Meeting information. 
// Person list is needed to resolve references to meeting participants.
// No check made for whether the Room already exists or not.
// Throw Error exception if invalid data discovered in file.
// Input for a member variable value is read directly into the member variable.
 Room::Room(std::ifstream& is, const Ordered_list<const Person*, Less_than_ptr<const Person*> >& people_list)
 {
 	is >> room_number;

 	int num_meetings;
 	is >> num_meetings;

 	for (int i = 0; i < num_meetings && is.good(); ++i) {
 		meetings.insert(Meeting(is, people_list));
 	}

 	if (is.bad())
 		throw runtime_error{"Invalid data found in file!"};
 }
				
// Room objects manage their own Meeting container. Meetings are objects in
// the container. The container of Meetings is not available to clients.

// Add the Meeting, throw exception if there is already a Meeting at that time.
// A copy of the supplied Meeting is stored in the Meeting container.
void Room::add_Meeting(const Meeting& m)
{
	if (is_Meeting_present(m.get_time()))
		throw runtime_error{"There is already a meeting at that time!"};
	meetings.insert(m);
}
// Return true if there is a Meeting at the time, false if not.
bool Room::is_Meeting_present(int time) const
{
	Meeting probe(time);
	if (meetings.find(probe) == meetings.end())
		return false;
	return true;
}
// Return a reference if the Meeting is present, throw exception if not.
Meeting& Room::get_Meeting(int time) const
{
	Meeting probe(time);
	auto itr = meetings.find(probe);
	if (itr == meetings.end())
		throw runtime_error{"No meeting at that time!"};
	return *itr;
}
// Remove the specified Meeting, throw exception if a Meeting at that time was not found.
void Room::remove_Meeting(int time)
{
	Meeting probe(time);
	auto itr = meetings.find(probe);
	if (itr == meetings.end())
		throw runtime_error{"No meeting at that time!"};
	meetings.erase(itr);
}
// Return true if the person is present in any of the meetings
bool Room::is_participant_present(const Person* person_ptr) const
{
	return apply_if_arg(meetings.begin(), meetings.end(), is_participant_in_meeting, person_ptr);
}

// Write a Rooms's data to a stream in save format, with endl as specified.
void Room::save(std::ostream& os) const
{
	os << room_number << " " << meetings.size() << endl;
	apply_arg_ref(meetings.begin(), meetings.end(), save_meeting, os);
}

void save_meeting(const Meeting meeting, ostream& os)
{
	meeting.save(os);
}
	
// Print the Room data as follows:
// The room heading with room number followed by an endl, followed by either:
// The no-meetings message with an endl, or
// The information for each meeting, which should automatically have a final endl
std::ostream& operator<< (std::ostream& os, const Room& room)
{
	os << "--- Room " << room.get_room_number() << " ---" << endl;
	if (room.get_number_Meetings())
		apply_arg_ref(room.meetings.begin(), room.meetings.end(), print_meeting, os);
	else
		os << "No meetings are scheduled" << endl;
	return os;
}

bool is_participant_in_meeting(const Meeting& meeting, const Person* person_ptr)
{
	return meeting.is_participant_present(person_ptr);
}

void print_meeting(const Meeting meeting, ostream& os)
{
	os << meeting;
}
