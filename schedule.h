#ifndef _SCHEDULE_H_
#define _SCHEDULE_H_
#include <iostream>
#include <string>

using namespace std;


struct course {
	string name;
	string section;
	int start;
	int end;
	bool day; // true if M W F
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