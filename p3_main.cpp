#include "Person.h"
#include "Meeting.h"
#include "Room.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

// struct used to pass into every command function
struct container_t {
	people_list_t people;
	room_list_t rooms;
};

// types for the command map
using CommandFunction = void (*)(container_t&);
using command_map_t = map<string, CommandFunction>;

// MAIN HELPER FUNCTIONS

void skip_line();
int read_room_number();
int read_time();
const Person* get_Person_ptr(people_list_t&, string);
people_itr_t get_position_for_new_Person(people_list_t&, string);

void pi(container_t&);

void pr(container_t&);
void check_room_no_in_range(int);
Room& read_no_and_get_room(room_list_t&);
room_list_t::iterator get_room_itr(room_list_t&, int);
room_list_t::iterator get_position_for_new_Room(room_list_t&, Room&);
Room& get_room(room_list_t&, int);

void pm(container_t&);
void check_time_in_range(int);

void ps(container_t&);

void pg(container_t&);

void pa(container_t&);
int num_meetings(room_list_t&);

void ai(container_t&);

void ar(container_t&);

void am(container_t&);

void ap(container_t&);

void rm(container_t&);

void di(container_t&);
bool is_participant_present_wrapper(Room room, const Person* p);

void dr(container_t&);

void dm(container_t&);

void dp(container_t&);

void ds(container_t&);

void dg(container_t&);
void dg_no_messages(room_list_t&, people_list_t&);

void da(container_t&);
void da_no_messages(room_list_t&, people_list_t&);

void sd(container_t&);

void set_container_and_close_file(container_t& container, ofstream&, people_list_t&, room_list_t&);
void set_container_and_close_file(container_t& container, ifstream&, people_list_t&, room_list_t&);
void set_backups(container_t&, people_list_t&, room_list_t&);
void ld(container_t&);

int main() 
{

	container_t container;
	string command;
	command_map_t command_map;

	// populate the command map
	command_map["pi"] = &pi;
	command_map["pr"] = &pr;
	command_map["pm"] = &pm;
	command_map["ps"] = &ps;
	command_map["pg"] = &pg;
	command_map["pa"] = &pa;
	command_map["ai"] = &ai;
	command_map["ar"] = &ar;
	command_map["am"] = &am;
	command_map["ap"] = &ap;
	command_map["rm"] = &rm;
	command_map["di"] = &di;
	command_map["dr"] = &dr;
	command_map["dm"] = &dm;
	command_map["dp"] = &dp;
	command_map["ds"] = &ds;
	command_map["dg"] = &dg;
	command_map["da"] = &da;
	command_map["sd"] = &sd;
	command_map["ld"] = &ld;

	while (true) {
		try {
		/* main program loop */
		cout << "\nEnter command: ";
		cin.width(2);
		cin >> command;
		if (command == "qq") {
			// quit
			da(container);
			cout << "Done" << endl;
			return 0;
		}
		// test for validity of command and run, otherwise
		// cout an error, clear cin and skip the line
		auto cur_command = command_map.find(command);
		if (cur_command != command_map.end()) {
			auto func = cur_command->second;
			func(container);
		} else {
			cout << "Unrecognized command!" << endl;
			cin.clear();
			skip_line();
		}
		} /* end of try block */
		catch (Error& e) {
			cout << e.msg << endl;
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
	// removing for now
	// assert(cin);
	int room_number;
	cin >> room_number;
	if (!cin) {
		cin.clear();
		throw Error("Could not read an integer value!");
	}
	check_room_no_in_range(room_number);
	return room_number;
}

int read_time()
{
	// removing for now
	// assert(cin);
	int time;
	cin >> time;
	if (!cin) {
		cin.clear();
		throw Error("Could not read an integer value!");
	}
	check_time_in_range(time);
	return time;
}

// if get_Person_itr returns the correct person (as determined by lastname)
// returns a pointer to that person, otherwise throws no person with lastname
// error
const Person* get_Person_ptr(people_list_t& people, string lastname)
{
	people_itr_t person_itr = get_Person_itr(people, lastname);
	if (person_itr == people.end() || (*person_itr)->get_lastname() != lastname) {
		throw Error("No person with that name!");
	}
	return *person_itr;
}

// returns an iterator to the position where a new person with supplied lastname
// would go. throws an error if a person with that lastname already exists
people_itr_t get_position_for_new_Person(people_list_t& people, string lastname)
{
	people_itr_t position = get_Person_itr(people, lastname);
	if (position != people.end() && (*position)->get_lastname() == lastname) {
		throw Error("There is already a person with this last name!");
	}
	return position;
}

// PI and subfunction definitions
void pi(container_t& container)
{
	string lastname;
	cin >> lastname;
	auto person = get_Person_ptr(container.people, lastname);
	cout << *person << endl;
}

// PR and subfunction definitions
void pr(container_t& container)
{
	Room room = read_no_and_get_room(container.rooms);
	cout << room;
}
/* 	throws error if room number is invalid */
void check_room_no_in_range(int room_number)
{
	if (room_number < 0)
		throw Error("Room number is not in range!");
}

Room& read_no_and_get_room(room_list_t& rooms)
{
	return get_room(rooms, read_room_number());
}

/*	returns an iterator to the room given by room_number
	if no room of that number exists, throws the appropriate error */
room_list_t::iterator get_room_itr(room_list_t& rooms, int room_number)
{
	Room probe(room_number);
	auto room_itr = get_position_for_new_Room(rooms, probe);
	if (*room_itr == probe) {
		// found a match
		return room_itr;
	}
	throw Error("No room with that number!");
}

room_list_t::iterator get_position_for_new_Room(room_list_t& rooms, Room& probe)
{
	return lower_bound(rooms.begin(), rooms.end(), probe);
}


Room& get_room(room_list_t& rooms, int room_number)
{
	auto room_itr = get_room_itr(rooms, room_number);
	return *room_itr;
}

// PM and subfunction definitions
void pm(container_t& container)
{
	Room room = read_no_and_get_room(container.rooms);
	cout << room.get_Meeting(read_time());
}
/* 	checks if given time is in range
	throws error if time isn't in range
	otherwise returns */
void check_time_in_range(int time) {
	if ((time >= 1 && time <= 5) || (time >= 9 && time <= 12)) {
		return;
	}
	else {
		throw Error("Time is not in range!");
	}
}

void ps(container_t& container)
{
	if (container.rooms.empty())
		cout << "List of rooms is empty" << endl;
	else
		cout << "Information for " << container.rooms.size() << " rooms:" << endl;
	for_each(container.rooms.begin(), container.rooms.end(), [](const Room r) { cout << r; });
}

void pg(container_t& container)
{
	list<const Person*> people = container.people;
	if (people.empty())
		cout << "List of people is empty" << endl;
	else
		cout << "Information for " << people.size() << " people:" << endl;
	for_each(people.begin(), people.end(), [](const Person* p){ cout << *p << endl;});
}

// PA and subfunctino definitions
void pa(container_t& container)
{
 	cout << "Memory allocations:" << endl;
 	cout << "Persons: " << container.people.size() << endl;
 	cout << "Meetings: " << num_meetings(container.rooms) << endl;
 	cout << "Rooms: " << container.rooms.size() << endl;
}
int num_meetings(room_list_t& rooms)
{
	int sum = 0;
	for_each(rooms.begin(), rooms.end(), [&sum](const Room r){ sum += r.get_number_Meetings(); });
	return sum;
}

void ai(container_t& container)
{
	string firstname, lastname, phoneno;
	cin >> firstname >> lastname >> phoneno;
	Person* new_person;
	people_itr_t position;
	try {
		new_person = new Person(firstname, lastname, phoneno);
		position = get_position_for_new_Person(container.people, lastname);
	} catch(Error& e) {
		delete new_person;
		throw;
	} catch(bad_alloc& e) {
		cerr << "Not enough memory!" << endl;
		exit(1);
	}
	container.people.insert(position, new_person);
	cout << "Person " << lastname << " added" << endl;
}

void ar(container_t& container)
{
	int room_number = read_room_number();
	Room probe(room_number);
	// test if the item is already there
	if (binary_search(container.rooms.begin(), container.rooms.end(), probe)) {
		throw Error("There is already a room with this number!");
	}
	auto insert = lower_bound(container.rooms.begin(), container.rooms.end(), probe);
	container.rooms.insert(insert, probe);
	cout << "Room " << room_number << " added" << endl;
}

void am(container_t& container)
{
	Room& room = read_no_and_get_room(container.rooms);
	
	int time = read_time();

	string topic;
	cin >> topic;

	Meeting meeting(time, topic);
	room.add_Meeting(meeting);
	cout << "Meeting added at " << time << endl;
}

void ap(container_t& container)
{
	Room& room = read_no_and_get_room(container.rooms);

	int time = read_time();
	Meeting meeting = room.get_Meeting(time);

	string lastname;
	cin >> lastname;

	const Person* person = get_Person_ptr(container.people, lastname);

	meeting.add_participant(person);
	// TODO: fix this shite
	room.remove_Meeting(time);
	room.add_Meeting(meeting);

	cout << "Participant " << lastname << " added" << endl;
}

void rm(container_t& container)
{
	// read and error check old_room_no
	int old_room_no = read_room_number();
	Room& old_room = get_room(container.rooms, old_room_no);

	// read and error check old_time
	int old_time = read_time();
	Meeting old_meeting = old_room.get_Meeting(old_time);

	// read and error check new_room_no
	int new_room_no = read_room_number();
	Room& new_room = get_room(container.rooms, new_room_no);

	// read and error check new_time
	int new_time = read_time();

	// check that the new room and time is the same as old room and time
	if ((new_time == old_time) && (new_room_no == old_room_no)) {
		cout << "No change made to schedule" << endl;
		return;
	}

	// remove
	old_room.remove_Meeting(old_time);
	// change
	old_meeting.set_time(new_time);
	// copy
	try {
	new_room.add_Meeting(old_meeting);
	} catch (Error& e) {
		// there was already a meeting at that time
		// add the old meeting back into its old time, then rethrow
		old_meeting.set_time(old_time);
		old_room.add_Meeting(old_meeting);
		throw;
	}

	// now remove old room
	cout << "Meeting rescheduled to room " << new_room_no << " at " << new_time << endl;
}

void di(container_t& container)
{
	string lastname;
	cin >> lastname;

	people_itr_t person = get_Person_itr(container.people, lastname);

	// now need to determine if this person is in any meetings
	// TODO: use commitments maw fucka
	if (any_of(container.rooms.begin(), container.rooms.end(), [person](const Room& r){ return r.is_participant_present(*person); }))
		throw Error("This person is a participant in a meeting!");

	delete *person;
	container.people.erase(person);
	cout << "Person " << lastname << " deleted" << endl;
}
bool is_participant_present_wrapper(Room room, const Person* p)
{
	return room.is_participant_present(p);
}

void dr(container_t& container)
{
	int room_number = read_room_number();
	room_list_t::iterator room = get_room_itr(container.rooms, room_number);

	room->clear_Meetings();
	container.rooms.erase(room);
	cout << "Room " << room_number << " deleted" << endl;
}

void dm(container_t& container)
{
	Room& room = read_no_and_get_room(container.rooms);

	int time = read_time();
	room.remove_Meeting(time);
	cout << "Meeting at " << time << " deleted" << endl;
}

void dp(container_t& container)
{
	Room& room = read_no_and_get_room(container.rooms);

	int time = read_time();

	Meeting meeting = room.get_Meeting(time);

	string lastname;
	cin >> lastname;
	const Person* person = get_Person_ptr(container.people, lastname);

	meeting.remove_participant(person);
	// TODO: FIX THIS SHITE
	room.remove_Meeting(time);
	room.add_Meeting(meeting);
	cout << "Participant " << lastname << " deleted" << endl;
}

void ds(container_t& container)
{
	for_each(container.rooms.begin(), container.rooms.end(), [](Room& r){ r.clear_Meetings(); });
	cout << "All meetings deleted" << endl;
}

void dg(container_t& container)
{
	dg_no_messages(container.rooms, container.people);
	cout << "All persons deleted" << endl;
}

void dg_no_messages(room_list_t& rooms, people_list_t& people)
{
	// the one range for
	for (Room& r : rooms) {
		if (r.has_Meetings())
			throw Error("Cannot clear people list unless there are no meetings!");
	}
	// first delete all pointed to data
	for_each(people.begin(), people.end(), [](const Person* p){ delete p; });
	people.clear();
}

void da(container_t& container)
{
	container.rooms.clear();
	cout << "All rooms and meetings deleted" << endl;
	// need to pay special attention to people since the people
	// list only holds pointers to dynamically allocated objects
	dg(container);
}

void da_no_messages(room_list_t& rooms, people_list_t& people)
{
	rooms.clear();
	dg_no_messages(rooms, people);
}

void sd(container_t& container)
{
	string filename;
	cin >> filename;

	ofstream output_file(filename);
	if (!output_file.is_open())
		throw Error("Could not open file!");

	// file format:
	// the number of people
	output_file << container.people.size() << endl;
	// print each person per line
	for_each(container.people.begin(), container.people.end(), [&output_file](const Person* p){ output_file << *p << endl;});

	// number of rooms
	output_file << container.rooms.size() << endl;
	// each room
	for_each(container.rooms.begin(), container.rooms.end(), [&output_file](Room& r){ r.save(output_file); });

	output_file.close();

	cout << "Data saved" << endl;
}

void set_container_and_close_file(container_t& container, ifstream& file, people_list_t& people_backup, room_list_t& rooms_backup)
{
	set_backups(container, people_backup, rooms_backup);
	file.close();
}

void set_container_and_close_file(container_t& container, ofstream& file, people_list_t& people_backup, room_list_t& rooms_backup)
{
	set_backups(container, people_backup, rooms_backup);
	file.close();
}

void set_backups(container_t& container, people_list_t& people_backup, room_list_t& rooms_backup)
{
	container.people.swap(people_backup);
	container.rooms.swap(rooms_backup);
	people_backup.clear();
	rooms_backup.clear();
}

void ld(container_t& container)
{
	// loops are allowed in here yaaaaaaaay
	string filename;
	cin >> filename;

	ifstream input_file(filename);
	if (!input_file.is_open())
		throw Error("Could not open file!");

	// clear all data
	room_list_t rooms_backup(container.rooms);
	people_list_t people_backup(container.people);

	container.people.clear();
	container.rooms.clear();

	try {
	// file format:
	// the number of people
	int max;
	input_file >> max;
	if (!input_file)
		throw Error("Invalid data found in file!");

	for (int i = 0; i < max; ++i) {
		// read in each person
		const Person* new_person;
		try {
			new_person = new Person(input_file);
			auto position = get_position_for_new_Person(container.people, new_person->get_lastname());
			container.people.insert(position, new_person);
		} catch (Error& e) {
			// need to delete the new person
			delete new_person;
			throw Error("Invalid data found in file!");
		}
	}
	// read in number of rooms
	input_file >> max;
	if (!input_file)
		throw Error("Invalid data found in file!");

	for (int i = 0; i < max; ++i) {
		// read in each room
		Room new_room(input_file, container.people);
		room_list_t::iterator position = get_position_for_new_Room(container.rooms, new_room);
		container.rooms.insert(position, new_room);
	}
	// done!
	}
	catch(Error& e) {
		da_no_messages(container.rooms, container.people);
		set_container_and_close_file(container, input_file, people_backup, rooms_backup);
		throw;
	}
	da_no_messages(rooms_backup, people_backup);
	input_file.close();
	cout << "Data loaded" << endl;
}
