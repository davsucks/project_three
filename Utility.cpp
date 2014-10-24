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

void print_message_and_quit()
{
	cerr << "Ran out of memory!" << endl;
	exit(1);
}
