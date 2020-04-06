#include <iostream>
#include <string>
#include <fstream>
#include "schedule.h"
using namespace std;

int8_t strToInt(string days){
	int8_t myDay = 0;
	for(int i=0;i<7;i++){
		myDay  = myDay << 1;
		if(days[i]=='1'){
			myDay ++;
		}
	}
	return myDay;
}

void readFile(char file_name[], schedule& userSchedule){
	ifstream reading;
	string mystr; 
	string mysub;
	string whatDay;
	string className;
	bool getClass = false;
	
	classInfo thisClass;
	int cutoff;
	reading.open(file_name);
	{
		while(getline(reading,mystr)){
			if(mystr[0]=='C'){
				//cout << "searching " << endl;
				cutoff = mystr.find(',');
				className = mystr.substr(2,cutoff-2);
				if(userSchedule.inReq(className)){
					getClass = true;
					//cout << "found it" << endl;
				}else{
					//cout << "not this one" << endl;
					getClass = false;
				}
				
			}else if(getClass){
				cutoff = mystr.find(',');
				thisClass.section = mystr.substr(0,cutoff);
				mysub = mystr.substr(cutoff+1);
				cutoff = mysub.find(',');
				thisClass.start = stod(mysub.substr(0,cutoff));
				mysub = mysub.substr(cutoff+1);
				cutoff = mysub.find(',');
				thisClass.end = stod(mysub.substr(0,cutoff));
				mysub = mysub.substr(cutoff+1);
				cutoff = mysub.find(',');
				whatDay = mysub.substr(0,cutoff);
				thisClass.day = strToInt(whatDay);
				mysub = mysub.substr(cutoff+1);
				cutoff = mysub.find(',');
				thisClass.professor = mysub.substr(0,cutoff);
				//cout << "prof " << thisClass.professor << endl;
				mysub = mysub.substr(cutoff+1);
				thisClass.rating = stoi(mysub);
				userSchedule.insert(className,thisClass);
			}
		}
	}
}
void welcomePrompt(schedule& userSchedule){
	cout << "Welcome to schedule builder!" << endl;
	cout << "How many classes (including labs and seminars) will you be taking this semester? ";
	int numberOfClasses;
	string className;
	cin >> numberOfClasses;
	cout << "Please enter each class followed by the enter key " << endl;

	for(int i=0;i<numberOfClasses;i++){
		//scanf("%[\n]s",className);
		cin >> ws;
		getline(cin,className);

		// cout << "Class: " << className << endl;
		
		userSchedule.insertRequired(className);
	}
	cout << "Do you have a preference for morning or afternoon classes? " << endl;
	cout << "Please select a preference (morning, afternoon, n/a): ";
	string selection;
	bool answered = false;
	bool prof_only = false;
	while(!answered){
		cin >> selection;
		if(selection == "morning"){
			userSchedule.preferMorn(true);
			answered = true;
		}else if(selection == "afternoon"){
			userSchedule.preferAft(true);
			answered = true;
		}else if(selection == "n/a"){
			answered = true;
			prof_only = true;
		}else{
			cout << "Please select a valid preference (morning, afternoon, n/a): ";
		}
	}
	answered = false;
	if(!prof_only){
		cout << "Would you like to combine your preference with a high professor rating? (y/n) ";
		while(!answered){
			cin >> selection;
			if(selection == "y" || selection == "Y" ){
				userSchedule.preferProf(true);
				answered = true;
			}else if(selection=="n" || selection=="N"){
				userSchedule.preferProf(false);
				answered = true;
			}else{
				cout << "Please select a valid preference (y/n): ";	
			}
		}
	}

}

int main(int argc, char *argv[]){
	schedule userSchedule;

	welcomePrompt(userSchedule);
	readFile(argv[1],userSchedule);
	cout << endl;
	cout << "Loading..." << endl;
	bool itworked = userSchedule.makeSchedule();
	cout << endl;
	if (itworked) {
		userSchedule.display();
	} else {
		cout << "Sorry! A schedule cannot be made with these classes" << endl;
	}

	return 0;
}