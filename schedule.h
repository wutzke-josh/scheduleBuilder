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
		return rating > other.rating;
	}
};	

// struct course {
// 	string name;
// 	vector<classInfo> sections;
// 	bool operator== (const course& other){
// 		return name==other->name;
// 	}
// }

class schedule {
public:
  	void insertRequired(string required);
  	void insert(string name, classInfo myClass);
    // unordered_set<course> buildSchedule();
	void sortRating();
	void display();
	bool makeSchedule();
private:
	vector<string> courseReq;
	vector<string> addedClasses;

	unordered_map<string,vector<classInfo>> myCourses;
	unordered_map<string, classInfo> finalSchedule;

	bool checkConflict(unordered_map<string, classInfo> schedule, classInfo section);
	void swap(classInfo& c1,classInfo& c2);
	void quickSort(vector<classInfo>& thisCourse, int start, int n);
	int pivot(vector<classInfo>& thisCourse, int n, int pi,int start);

};

#endif