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
				cutoff = mystr.find(',');
				className = mystr.substr(2,cutoff-2);
				if(userSchedule.inReq(className)){
					getClass = true;
				}else{
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
	char className[256];
	cin >> numberOfClasses;
	cout << "Please enter each class followed by the enter key " << endl;
	for(int i=0;i<numberOfClasses;i++){
		cin.getline(className,256);
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
	// readReq(argv[1],userSchedule);

	welcomePrompt(userSchedule);
	readFile(argv[1],userSchedule);
	bool itworked = userSchedule.makeSchedule();
	if (itworked) {
		userSchedule.display();
	} else {
		cout << "Sorry! A schedule cannot be made with these classes" << endl;
	}

	return 0;
}

	// schedule userSchedule;
	// classInfo section1;
	// userSchedule.insertRequired("soc 225");
	// userSchedule.insertRequired("ch e 243");
	// userSchedule.insertRequired("stat 235");
	// userSchedule.insertRequired("ece 325");
	// userSchedule.insertRequired("ece 321");
	// userSchedule.insertRequired("ece 311");

	// section1 = {"EB01",9,10,true,"David Smith",3};
	// userSchedule.insert("ece 311",section1);
	// section1 = {"EB02",13,14,false,"Priyanka Emayan",10};
	// userSchedule.insert("ece 311",section1);
	// section1 = {"EB03",9,10,true,"Mateo Ruiz",8};
	// userSchedule.insert("ece 311",section1);

	// section1 = {"B1",11,12,true,"Megan Nemeth",6};
	// userSchedule.insert("ece 321",section1);
	// section1 = {"B2",14,15,true,"Dustin Meyers",7};
	// userSchedule.insert("ece 321",section1);

	// section1 = {"EB1",13,14,false,"Anna Hayworth",4};
	// userSchedule.insert("ece 325",section1);

	// section1 = {"B1",14,15,false,"Muhammad Shareef",9};
	// userSchedule.insert("stat 235",section1);
	// section1 = {"B2",8,9,true,"Jennifer Fawcett",5};
	// userSchedule.insert("stat 235",section1);

	// section1 = {"EB01",8,9,true,"Miriam Barasha",8};
	// userSchedule.insert("ch e 243",section1);
	// section1 = {"EB02",11,12,true,"Lydia Johnstone",1};
	// userSchedule.insert("ch e 243",section1);
	// section1 = {"EB03",8,9,false,"Alan Campbell",8};
	// userSchedule.insert("ch e 243",section1);

	// section1 = {"A1",13,14,false,"Andy Dufresne",7};
	// userSchedule.insert("soc 225",section1);
	// section1 = {"ebo2",9,10,false,"pescke",7};
	// userSchedule.insert("math",section1);
	// userSchedule.insertRequired("science");
	// userSchedule.insertRequired("sad");

	// section1 = {"eb01", 13, 14, true, "mar",10};
	// userSchedule.insert("science", section1);
	// section1 = {"eb02", 9, 10, true, "saiyu",7};
	// userSchedule.insert("science", section1);

	// section1 = {"eb01", 10, 11, true, "hello",2};
	// userSchedule.insert("sad", section1);
	// section1 = {"eb02", 11, 14, true, "this",7};
	// userSchedule.insert("sad", section1);