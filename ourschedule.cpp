#include <iostream>
#include <string>
#include <fstream>
#include "schedule.h"
using namespace std;

int readFile(char file_name[], schedule& ourSchedule){
	ifstream reading;
	string mystr; 
	string mysub;
	string whatDay;
	string className;
	
	classInfo thisClass;
	int cutoff;
	reading.open(file_name);
	{
		while(getline(reading,mystr)){
			if(mystr[0]=='C'){
				cutoff = mystr.find(',');
				className = mystr.substr(2,cutoff-2);
				ourSchedule.insertRequired(className);

				cout << "Class " << className << endl;
			}else{
				cutoff = mystr.find(',');
				thisClass.section = mystr.substr(0,cutoff);
				//cout << "section " << thisClass.section << ' ';
				mysub = mystr.substr(cutoff+1);
				cutoff = mysub.find(',');

				thisClass.start = stoi(mysub.substr(0,cutoff));
				//cout << "start " << thisClass.start<< ' ';
				mysub = mysub.substr(cutoff+1);
				cutoff = mysub.find(',');
				thisClass.end = stoi(mysub.substr(0,cutoff));
				//cout << "end " << thisClass.end<< ' ';
				mysub = mysub.substr(cutoff+1);
				cutoff = mysub.find(',');
				whatDay = mysub.substr(0,cutoff);
				//cout << "day " << whatDay << ' ';
				if(whatDay == "true"){
					//cout << "in true" << ' ';
					thisClass.day = true;
				}else{
					//cout << "in false" << ' ';
					thisClass.day = false;
				}
				mysub = mysub.substr(cutoff+1);
				cutoff = mysub.find(',');
				thisClass.professor = mysub.substr(0,cutoff);
				//cout << "prof " << thisClass.professor<< ' ';
				mysub = mysub.substr(cutoff+1);
				thisClass.rating = stoi(mysub);
				//cout << "rate " << thisClass.rating << endl;

				ourSchedule.insert(className,thisClass);
			}
		}
	}
}

int main(int argc, char *argv[]){
	// schedule ourSchedule;
	// cout << "before" << endl;
	// readFile(argv[1],ourSchedule);
	// cout << "after" << endl;

	schedule test;
	classInfo section1;

	test.insertRequired("ece 311");
	test.insertRequired("ece 321");
	test.insertRequired("ece 325");
	test.insertRequired("stat 235");
	test.insertRequired("ch e 243");
	test.insertRequired("soc 225");

	section1 = {"EB01",9,10,true,"David Smith",3};
	test.insert("ece 311",section1);
	section1 = {"EB02",13,14,false,"Priyanka Emayan",10};
	test.insert("ece 311",section1);
	section1 = {"EB03",9,10,true,"Mateo Ruiz",8};
	test.insert("ece 311",section1);

	section1 = {"B1",11,12,true,"Megan Nemeth",6};
	test.insert("ece 321",section1);
	section1 = {"B2",14,15,true,"Dustin Meyers",7};
	test.insert("ece 321",section1);

	section1 = {"EB1",13,14,false,"Anna Hayworth",4};
	test.insert("ece 325",section1);

	section1 = {"B1",14,15,false,"Muhammad Shareef",9};
	test.insert("stat 235",section1);
	section1 = {"B2",8,9,true,"Jennifer Fawcett",5};
	test.insert("stat 235",section1);

	section1 = {"EB01",8,9,true,"Miriam Barasha",8};
	test.insert("ch e 243",section1);
	section1 = {"EB02",11,12,true,"Lydia Johnstone",1};
	test.insert("ch e 243",section1);
	section1 = {"EB03",8,9,false,"Alan Campbell",8};
	test.insert("ch e 243",section1);

	section1 = {"A1",13,14,false,"Andy Dufresne",7};
	test.insert("soc 225",section1);
	// section1 = {"ebo2",9,10,false,"pescke",7};
	// test.insert("math",section1);

	// section1 = {"eb01", 13, 14, true, "mar",10};
	// test.insert("science", section1);
	// section1 = {"eb02", 9, 10, true, "saiyu",7};
	// test.insert("science", section1);

	// section1 = {"eb01", 10, 11, true, "hello",2};
	// test.insert("sad", section1);
	// section1 = {"eb02", 11, 14, true, "this",7};
	// test.insert("sad", section1);


	test.preferMorn();
	cout << "morn" << endl;
	test.preferProf(false);
	cout << "prof" << endl;
	bool itworked = test.makeSchedule();
	cout << "made" << endl;
	if (itworked) {
		test.display();
	} else {
		cout << "uhoh :(" << endl;
	}

	return 0;
}