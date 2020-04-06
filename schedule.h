// ============================
// Name: Josh Wutzke, Jordan Rusk and Sarah Hildt
// CMPUT 275, Winter 2020
// Final Project
// ============================
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
	double start;
	double end;
	int8_t day;
	string professor;
	int rating;
	int score = 0;
};	

struct classPreferences {
	bool morning = false;
	bool afternoon = false;
	bool prof = true;
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
    bool inReq(string className);
	void sortRating();
	void display();
	bool makeSchedule();
	void preferMorn(bool pref = true);
	void preferAft(bool pref = true);
	void preferProf(bool pref = true);
private:
	vector<string> courseReq;
	vector<string> addedClasses;

	classPreferences prefer;

	void checkPrefs();

	unordered_map<string,vector<classInfo>> myCourses;
	unordered_map<string, classInfo> finalSchedule;

	void computeScore();
	bool checkConflict(unordered_map<string, classInfo> schedule, classInfo section);
	void swap(classInfo& c1,classInfo& c2);
	void quickSort(vector<classInfo>& thisCourse, int start, int n);
	int pivot(vector<classInfo>& thisCourse, int n, int pi,int start);

};

#endif