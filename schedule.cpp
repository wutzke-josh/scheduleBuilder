#include "schedule.h"

using namespace std;

// void schedule::insertRequired(string required){
// 	courseReq.push_back(required);
// }

void schedule::insert(string name, classInfo myClass){
	myCourses[name].push_back(myClass);
} 

