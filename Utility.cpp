#include "Utility.h"
#include "Person.h"
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
