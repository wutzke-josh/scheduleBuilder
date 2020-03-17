#ifndef _SCHEDULE_H_
#define _SCHEDULE_H_
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

struct classInfo {
	string section;
	int start;
	int end;
	bool day;
	string professor;
	int rating;
	bool operator> (const classInfo& other){
		return rating > other->rating;
	}
}

// struct course {
// 	string name;
// 	vector<classInfo> sections;
// 	bool operator== (const course& other){
// 		return name==other->name;
// 	}
// }

class schedule {
public:
	schedule();
  	~schedule();
  	//void insertRequired(string required);
  	void insert(string name, classInfo myClass);
 //  unordered_set<course> buildSchedule();
	// void sortRating(course myCourses[]);
private:
	//vector<string> courseReq;
	vector<string> addedClasses;

	unordered_map<string,vector<classInfo>> myCourses;

}

#endif