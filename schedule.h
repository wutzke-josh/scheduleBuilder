#ifndef _SCHEDULE_H_
#define _SCHEDULE_H_
#include <iostream>
#include <string>

using namespace std;

struct classInfo {
	string section;
	int start;
	int end;
	bool day;
}

struct course {
	string name;
	classInfo sections[];
	string professor;
	int rating;
	bool operator== (const course& other){
		return name==other.name;
	}
}

class schedule {
public:
	schedule(set<string> required);
  	~schedule();
  	void insert(course newclass);
  	unordered_set<course> buildSchedule();
	void sortRating(course myCourses[]);
private:
	course myCourses[];

}
#endif