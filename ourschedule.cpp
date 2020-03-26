#include <iostream>
#include "schedule.h"
using namespace std;

int main(){
	schedule test;
	classInfo section1;
	section1 = {"eb01",11,12,true,"dragos",10};
	test.insert("math",section1);
	classInfo section2 = {"ebo2",9,10,false,"pescke",7};
	test.insert("math",section2);
	classInfo section3 = {"eb03",13,14,true,"powell",8};
	test.insert("math",section3);
	test.display();
	test.sortRating();
	test.display();


	return 0;
}