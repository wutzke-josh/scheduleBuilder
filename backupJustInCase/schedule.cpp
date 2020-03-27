#include "schedule.h"

using namespace std;

void schedule::insert(string name, classInfo myClass){
	myCourses[name].push_back(myClass);
}
void schedule::display(){
	for(auto x: finalSchedule){
		cout << x.first << ' ' << x.second.section << ' ' << x.second.rating << endl;
	}
}

void schedule::swap(classInfo& c1, classInfo& c2) {
	classInfo tmp = c1;
	c1 = c2;
	c2 = tmp;
} 
int schedule::pivot(vector<classInfo>& thisCourse, int n, int pi,int start){
	swap(thisCourse[pi],thisCourse[start+n-1]);
	int lo = start;
	int hi = start+n-2;
	while(lo<=hi){
		if(thisCourse[lo].rating>=thisCourse[start+n-1].rating){
			lo++;
		}else if(thisCourse[hi].rating<thisCourse[start+n-1].rating){
			hi --;
		}else{
			swap(thisCourse[lo],thisCourse[hi]);
		}
	}

	swap(thisCourse[lo],thisCourse[start+n-1]);
	
	return lo;

}


void schedule::quickSort(vector<classInfo>& thisCourse, int start, int n){

	if(n<=1){
		return;
	}else{
		int pi = start+(n/2);
		int new_pi = pivot(thisCourse,n,pi,start);

		quickSort(thisCourse,start,new_pi-start);
		
		quickSort(thisCourse,new_pi+1,n-(new_pi-start)-1);
	}
}
void schedule::sortRating(){
	auto x = myCourses.begin();
	while (x != myCourses.end()) {
		quickSort(x->second,0,(x->second).size());
		x++;
	}
}

bool schedule::checkConflict(unordered_map<string, classInfo> schedule, classInfo section) {
	bool conflict = false;
	for(auto x: schedule) {
		if (x.second.day == section.day) {
			if (!((x.second.end <= section.start) || (section.end <= x.second.start))) {
				conflict = true;
			}
		}
	}
	return conflict;
}

int schedule::computeScore(const classInfo& section) {
	// if the rating is 5 or below, subtract a penalty
	// factor so that you don't get schedules with
	// five 10's and one 1
	int score = 0;
	if (section.rating <= 5) {
		score -= (6 - section.rating) * 2;
	}
	score += section.rating;
	return score;
}


bool schedule::makeSchedule() {
	sortRating();

	int bestScore = 0;

	// keep track if any schedule works
	bool somePass = false;

	for (int offset = 0; offset < courseReq.size(); offset++) {
		unordered_map<string, classInfo> currentSchedule;
		int currentScore = 0;
		bool thisPass = true;
		for(int i = 0; i < courseReq.size(); i++) {
			string courseName = courseReq[(i + offset) % courseReq.size()];
			vector<classInfo> x = myCourses[courseName];
			bool conflict;
			int j = 0;
			conflict = checkConflict(currentSchedule, x[j]);

			while (conflict && (j < x.size())) {
				j++;
				conflict = checkConflict(currentSchedule, x[j]);
			}

			if (!conflict) {
				currentSchedule[courseName] = x[j];

				currentScore += computeScore(x[j]);
			} else {
				thisPass = false;
				break;
			}
		}

		// if this passed, then some passed
		if (thisPass) {
			somePass = true;
		}
		if ((currentScore > bestScore) && thisPass) {
			finalSchedule = currentSchedule;
			bestScore = currentScore;
		}
	}
	// 'should' return true if it worked
	return somePass;
}

void schedule::insertRequired(string required) {
	courseReq.push_back(required);
}
