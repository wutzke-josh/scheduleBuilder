// ============================
// Name: Josh Wutzke, Jordan Rusk and Sarah Hildt
// CMPUT 275, Winter 2020
// Final Project
// ============================
#include "schedule.h"
#include <iomanip>
#include <algorithm>

using namespace std;

void schedule::insert(string name, classInfo myClass){
	/*
	Inserts a class into the myCourses vector
	Inputs: Name of class (string), classInfo myClass
	Outputs: None
	*/
	myCourses[name].push_back(myClass);
}


bool schedule::inReq(string className){
	/*
	Check if a class is in the required classes
	Inputs: className (string)
	Outputs: bool (if it is in there)
	*/
	bool found = false;
	for(auto x: courseReq){
		// if the class is in the course requirements, return true
		if(x==className){
			found = true;
		}
	}
	return found;
}


void schedule::display(){
	/*
	prints the schedule to the user
	Inputs: None
	Outputs: None
	*/
	vector<string> week = {"Sundays","Mondays","Tuesdays","Wednesdays","Thursdays","Fridays","Saturdays"};
	// iterates through all courses in the final scheduleinRe
	for(auto x: finalSchedule){
		// displays course name
		cout << x.first << ": " << endl;
		// displayes section, professor, and rating
		cout << "Section " << x.second.section << " with " << x.second.professor;
		cout << " (rating = " << x.second.rating << ")" << endl;
		cout << "on ";
		// iterates through the day integer. For each position where the bit is a one,
		// the corresponding day from the week vector is chosen
		for(int i=6;i>=0;i--){
			if((x.second.day >> i) & 1){
				cout << week[6-i] << " ";
			}
		}
		// the hour is separated from the minute to simplify display	
		int startHour = x.second.start;
		// the decimal representing the fraction of the hour is converted to minutes
		int startMin = (x.second.start-startHour)*60.0;
		int endHour = x.second.end;
		int endMin = (x.second.end-endHour)*60.0;
		// the time is displayed. If the class begins or ends directly on the hour, the width is set
		// so the colon will be followed by two 0's
		cout << "from " << startHour << ":" <<setw(2) << setfill('0') << right << startMin << " to " << endHour << ":";
		cout <<setw(2) << setfill('0') << right << endMin << endl;
		cout << endl;
	}
}


void schedule::swap(classInfo& c1, classInfo& c2) {
	/*
	Swaps two classInfo elements
	Inputs: classInfo c1 and c2
	Outputs: None
	*/
	classInfo tmp = c1;
	c1 = c2;
	c2 = tmp;
} 


int schedule::pivot(vector<classInfo>& thisCourse, int n, int pi,int start){
	/*
	Pivot function creates new pivot for use in quick sort. It was adapted
	from the pseudocode available on eClass
	Inputs: the vector of classInfo's, length of section, current pivot, 
			starting point of vector
	Outputs: new pivot (int)
	*/
	swap(thisCourse[pi],thisCourse[start+n-1]);
	// start is the beginning of the section in the vector we are
	// looking at, and all indexing must take this into account
	int lo = start;
	int hi = start+n-2;
	while(lo<=hi){
		if(thisCourse[lo].score>=thisCourse[start+n-1].score){
			lo++;
		}else if(thisCourse[hi].score<thisCourse[start+n-1].score){
			hi --;
		}else{
			swap(thisCourse[lo],thisCourse[hi]);
		}
	}

	swap(thisCourse[lo],thisCourse[start+n-1]);
	// returns new pivot
	return lo;

}


void schedule::quickSort(vector<classInfo>& thisCourse, int start, int n){
	/*
	The quickSort algorithm presented in class
	Inputs: a vector of all classInfo, the start position and the size
	Outputs: None
	*/

	if(n<=1){
		return;
	}else{
		int pi = start+(n/2);
		int new_pi = pivot(thisCourse,n,pi,start);
		// sorts first half recursively
		quickSort(thisCourse,start,new_pi-start);
		// sorts second half recursively
		quickSort(thisCourse,new_pi+1,n-(new_pi-start)-1);
	}
}


void schedule::sortRating(){
	/*
	Sorts all of the sections by each class rating
	Inputs: None
	Outputs: None
	*/
	// sets the beginning of the iterator and goes until all classes
	auto x = myCourses.begin();
	// for each class, the sections are sorted by rating
	while (x != myCourses.end()) {
		quickSort(x->second,0,(x->second).size());
		x++;
	}
}


bool schedule::checkConflict(unordered_map<string, classInfo> schedule, classInfo section) {
	/* 
	Checks to see if there is a scheduling conflict between two sections
	Inputs: an unordered map representing the current schedule, and an object of type class Info
	Outputs: a boolean value (whether there is conflict or not)
	*/
	// for every course in the schedule
	for(auto x: schedule) {
		// check to see if the current course and the given parameter 'section' are on the same days
		// this will return true even if one course is a lab on mondays while another is a course
		// on mondays, wednesdays, and fridays
		if ((x.second.day & section.day) != 0) {
			// if the end of one course is after or equal to the start of the other
			// or vice versa, then there is a scheduling conflict
			if (!((x.second.end <= section.start) || (section.end <= x.second.start))) {
				return true;
			}
		}
	}
	//otherwise, there were no conflicts
	return false;
}


void schedule::computeScore() {
	// if the rating is 5 or below, subtract a penalty
	// factor so that you don't get schedules with
	// five 10's and one 1. Also add a factor for time
	// of day. Which are added depends on user selections
	// stored in prefer struct
	for( auto subject: myCourses) {
		for (unsigned int i = 0; i < subject.second.size(); i++) {
			classInfo section = subject.second[i];
			int score = 0;

			if (prefer.prof) {
				if (section.rating <= 5) {
					score -= (6 - section.rating) * 2;
				}
				score += section.rating;
			}
			if (prefer.morning) {
				score += (24 - section.start) / 4;
			} else if (prefer.afternoon) {
				score += section.start / 4;
			}

			myCourses[subject.first][i].score = score;
		}
	}
}


bool schedule::makeSchedule() {
	// When this function is called, it takes the current states of the object (options and courses)
	// to find the optimal schedule. This function takes every possible permutation of finding a schedule
	// and can take a long time with more than 8 required classes.
	// Arguments: None
	// Returns: bool somePass - returns whether any schedules are made, false if none possible

	// to make the schedule, we first need to compute the score then with
	// the computed score, sort all of the courses by the rating
	computeScore();
	sortRating();

	// at this point, the myCourses map has a key that contains the name of the class
	// the the value is a vector that contains all of the sections for that class, sorted
	// from the highest scored section to the lowest.
	int length = courseReq.size();


	// make a list that will have every permutation iterated through
	// to index the courseReq vector, this is because the std::next_permutation
	// doesn't work with our vector.
	int permutation[length];
	for (int i = 0; i < length; i++) {
		permutation[i] = i;
	}

	// min value of int on vm in case best score is negative
	int bestScore = -2147483648l;

	// keep track if any schedule works
	bool somePass = false;

	do {
		// Make a possible schedule and assume one can be made in this order
		unordered_map<string, classInfo> currentSchedule;
		int currentScore = 0;
		bool thisPass = true;

		// For each class, go through the list of sections, checking to see if the
		// highest scored section has any conflicts with existing sections
		for(unsigned int i = 0; i < length; i++) {
			// find course name using permutation vector
			string courseName = courseReq[permutation[i]];
			// use courseName to get a vector of sections for that course
			vector<classInfo> courseSections = myCourses[courseName];

			// until a course without conflict is found or the
			// all courses are checked, check the next course
			auto x = courseSections.begin();
			bool conflict = true;
			while (conflict && (x != courseSections.end())) {
				conflict = checkConflict(currentSchedule, *x);
				if (conflict) {
					x++;
				}
			}

			// if no conflict, add to current schedule
			if (!conflict) {
				currentSchedule[courseName] = *x;

				currentScore += x->score;
			} else {
				// otherwise, making a schedule with this permutation is impossible
				thisPass = false;
				break;
			}
		}

		// if this passed, then some passed (there is at least possible schedule)
		if (thisPass) {
			somePass = true;
		}
		// if this score is the best and the schedule was made, update best schedule
		if ((currentScore > bestScore) && thisPass) {
			finalSchedule = currentSchedule;
			bestScore = currentScore;
		}
	// every loop, make a new permutation of the permutation list
	} while (next_permutation(permutation, permutation+length));
	// returns if any schedules were made
	return somePass;
}


bool schedule::makeScheduleFast() {
	// When this function is called, it takes the current states of the object (options and courses)
	// to find a good schedule. This function takes a small set of permutations of finding a schedule
	// and runs very fast but, especially in inputs larger than 10, may not find a schedule even if possible.
	// Arguments: None
	// Returns: bool somePass - returns whether any schedules are made, false if none possible

	// to make the schedule, we first need to compute the score then with
	// the computed score, sort all of the courses by the rating
	computeScore();
	sortRating();


	// keep track if any schedule works and best score
	// -2147483648l is the min value of an int on the vm
	int bestScore = -2147483648l;
	bool somePass = false;

	// for each starting point(offset), greedily find best schedule
	for (unsigned int offset = 0; offset < courseReq.size(); offset++) {
		// declare current schedule, its score, and if it can be made
		unordered_map<string, classInfo> currentSchedule;
		int currentScore = 0;
		bool thisPass = true;

		// for each class, with the offset, find the best section that doesn't conflict
		// with previously added classes
		for(unsigned int i = 0; i < courseReq.size(); i++) {
			// get name of course and list of all sections for it
			string courseName = courseReq[(i + offset) % courseReq.size()];
			vector<classInfo> courseSections = myCourses[courseName];
			bool conflict = true;

			// starting with the highest rated section,
			// until a section without conflict is found or
			// the end of the vector is reached, check next section
			auto x = courseSections.begin();
			while (conflict && (x != courseSections.end())) {
				conflict = checkConflict(currentSchedule, *x);
				if (conflict) {
					x++;
				}
			}

			// if class found, add to schedule
			if (!conflict) {
				currentSchedule[courseName] = *x;

				currentScore += x->score;
			// otherwise break as this permutation won't
			// produce a complete schedule
			} else {
				thisPass = false;
				break;
			}
		}

		// if this passed, then some passed(a schedule was found)
		if (thisPass) {
			somePass = true;
		}
		// if a schedule was made, update best if better
		if ((currentScore > bestScore) && thisPass) {
			finalSchedule = currentSchedule;
			bestScore = currentScore;
		}
	}
	// returns if any schedules were made
	return somePass;
}


void schedule::insertRequired(string required) {
	/*
	Inserts a required class into the courseReq vector
	Inputs: the required class (string)
	Outputs: None
	*/
	courseReq.push_back(required);
}


void schedule::preferMorn(bool pref) {
	/*
	Sets the morning preference
	Inputs: the preference for morning (bool)
	Outputs: None
	*/
	prefer.morning = pref;
	// if a user prefers morning, they cannot also prefer afternoon
	if (pref) {
		prefer.afternoon = false;
	}
	checkPrefs();
}


void schedule::preferAft(bool pref) {
	/*
	Sets the afternoon preference
	Inputs: the preference for afternoon (bool)
	Outputs: None
	*/
	prefer.afternoon = pref;
	// if a user prefers afternoon they cannot also prefer mornings
	if (pref) {
		prefer.morning = false;
	}
	checkPrefs();
}


void schedule::preferProf(bool pref) {
	/*
	Sets the professor preference
	Inputs: the preference for professor (bool)
	Outputs: None
	*/
	prefer.prof = pref;
	checkPrefs();
}


void schedule::checkPrefs() {
	// if the user has not selected their preferences, the default is best professor
	if(!(prefer.prof || prefer.morning || prefer.afternoon)) {
		prefer.prof = true;
	}
}