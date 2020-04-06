// ============================
// Name: Josh Wutzke, Jordan Rusk and Sarah Hildt
// CMPUT 275, Winter 2020
// Final Project
// ============================
#include <iostream>
#include <string>
#include <fstream>
#include "schedule.h"
using namespace std;

int8_t strToInt(string days){
	/*
	Converts a binary string to an integer. Each 1 means the
	class is on that, and 0's mean it is not
	Inputs: string
	Outputs: int8_t, the integer represented by the binary
	*/
	int8_t myDay = 0;
	for(int i=0;i<7;i++){
		// shifting the integer each time
		myDay  = myDay << 1;
		if(days[i]=='1'){
			//if the position in binary is 1 add to the binary
			myDay ++;
		}
	}
	return myDay;
}

void readFile(char file_name[], schedule& userSchedule){
	/*
	reads a text file containing class sections
	Inputs: a filename and schedule to build
	Outputs: None
	*/
	ifstream reading;
	string mystr; 
	string mysub;
	string whatDay;
	string className;
	bool getClass = false;
	classInfo thisClass;
	int cutoff;
	// the file is opened and read through line by line
	reading.open(file_name);
	{
		while(getline(reading,mystr)){
			// if a line begins with C, it is a class name
			if(mystr[0]=='C'){
				cutoff = mystr.find(',');
				className = mystr.substr(2,cutoff-2);
				// if the class is a required class, we can get the sections
				if(userSchedule.inReq(className)){
					getClass = true;
				}else{
					getClass = false;
				}
				
			}else if(getClass){
				// the information is stored in a comma separated file
				cutoff = mystr.find(',');
				thisClass.section = mystr.substr(0,cutoff);
				mysub = mystr.substr(cutoff+1);
				cutoff = mysub.find(',');
				// the start and end times are added to the section as doubles
				thisClass.start = stod(mysub.substr(0,cutoff));
				mysub = mysub.substr(cutoff+1);
				cutoff = mysub.find(',');
				thisClass.end = stod(mysub.substr(0,cutoff));
				mysub = mysub.substr(cutoff+1);
				cutoff = mysub.find(',');
				whatDay = mysub.substr(0,cutoff);
				// the day is stored as a binary number so it must be converted to an int8_t
				thisClass.day = strToInt(whatDay);
				mysub = mysub.substr(cutoff+1);
				cutoff = mysub.find(',');
				// the professor is simply a string that can be added directly to the section
				thisClass.professor = mysub.substr(0,cutoff);
				mysub = mysub.substr(cutoff+1);
				thisClass.rating = stoi(mysub);
				// each section is added to the userSchedule under the class name
				userSchedule.insert(className,thisClass);
			}
		}
	}
	reading.close();
}
bool welcomePrompt(schedule& userSchedule){
	/*
	Takes in user input for the schedule
	Inputs: Schedule to build
	Outputs: Bool fast - if loop limit is active or not
	*/
	cout << "Welcome to schedule builder!" << endl;
	cout << "How many classes (including labs and seminars) will you be taking this semester? ";
	int numberOfClasses;
	string className;
	// takes in the number of classes the user intends to take
	cin >> numberOfClasses;
	cout << "Please enter each class followed by the enter key " << endl;

	for(int i=0;i<numberOfClasses;i++){
		//ignores whitespace at beginning of line
		cin >> ws;
		// gets the class from the user and adds it to the vector of required cources
		getline(cin,className);		
		userSchedule.insertRequired(className);
	}
	cout << "Do you have a preference for morning or afternoon classes? " << endl;
	cout << "Please select a preference (morning, afternoon, n/a): ";
	string selection;
	bool answered = false;
	bool prof_only = false;
	// loops until the user enters valid input
	while(!answered){
		cin >> selection;
		if(selection == "morning"){
			userSchedule.preferMorn(true);
			answered = true;
		}else if(selection == "afternoon"){
			userSchedule.preferAft(true);
			answered = true;
		}else if(selection == "n/a"){
			// if time does not matter, the code simply chooses the 
			// highest possible professor. If there is no preference for 
			// time, the code will automatically select preferProf(true)
			answered = true;
			prof_only = true;
		}else{
			cout << "Please select a valid preference (morning, afternoon, n/a): ";
		}
	}
	answered = false;
	// loops until a valid professor preference is inputted
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
	cout << "Would you like to run fast or in depth? (fast/slow): ";
	answered = false;
	while (!answered) {
		cin >> selection;
		if (selection == "fast") {
			return true;
		} else if (selection == "slow") {
			return false;
		} else {
			cout << "Please select a valid preference (fast/slow): ";
		}
	}
}

int main(int argc, char *argv[]){
	/*
	The main file that runs the code
	Inputs: The arguments from the command line
	Outputs: Int 0
	*/ 
	schedule userSchedule;

	// check to see if the correct number of command arguments was given
	if (argc<2) {
		cout << "Please enter your available course section data file" << endl;
	} else if (argc>2) {
		cout << "Too many arguments. Please try again" << endl;
	} else {
		// displays the welcome prompt to the user
		bool fast = welcomePrompt(userSchedule);
		// reads the file that the courses are to be chosen from
		readFile(argv[1],userSchedule);
		cout << endl;
		cout << "Loading..." << endl;
		bool itworked;
		if (fast) {
			itworked = userSchedule.makeScheduleFast();
		} else {
			itworked = userSchedule.makeSchedule();
		}
		// if the schedule build was successful, the optimal schedule will
		// be displayed. If a schedule cannot be made (ie required class not in text file)
		// an error message will be displayed. 
		cout << endl;
		if (itworked) {
			userSchedule.display();
		} else {
			cout << "Sorry! A schedule cannot be made with these classes." << endl;
		}
	}

	return 0;
}