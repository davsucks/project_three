#include "Room.h"
#include <algorithm>
using namespace std;
using namespace std::placeholders;

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

// struct used to compare meeting times
bool Room::comp_Meetings::operator() (const int& rhs, const int& lhs) const
{
	return normalize_time(rhs) < normalize_time(lhs);
}

void save_meeting(pair<int, Meeting*>, ostream&);

// Construct a Room from an input file stream in save format, using the people list,
// restoring all the Meeting information. 
// Person list is needed to resolve references to meeting participants.
// No check made for whether the Room already exists or not.
// Throw Error exception if invalid data discovered in file.
// Input for a member variable value is read directly into the member variable.

 Room::Room(std::ifstream& is, const people_list_t& people_list)
 {
 	is >> room_number;

 	int num_meetings;
 	is >> num_meetings;

 	try {
 	for (int i = 0; i < num_meetings && is.good(); ++i) {
 		Meeting* new_meeting = new Meeting(is, people_list, room_number);
 		pair<int, Meeting*> new_pair(new_meeting->get_time(), new_meeting);
 		meetings.insert(new_pair);
 	}
 	} catch (bad_alloc& e) {
 		print_message_and_quit();
 	}

 	if (!is)
 		throw Error("Invalid data found in file!");
 }
				
// Room objects manage their own Meeting container. Meetings are objects in
// the container. The container of Meetings is not available to clients.

// Add the Meeting, throw exception if there is already a Meeting at that time.
// A copy of the supplied Meeting is stored in the Meeting container.
void Room::add_Meeting(Meeting* m)
{
	if (is_Meeting_present(m->get_time()))
		throw Error("There is already a meeting at that time!");
	// add commitments
	meetings.insert( pair<int, Meeting*>(m->get_time(), m) );
}
// Return true if there is a Meeting at the time, false if not.
bool Room::is_Meeting_present(int time) const
{
	auto meeting = meetings.find(time);
	if (meeting == meetings.end())
		return false;
	return true;
}
// Return a reference if the Meeting is present, throw exception if not.
Meeting* Room::get_Meeting(int time) const
{
	if (!is_Meeting_present(time))
		throw Error("No meeting at that time!");
	auto pair = meetings.find(time);
	return pair->second;
}
// Remove the specified Meeting, throw exception if a Meeting at that time was not found.
void Room::remove_Meeting(int time)
{
	if (!is_Meeting_present(time))
		throw Error("No meeting at that time!");
	// erase the meeting from the container
	meetings.erase(time);
}

void Room::clear_Meetings()
{
	// deallocate every meeting node
	// NOTE: in deallocating a meeting node meetings destructor should be called
	// which will, in turn, deallocate every commitment associated with it
	for_each(meetings.begin(), meetings.end(), [](pair<int, Meeting*> meeting_p){ delete meeting_p.second; });
	// now clear the map
	meetings.clear();
}

// Write a Rooms's data to a stream in save format, with endl as specified.
void Room::save(std::ostream& os) const
{
	os << room_number << " " << meetings.size() << endl;
	auto bound_save = bind(save_meeting, _1, ref(os));
	for_each(meetings.begin(), meetings.end(), bound_save);
}

void save_meeting(pair<int, Meeting*> meeting, ostream& os)
{
	meeting.second->save(os);
}
	
// Print the Room data as follows:
// The room heading with room number followed by an endl, followed by either:
// The no-meetings message with an endl, or
// The information for each meeting, which should automatically have a final endl
std::ostream& operator<< (std::ostream& os, const Room& room)
{
	os << "--- Room " << room.get_room_number() << " ---" << endl;
	if (room.has_Meetings())
		for_each(room.meetings.begin(), room.meetings.end(), [&os](pair<int, Meeting*> meeting_pair) { os << *(meeting_pair.second); });
	else
		os << "No meetings are scheduled" << endl;
	return os;
}
