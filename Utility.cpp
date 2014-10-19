#include "Utility.h"
#include "Person.h"
#include <algorithm>
using namespace std;

void p_person(const Person* p, ostream& os)
{
	os << *p << endl;
}

const int half_day = 12;
int normalize_time(int time)
{
	if (time >= 1 && time <= 5)
		return time + half_day;
	else
		return time;
}

/* this function returns an iterator to the person in the list whos last
   name is lexicographically larger than or equal to the given lastname
   otherwise returns iterator to the end of people */
people_itr_t get_Person_itr(people_list_t& people, string lastname)
{
	return find_if(people.begin(), people.end(), [lastname](const Person* s) { return s->get_lastname() >= lastname; });
}
