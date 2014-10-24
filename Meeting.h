/* Meeting class - this class represents a Meeting in terms of a time, topic, and 
list of participants. 
Note: Meeting's interface and I/O functions assume that the time is expressed in 12-hour format.

Public functions provide for maintaining the list of participants,
but no direct access to it is allowed. 

An overloaded operator< is provided to simplify constructing a list of Meetings 
in order by time, and an output operator to simplify printing the Meeting information.

We let the compiler supply the destructor and the copy/move constructors and assignment operators.

*/
#ifndef MEETING_H
#define MEETING_H

#include "Person.h"
#include "Utility.h"
#include <set>

class Meeting {
public:
	Meeting(int time_, const std::string& topic_)
		: time(time_), topic(topic_)
		{participants = Participants_t();}
	// construct a Meeting with only a time
		
	Meeting(int time_)
		: time(time_), topic()
		{participants = Participants_t();}
	// Construct a Meeting from an input file stream in save format
	// Throw Error exception if invalid data discovered in file.
	// No check made for whether the Meeting already exists or not.
	// Person list is needed to resolve references to meeting participants
	// Input for a member variable value is read directly into the member variable.
	Meeting(std::ifstream& is, const people_list_t& people, int room_number);

	// used to completely destroy commitments
	~Meeting();

	// accessors
	int get_time() const
		{return time;}
	void set_time(int time_)
		{time = time_;}

	// Meeting objects manage their own participant list. Participants
	// are identified by a pointer to that individual's Person object.

	// Add to the list, throw exception if participant was already there.
	void add_participant(Person* p, int room_number);
	// Return true if the person is a participant, false if not.
	bool is_participant_present(Person* p) const;
	// Remove from the list, throw exception if participant was not found.
	void remove_participant(Person* p);

	// returns true if any participants in this meeting are committed at the new time
	bool any_participants_committed(int time);

	void update_Commitments(int old_time, int new_room_no, int new_time);
			
	// Write a Meeting's data to a stream in save format with final endl.
	void save(std::ostream& os) const;

	// This operator defines the order relation between meetings, based just on the time
	bool operator< (const Meeting& other) const;
	
	friend std::ostream& operator<< (std::ostream& os, const Meeting& meeting);
		
private:
	struct comp_participants {
		bool operator() (const Person*, const Person*) const;
	};
	using Participants_t = std::set<Person*, comp_participants>;
	Participants_t participants;
	
	int time;
	std::string topic;
};

// Print the Meeting data as follows:
// The meeting time and topic on one line, followed either by:
// the no-participants message with an endl
// or the partipcants, one per line, with an endl after each one
std::ostream& operator<< (std::ostream& os, const Meeting& meeting);

#endif
