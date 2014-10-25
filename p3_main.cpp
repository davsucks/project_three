#include "Person.h"
#include "Meeting.h"
#include "Room.h"
#include <algorithm>
using namespace std;

// struct used to pass into every command function
struct Container_t {
	People_list_t people;
	Room_list_t rooms;
};


// types for the command map
using CommandFunction = void (*)(Container_t&);
using command_map_t = map<string, CommandFunction>;

// MAIN HELPER FUNCTIONS

// General Helpers used by command functions
void skip_line();
int read_int();
int read_room_number();
int read_time();
// Person helpers
Person* get_Person_ptr(People_list_t&, string);
People_list_t::iterator get_position_for_new_Person(People_list_t&, string);
// Room helpers
Room& read_no_and_get_room(Room_list_t&);
Room_list_t::iterator get_room_itr(Room_list_t&, int);
Room& get_room(Room_list_t&, int);

void set_container_and_close_file(Container_t& container, ifstream&, People_list_t&, Room_list_t&);
void set_backups(Container_t&, People_list_t&, Room_list_t&);

void pi(Container_t&);
void pr(Container_t&);
void pm(Container_t&);
void ps(Container_t&);
void pg(Container_t&);
void pa(Container_t&);
void pc(Container_t&);
void ai(Container_t&);
void ar(Container_t&);
void am(Container_t&);
void ap(Container_t&);
void rm(Container_t&);
void di(Container_t&);
void dr(Container_t&);
void dm(Container_t&);
void dp(Container_t&);
void ds(Container_t&);
void dg(Container_t&);
void dg_no_messages(Room_list_t&, People_list_t&);

void da(Container_t&);
void da_no_messages(Room_list_t&, People_list_t&);

void sd(Container_t&);
void ld(Container_t&);

int main() 
{

	Container_t container;
	char action;
	char item;
	string command;
	command_map_t command_map;

	// populate the command map
	command_map["pi"] = &pi;
	command_map["pr"] = &pr;
	command_map["pm"] = &pm;
	command_map["ps"] = &ps;
	command_map["pg"] = &pg;
	command_map["pa"] = &pa;
	command_map["pc"] = &pc;
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
		cin >> action;
		cin >> item;
		command.clear();
		command += action;
		command += item;
		if (command == "qq") {
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

// GENERAL HELPER DEFINITIONS
// reads from cin until a newline is read
void skip_line()
{
	while (getchar() != '\n');
}
/* 	throws error if room number is invalid */
void check_room_no_in_range(int room_number)
{
	if (room_number < 0)
		throw Error("Room number is not in range!");
}
// reads an int from cin and checks if its in range for a room number
int read_room_number()
{
	int room_number = read_int();
	check_room_no_in_range(room_number);
	return room_number;
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
// reads an int from cin and checks if the time is in range
int read_time()
{
	int time = read_int();
	check_time_in_range(time);
	return time;
}
// reads an int from cin and throws an error if it failed
int read_int()
{
	int temp;
	cin >> temp;
	if (!cin) {
		cin.clear();
		throw Error("Could not read an integer value!");
	}
	return temp;
}

/* this function returns an iterator to the person in the list whos last
   name is lexicographically larger than or equal to the given lastname
   otherwise returns iterator to the end of people */
People_list_t::iterator get_Person_itr(People_list_t& people, string lastname)
{
	return find_if_not(people.begin(), people.end(), [lastname](const Person* s) { return s->get_lastname() < lastname; });
}

// if get_Person_itr returns the correct person (as determined by lastname)
// returns a pointer to that person, otherwise throws no person with lastname
// error
Person* get_Person_ptr(People_list_t& people, string lastname)
{
	auto person_itr = get_Person_itr(people, lastname);
	if (person_itr == people.end() || (*person_itr)->get_lastname() != lastname)
		throw Error("No person with that name!");

	return *person_itr;
}

// returns an iterator to the position where a new person with supplied lastname
// would go. throws an error if a person with that lastname already exists
People_list_t::iterator get_position_for_new_Person(People_list_t& people, string lastname)
{
	auto position = get_Person_itr(people, lastname);
	if (position != people.end() && (*position)->get_lastname() == lastname) {
		throw Error("There is already a person with this last name!");
	}
	return position;
}

// PI and subfunction definitions
void pi(Container_t& container)
{
	string lastname;
	cin >> lastname;
	auto person_ptr = get_Person_ptr(container.people, lastname);
	cout << *person_ptr << endl;
}

// PR and subfunction definitions
void pr(Container_t& container)
{
	Room& room = read_no_and_get_room(container.rooms);
	cout << room;
}

Room& read_no_and_get_room(Room_list_t& rooms)
{
	return get_room(rooms, read_room_number());
}

/*	returns an iterator to the room given by room_number
	if no room of that number exists, throws the appropriate error */
Room_list_t::iterator get_room_itr(Room_list_t& rooms, int room_number)
{
	Room probe(room_number);
	auto room_itr = lower_bound(rooms.begin(), rooms.end(), probe);
	if (room_itr != rooms.end() && *room_itr == probe) {
		// found a match
		return room_itr;
	}
	throw Error("No room with that number!");
}


Room& get_room(Room_list_t& rooms, int room_number)
{
	auto room_itr = get_room_itr(rooms, room_number);
	return *room_itr;
}

// PM and subfunction definitions
void pm(Container_t& container)
{
	Room& room = read_no_and_get_room(container.rooms);
	const Meeting* meeting_ptr = room.get_Meeting(read_time());
	cout << *meeting_ptr;
}


void ps(Container_t& container)
{
	if (container.rooms.empty())
		cout << "List of rooms is empty" << endl;
	else
		cout << "Information for " << container.rooms.size() << " rooms:" << endl;
	for_each(container.rooms.begin(), container.rooms.end(), [](const Room& r) { cout << r; });
}

void pg(Container_t& container)
{
	if (container.people.empty())
		cout << "List of people is empty" << endl;
	else
		cout << "Information for " << container.people.size() << " people:" << endl;
	for_each(container.people.begin(), container.people.end(), [](const Person* p){ cout << *p << endl;});
}


// function object used by the pa command
class Meeting_calculator_t {
public:
	// this function sets the sum member variable
	void operator()(Room_list_t& rooms)
	{
		sum = 0;
		for_each(rooms.begin(), rooms.end(), [this](const Room& r){ this->sum += r.get_number_Meetings(); });
	}
	int get_sum()
	{
		return sum;
	}
private:
	int sum;
};

// PA and subfunctino definitions
void pa(Container_t& container)
{
	Meeting_calculator_t num_meetings;
	num_meetings(container.rooms);
 	cout << "Memory allocations:" << endl;
 	cout << "Persons: " << container.people.size() << endl;
 	cout << "Meetings: " << num_meetings.get_sum() << endl;
 	cout << "Rooms: " << container.rooms.size() << endl;
}

void pc(Container_t& container)
{
	string lastname;
	cin >> lastname;

	Person* person_ptr = get_Person_ptr(container.people, lastname);
	person_ptr->print_Commitments();
}

void ai(Container_t& container)
{
	string firstname, lastname, phoneno;
	cin >> firstname >> lastname >> phoneno;
	Person* new_person_ptr;
	People_list_t::iterator position;
	try {
		new_person_ptr = new Person(firstname, lastname, phoneno);
		position = get_position_for_new_Person(container.people, lastname);
	} catch(Error& e) {
		delete new_person_ptr;
		throw;
	} catch(bad_alloc& e) {
		print_message_and_quit();
	}
	container.people.insert(position, new_person_ptr);
	cout << "Person " << lastname << " added" << endl;
}

void ar(Container_t& container)
{
	int room_number = read_room_number();
	Room probe(room_number);

	auto new_room_itr = lower_bound(container.rooms.begin(), container.rooms.end(), probe);

	if (new_room_itr == container.rooms.end() || *new_room_itr != probe) {
		container.rooms.insert(new_room_itr, probe);
		cout << "Room " << room_number << " added" << endl;
		return;
	}
	throw Error("There is already a room with this number!");
}

void am(Container_t& container)
{
	Room& room = read_no_and_get_room(container.rooms);
	
	int time = read_time();

	string topic;
	cin >> topic;

	Meeting* meeting_ptr;
	try {
		meeting_ptr = new Meeting(time, topic);
		room.add_Meeting(meeting_ptr);
	} catch (Error& e) {
		delete meeting_ptr;
		throw;
	} catch (bad_alloc& e) {
		print_message_and_quit();
	}
	cout << "Meeting added at " << time << endl;
}

void ap(Container_t& container)
{
	Room& room = read_no_and_get_room(container.rooms);

	int time = read_time();
	Meeting* meeting_ptr = room.get_Meeting(time);

	string lastname;
	cin >> lastname;

	Person* person_ptr = get_Person_ptr(container.people, lastname);

	meeting_ptr->add_participant(person_ptr, room.get_room_number());

	cout << "Participant " << lastname << " added" << endl;
}

void rm(Container_t& container)
{
	// read and error check old_room_no
	int old_room_no = read_room_number();
	Room& old_room = get_room(container.rooms, old_room_no);

	// read and error check old_time
	int old_time = read_time();
	Meeting* old_meeting_ptr = old_room.get_Meeting(old_time);

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

	// check that the new time is available for the meeting in the new room
	if (new_room.is_Meeting_present(new_time))
		throw Error("There is already a meeting at that time!");
	// check whether a change in meeting time causes commitment conflict for a participant
	if (new_time != old_time && old_meeting_ptr->any_participants_committed(new_time))
		throw Error("A participant is already committed at the new time!");

	old_meeting_ptr->set_time(new_time);
	new_room.add_Meeting(old_meeting_ptr);
	// adding was successful, so now remove the old meeting
	old_room.remove_Meeting(old_time);
	old_meeting_ptr->update_Commitments(old_time, new_room_no, new_time);

	// now remove old room
	cout << "Meeting rescheduled to room " << new_room_no << " at " << new_time << endl;
}

void di(Container_t& container)
{
	string lastname;
	cin >> lastname;

	// fetch and error check the person
	Person* person_ptr = get_Person_ptr(container.people, lastname);

	// now need to determine if this person is in any meetings
	// use commitments so that room isn't responsible for its participants
	if (person_ptr->has_Commitments())
		throw Error("This person is a participant in a meeting!");

	// fetch an iterator to make deleting easier
	auto person_itr = get_Person_itr(container.people, lastname);
	delete person_ptr;
	container.people.erase(person_itr);
	cout << "Person " << lastname << " deleted" << endl;
}

void dr(Container_t& container)
{
	int room_number = read_room_number();
	// get_room_itr will throw if the room doesn't exist
	auto room_itr = get_room_itr(container.rooms, room_number);
	// call clear_meetings to ensure that meeting's (and commitments)
	// are properly destroyed
	room_itr->clear_Meetings();
	container.rooms.erase(room_itr);
	cout << "Room " << room_number << " deleted" << endl;
}

void dm(Container_t& container)
{
	Room& room = read_no_and_get_room(container.rooms);

	int time = read_time();
	const Meeting* meeting_ptr = room.get_Meeting(time);
	room.remove_Meeting(time);
	// this will trigger meetings destructor, which will deallcoate all 
	// commitments associated with it
	delete meeting_ptr;
	cout << "Meeting at " << time << " deleted" << endl;
}

void dp(Container_t& container)
{
	Room& room = read_no_and_get_room(container.rooms);

	int time = read_time();

	Meeting* meeting_ptr = room.get_Meeting(time);

	string lastname;
	cin >> lastname;
	Person* person = get_Person_ptr(container.people, lastname);

	meeting_ptr->remove_participant(person);

	cout << "Participant " << lastname << " deleted" << endl;
}

void ds(Container_t& container)
{
	auto clear = mem_fn(&Room::clear_Meetings);
	for_each(container.rooms.begin(), container.rooms.end(), [&clear](Room& r){ clear(r); });
	cout << "All meetings deleted" << endl;
}

void dg(Container_t& container)
{
	dg_no_messages(container.rooms, container.people);
	cout << "All persons deleted" << endl;
}

void dg_no_messages(Room_list_t& rooms, People_list_t& people)
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

void da(Container_t& container)
{
	// need to delete all meetings from each room
	for_each(container.rooms.begin(), container.rooms.end(), [](Room& r){ r.clear_Meetings(); });
	container.rooms.clear();
	cout << "All rooms and meetings deleted" << endl;
	// need to pay special attention to people since the people
	// list only holds pointers to dynamically allocated objects
	dg(container);
}

void da_no_messages(Room_list_t& rooms, People_list_t& people)
{
	for_each(rooms.begin(), rooms.end(), [](Room& r){ r.clear_Meetings(); });
	rooms.clear();
	dg_no_messages(rooms, people);
}

void sd(Container_t& container)
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

void set_container_and_close_file(Container_t& container, ifstream& file, People_list_t& people_backup, Room_list_t& rooms_backup)
{
	set_backups(container, people_backup, rooms_backup);
	file.close();
}

void set_backups(Container_t& container, People_list_t& people_backup, Room_list_t& rooms_backup)
{
	container.people.swap(people_backup);
	container.rooms.swap(rooms_backup);
	// clear meetings from old rooms
	for_each(rooms_backup.begin(), rooms_backup.end(), [](Room& r){ r.clear_Meetings(); });
	rooms_backup.clear();
	// delete all allocated people
	for_each(people_backup.begin(), people_backup.end(), [](Person* p){ delete p; });
	people_backup.clear();

}

void ld(Container_t& container)
{
	string filename;
	cin >> filename;

	ifstream input_file(filename);
	if (!input_file.is_open())
		throw Error("Could not open file!");

	// create new lists to populate
	// that way if loading succeeds we just swap these lists
	// with the container lists
	// otherwise, just clear these lists and move on
	Room_list_t new_room_list;
	People_list_t new_people_list;

	try {
	// first read in the max number of people
	int max;
	input_file >> max;
	if (!input_file)
		throw Error("Invalid data found in file!");

	for (int i = 0; i < max; ++i) {
		// read in each person
		Person* new_person_ptr;
		try {
		new_person_ptr = new Person(input_file);
		// use push back since the person list will be in alphabetical order
		new_people_list.push_back(new_person_ptr);
		} catch (bad_alloc& e) {
			print_message_and_quit();
		}
	}
	// read in number of rooms
	input_file >> max;
	if (!input_file)
		throw Error("Invalid data found in file!");

	for (int i = 0; i < max; ++i) {
		// read in each room
		Room new_room(input_file, new_people_list);
		// use push back since rooms will be ordered by room number
		new_room_list.push_back(new_room);
	}
	// done!
	} catch(Error& e) {
		da_no_messages(new_room_list, new_people_list);
		input_file.close();
		throw;
	}
	set_container_and_close_file(container, input_file, new_people_list, new_room_list);
	cout << "Data loaded" << endl;
}
