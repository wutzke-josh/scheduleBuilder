#include "schedule.h"
#include <iomanip>

using namespace std;

void schedule::insert(string name, classInfo myClass){
	myCourses[name].push_back(myClass);
}
bool schedule::inReq(string className){
	bool found = false;
	for(auto x: courseReq){
		if(x==className){
			found = true;
		}
	}
	return found;
}

void schedule::display(){
	vector<string> week = {"Sundays","Mondays","Tuesdays","Wednesdays","Thursdays","Fridays","Saturdays"};
	for(auto x: finalSchedule){
		cout << x.first << ": " << endl;
		cout << "Section " << x.second.section << " with " << x.second.professor;
		cout << " (rating = " << x.second.rating << ")" << endl;
		cout << "on ";
		for(int i=6;i>=0;i--){
			if((x.second.day >> i) & 1){
				cout << week[6-i] << " ";
			}
		}
		
		int startHour = x.second.start;
		int startMin = (x.second.start-startHour)*60.0;

		int endHour = x.second.end;
		int endMin = (x.second.end-endHour)*60.0;
		cout << "from " << startHour << ":" <<setw(2) << setfill('0') << right << startMin << " to " << endHour << ":";
		cout <<setw(2) << setfill('0') << right << endMin << endl;
		cout << endl;
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
		if(thisCourse[lo].score>=thisCourse[start+n-1].score){
			lo++;
		}else if(thisCourse[hi].score<thisCourse[start+n-1].score){
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
	for(auto x: schedule) {
		if ((x.second.day & section.day) != 0) {
			if (!((x.second.end <= section.start) || (section.end <= x.second.start))) {
				return true;
			}
		}
	}
	return false;
}

void schedule::computeScore() {
	// if the rating is 5 or below, subtract a penalty
	// factor so that you don't get schedules with
	// five 10's and one 1
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
	computeScore();
	sortRating();

	int bestScore = 0;

	// keep track if any schedule works
	bool somePass = false;

	for (unsigned int offset = 0; offset < courseReq.size(); offset++) {
		unordered_map<string, classInfo> currentSchedule;
		int currentScore = 0;
		bool thisPass = true;
		for(unsigned int i = 0; i < courseReq.size(); i++) {
			string courseName = courseReq[(i + offset) % courseReq.size()];
			vector<classInfo> courseSections = myCourses[courseName];
			bool conflict = true;


			auto x = courseSections.begin();
			while (conflict && (x != courseSections.end())) {
				conflict = checkConflict(currentSchedule, *x);
				if (conflict) {
					x++;
				}
			}

			if (!conflict) {
				currentSchedule[courseName] = *x;

				currentScore += x->score;
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


void schedule::preferMorn(bool pref) {
	prefer.morning = pref;
	if (pref) {
		prefer.afternoon = false;
	}
	checkPrefs();
}

void schedule::preferAft(bool pref) {
	prefer.afternoon = pref;
	if (pref) {
		prefer.morning = false;
	}
	checkPrefs();
}
void schedule::preferProf(bool pref) {
	prefer.prof = pref;
	checkPrefs();
}
void schedule::checkPrefs() {
	if(!(prefer.prof || prefer.morning || prefer.afternoon)) {
		prefer.prof = true;
	}
}