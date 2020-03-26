#include "schedule.h"

using namespace std;

void schedule::insert(string name, classInfo myClass){
	myCourses[name].push_back(myClass);
}
void schedule::display(){
	for(auto x: myCourses){
		cout << x.first << ":" << endl;
		for(auto y: x.second){
			cout << y.section << " " <<y.rating << endl;
		}
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
