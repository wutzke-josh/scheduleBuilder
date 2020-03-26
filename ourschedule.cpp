#include <iostream>
#include "schedule.h"
using namespace std;

int main(){
	schedule test;
	classInfo section1;

	test.insertRequired("math");
	test.insertRequired("science");

	section1 = {"eb01",11,12,true,"dragos",10};
	test.insert("math",section1);
	section1 = {"ebo2",9,10,false,"pescke",7};
	test.insert("math",section1);
	section1 = {"eb03",13,14,true,"powell",8};
	test.insert("math",section1);

	section1 = {"eb01", 11, 12, true, "mar",10};
	test.insert("science", section1);
	section1 = {"eb02", 9, 10, true, "saiyu",2};
	test.insert("science", section1);
	section1 = {"eb03", 9, 10, false, "wheelock",7};
	test.insert("science", section1);
	section1 = {"eb04", 1, 2, true, "queueing",7};
	test.insert("science", section1);

	bool itworked = test.makeSchedule();
	if (itworked) {
		test.display();
	} else {
		cout << "uhoh :(" << endl;
	}

	return 0;
}