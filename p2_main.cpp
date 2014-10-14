#include "Ordered_list.h"
#include "String.h"
#include "Person.h"
#include "Meeting.h"
#include "Room.h"
#include "p2_globals.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
using namespace std;

// Custom Types
using people_list_t = Ordered_list<const Person*, Less_than_ptr<const Person*> >;
using people_iterator_t = people_list_t::Iterator;
using room_list_t = Ordered_list<Room>;
using room_iterator_t = room_list_t::Iterator;

// MAIN HELPER FUNCTIONS

void skip_line();
int read_room_number();
room_iterator_t read_no_and_get_room(room_list_t& rooms);
int read_time();

void pi(people_list_t&);

void pr(room_list_t&);
void check_room_no_in_range(int);
room_iterator_t get_room(room_list_t&, int);

void pm(room_list_t&);
void check_time_in_range(int);

void ps(room_list_t&);

void pg(people_list_t&);

void pa(people_list_t&, room_list_t&);
int num_meetings(room_list_t&);

void ai(people_list_t&);

void ar(room_list_t&);

void am(room_list_t&);

void ap(room_list_t&, people_list_t&);

void rm(room_list_t&);

void di(room_list_t&, people_list_t&);
bool is_participant_present_wrapper(Room room, const Person* p);

void dr(room_list_t&);

void dm(room_list_t&);

void dp(room_list_t&, people_list_t&);

void ds(room_list_t&);

void dg(room_list_t&, people_list_t&);
void dg_no_messages(room_list_t&, people_list_t&);

void da(room_list_t&, people_list_t&);
void da_no_messages(room_list_t&, people_list_t&);

void sd(room_list_t&, people_list_t&);

void ld(room_list_t&, people_list_t&);

int main() 
{
	people_list_t people;
	room_list_t rooms;
	char action;
	char item;
	while (true) {
		try {
		/* main program loop */
		cout << "\nEnter command: ";
		cin >> action;
		cin >> item;
		switch(action) {
			case 'p':
				switch(item) {
					case 'i': 
						pi(people);
						break;
					case 'r':
						pr(rooms);
						break;
					case 'm':
						pm(rooms);
						break;
					case 's':
						ps(rooms);
						break;
					case 'g':
						pg(people);
						break;
					case 'a':
						pa(people, rooms);
						break;
					default:
						cout << "Unrecognized command!" << endl;
						cin.clear();
						skip_line();
						break;
				}
				break;
			case 'a':
				switch(item) {
					case 'i':
						ai(people);
						break;
					case 'r':
						ar(rooms);
						break;
					case 'm':
						am(rooms);
						break;
					case 'p':
						ap(rooms, people);
						break;
					default:
						cout << "Unrecognized command!" << endl;
						cin.clear();
						skip_line();
						break;
				}
				break;
			case 'r':
				switch(item) {
					case 'm':
						rm(rooms);
						break;
					default:
						cout << "Unrecognized command!" << endl;
						cin.clear();
						skip_line();
						break;
				}
				break;
			case 'd':
				switch(item) {
					case 'i':
						di(rooms, people);
						break;
					case 'r':
						dr(rooms);
						break;
					case 'm':
						dm(rooms);
						break;
					case 'p':
						dp(rooms, people);
						break;
					case 's':
						ds(rooms);
						break;
					case 'g':
						dg(rooms, people);
						break;
					case 'a':
						da(rooms, people);
						break;
					default:
						cout << "Unrecognized command!" << endl;
						cin.clear();
						skip_line();
						break;
				}
				break;
			case 's':
				switch(item) {
					case 'd':
						sd(rooms, people);
						break;
					default:
						cout << "Unrecognized command!" << endl;
						cin.clear();
						skip_line();
						break;
				}
				break;
			case 'l':
				switch(item) {
					case 'd':
						ld(rooms, people);
						break;
					default:
						cout << "Unrecognized command!" << endl;
						cin.clear();
						skip_line();
						break;
				}
				break;
			case 'q':
				switch(item) {
					case 'q':
						da(rooms, people);
						printf("Done\n");
						return 0;
					default:
						break;
				}
				break;
			default:
				cout << "Unrecognized command!" << endl;
				cin.clear();
				skip_line();
				break;
		}
		} /* end of try block */
		catch (String_exception& x) {
			cout << x.msg << endl;
			cin.clear();
			skip_line();
		}
		catch (runtime_error& x) {
			cout << x.what() << endl;
			cin.clear();
			skip_line();
		}
		catch (...) {
			cout << "unknown error caught!" << endl;
			cin.clear();
			skip_line();
		}
	}
}

void skip_line()
{
	while (getchar() != '\n');
}

int read_room_number()
{
	assert(cin);
	int room_number;
	cin >> room_number;
	if (!cin) {
		cin.clear();
		throw runtime_error{"Could not read an integer value!"};
	}
	check_room_no_in_range(room_number);
	return room_number;
}

int read_time()
{
	assert(cin);
	int time;
	cin >> time;
	if (!cin) {
		cin.clear();
		throw runtime_error{"Could not read an integer value!"};
	}
	check_time_in_range(time);
	return time;
}

room_iterator_t read_no_and_get_room(room_list_t& rooms)
{
	return get_room(rooms, read_room_number());
}

// PI and subfunction definitions
void pi(people_list_t& people)
{
	String lastname;
	cin >> lastname;
	Person probe(lastname);
	auto person = people.find(&probe);
	if (person == people.end())
		throw runtime_error{"No person with that name!"};
	cout << **person << endl;
}

// PR and subfunction definitions
void pr(room_list_t& rooms)
{
	auto room = read_no_and_get_room(rooms);
	cout << *room;
}
/* 	checks validity of room number,
	if not, throws error.
	Otherwise, returns */
void check_room_no_in_range(int room_number)
{
	if (room_number < 0)
		throw runtime_error{"Room number is not in range!"};
}
/*	returns an iterator to the room given by room_number
	if no room of that number exists, throws the appropriate error */
room_iterator_t get_room(room_list_t& rooms, int room_number)
{
	Room probe(room_number);
	auto room = rooms.find(probe);
	if (room == rooms.end())
		throw runtime_error{"No room with that number!"};
	return room;
}

// PM and subfunction definitions
void pm(room_list_t& rooms)
{
	auto room = read_no_and_get_room(rooms);

	int time = read_time();
	Meeting& meeting = room->get_Meeting(time);
	cout << meeting;
}
/* 	checks if given time is in range
	throws error if time isn't in range
	otherwise returns */
void check_time_in_range(int time) {
	if ((time >= 1 && time <= 5) || (time >= 9 && time <= 12)) {
		return;
	}
	else {
		throw runtime_error{"Time is not in range!"};
	}
}

void ps(room_list_t& rooms)
{
	if (!rooms.size())
		cout << "List of rooms is empty" << endl;
	else
		cout << "Information for " << rooms.size() << " rooms:" << endl;
	for (Room& r : rooms) {
		cout << r;
	}
}

void pg(people_list_t& people)
{
	if (!people.size())
		cout << "List of people is empty" << endl;
	else
		cout << "Information for " << people.size() << " people:" << endl;
	for (const Person* p : people) {
		cout << *p << endl;
	}
}

// PA and subfunctino definitions
void pa(people_list_t& people, room_list_t& rooms)
{
 	cout <<"Memory allocations:" << endl;
 	cout <<"Strings: " << String::get_number() << " with " << String::get_total_allocation() << " bytes total" << endl;
 	cout <<"Persons: " << people.size() << endl;
 	cout <<"Meetings: " << num_meetings(rooms) << endl;
 	cout <<"Rooms: " << rooms.size() << endl;
	cout << "Lists: " << g_Ordered_list_count << endl;
 	cout <<"List Nodes: " << g_Ordered_list_Node_count << endl;
}
int num_meetings(room_list_t& rooms)
{
	int sum = 0;
	for (auto itr = rooms.begin(); itr != rooms.end(); ++itr)
		sum += itr->get_number_Meetings();
	return sum;
}

void ai(people_list_t& people)
{
	String firstname, lastname, phoneno;
	cin >> firstname >> lastname >> phoneno;
	const Person* new_person = new Person(firstname, lastname, phoneno);
	if (people.find(new_person) != people.end()) {
		delete new_person;
		throw runtime_error{"There is already a person with this last name!"};
	}
	people.insert(new_person);
	cout << "Person " << lastname << " added" << endl;
}

void ar(room_list_t& rooms)
{
	int room_number = read_room_number();
	try {
		// get room will throw an error if there isn't a room in the list
		// which is exactly what we want, therefore if we don't catch a
		// runtime_error here we know there is already a room in the list
		get_room(rooms, room_number);
	} catch(runtime_error e) {
		rooms.insert(Room(room_number));
		cout << "Room " << room_number << " added" << endl;
		return;
	}
	throw runtime_error{"There is already a room with this number!"};
}

void am(room_list_t& rooms)
{
	auto room = read_no_and_get_room(rooms);
	
	int time = read_time();

	String topic;
	cin >> topic;

	Meeting meeting(time, topic);
	room->add_Meeting(meeting);
	cout << "Meeting added at " << time << endl;
}

void ap(room_list_t& rooms, people_list_t& people)
{
	auto room = read_no_and_get_room(rooms);

	int time = read_time();
	Meeting& meeting = room->get_Meeting(time);

	String lastname;
	cin >> lastname;
	Person probe(lastname);
	auto person = people.find(&probe);
	if (person == people.end())
		throw runtime_error{"No person with that name!"};

	meeting.add_participant(*person);
	cout << "Participant " << lastname << " added" << endl;
}

void rm(room_list_t& rooms)
{
	// read and error check old_room_no
	int old_room_no = read_room_number();
	auto old_room = get_room(rooms, old_room_no);

	// read and error check old_time
	int old_time = read_time();
	Meeting old_meeting = old_room->get_Meeting(old_time);

	// read and error check new_room_no
	int new_room_no = read_room_number();
	auto new_room = get_room(rooms, new_room_no);

	// read and error check new_time
	int new_time = read_time();

	// remove
	old_room->remove_Meeting(old_time);
	// change
	old_meeting.set_time(new_time);
	// copy
	try {
	new_room->add_Meeting(old_meeting);
	} catch (runtime_error& e) {
		// there was already a meeting at that time
		// add the old meeting back into its old time, then rethrow
		old_meeting.set_time(old_time);
		old_room->add_Meeting(old_meeting);
		throw;
	}

	// now remove old room
	cout << "Meeting rescheduled to room " << new_room_no << " at " << new_time << endl;
}

void di(room_list_t& rooms, people_list_t& people)
{
	String lastname;
	cin >> lastname;

	Person probe(lastname);
	auto person = people.find(&probe);
	if (person == people.end())
		throw runtime_error{"No person with that name!"};

	// now need to determine if this person is in any meetings
	if (apply_if_arg(rooms.begin(), rooms.end(), is_participant_present_wrapper, *person))
		throw runtime_error{"This person is a participant in a meeting!"};

	delete *person;
	people.erase(person);
	cout << "Person " << lastname << " deleted" << endl;
}
bool is_participant_present_wrapper(Room room, const Person* p)
{
	return room.is_participant_present(p);
}

void dr(room_list_t& rooms)
{
	int room_number = read_room_number();
	auto room = get_room(rooms, room_number);

	room->clear_Meetings();
	rooms.erase(room);
	cout << "Room " << room_number << " deleted" << endl;
}

void dm(room_list_t& rooms)
{
	auto room = read_no_and_get_room(rooms);

	int time = read_time();
	room->remove_Meeting(time);
	cout << "Meeting at " << time << " deleted" << endl;
}

void dp(room_list_t& rooms, people_list_t& people)
{
	auto room = read_no_and_get_room(rooms);

	int time = read_time();

	Meeting& meeting = room->get_Meeting(time);

	String lastname;
	cin >> lastname;
	Person probe(lastname);
	auto person = people.find(&probe);
	if (person == people.end())
		throw runtime_error{"No person with that name!"};

	meeting.remove_participant(*person);
	cout << "Participant " << lastname << " deleted" << endl;
}

void ds(room_list_t& rooms)
{
	for (Room& r : rooms)
		r.clear_Meetings();
	cout << "All meetings deleted" << endl;
}

void dg(room_list_t& rooms, people_list_t& people)
{
	dg_no_messages(rooms, people);
	cout << "All persons deleted" << endl;
}

void dg_no_messages(room_list_t& rooms, people_list_t& people)
{
	for (Room& r : rooms) {
		if (r.has_Meetings())
			throw runtime_error{"Cannot clear people list unless there are no meetings!"};
	}
	for (auto itr = people.begin(); itr != people.end(); ) {
		delete *itr;
		people.erase(itr++);
	}
}

void da(room_list_t& rooms, people_list_t& people)
{
	rooms.clear();
	cout << "All rooms and meetings deleted" << endl;
	// need to pay special attention to people since the people
	// list only holds pointers to dynamically allocated objects
	dg(rooms, people);
}

void da_no_messages(room_list_t& rooms, people_list_t& people)
{
	rooms.clear();
	dg_no_messages(rooms, people);
}

void sd(room_list_t& rooms, people_list_t& people)
{
	String filename;
	cin >> filename;

	ofstream output_file(filename.c_str());
	if (!output_file.is_open())
		throw runtime_error{"Could not open file!"};

	// file format:
	// the number of people
	output_file << people.size() << endl;
	// print each person per line
	for (const Person* p : people) {
		output_file << *p << endl;
	}
	// number of rooms
	output_file << rooms.size() << endl;
	// each room
	for (Room& r : rooms) {
		r.save(output_file);
	}
	output_file.close();

	cout << "Data saved" << endl;
}

void ld(room_list_t& rooms, people_list_t& people)
{
	String filename;
	cin >> filename;

	ifstream input_file(filename.c_str());
	if (!input_file.is_open())
		throw runtime_error{"Could not open file!"};

	// clear all data
	room_list_t rooms_backup(rooms);
	people_list_t people_backup(people);

	people.clear();
	rooms.clear();

	try {
	// file format:
	// the number of people
	int max;
	input_file >> max;
	if (!input_file)
		throw runtime_error{"Invalid data found in file!"};

	for (int i = 0; i < max; ++i) {
		// read in each person
		const Person* new_person;
		try {
			new_person = new Person(input_file);
			people.insert(new_person);
		} catch (runtime_error& e) {
			delete new_person;
			throw;
		}
	}
	// read in number of rooms
	input_file >> max;
	if (!input_file)
		throw runtime_error{"Invalid data found in file!"};

	for (int i = 0; i < max; ++i) {
		// read in each room
		rooms.insert(Room(input_file, people));
	}
	// done!
	}
	catch(runtime_error& e) {
		cout << e.what() << endl;
		people = people_backup;
		rooms = rooms_backup;
		input_file.close();
		return;
	}
	da_no_messages(rooms_backup, people_backup);
	input_file.close();
	cout << "Data loaded" << endl;
}