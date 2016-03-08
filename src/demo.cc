#include <iostream>
#include <fstream>

#include "taskset.h"
#include "algo.h"

using namespace std;

int main(int argc, char **argv) {
	ifstream fin(argv[1]);
	taskset ts(fin);
/*	for(int i=0; i<=ts.rnum; i++)
		cout<<ts.b[i]<<" ";
	cout<<endl;*/
	algo al(ts);
	int ret=al.schedule();
	if(ret==1) {
		if(al.checkSchedule()) {
//			cout<<"success schedule"<<endl;
		}
		al.myOldAllocate();
		cout<<al.countPreemption()<<"\t"<<al.countMigration()<<endl;
//		al.displayResult();
//		cout<<"preemption="<<al.countPreemption()<<endl;
//		cout<<"migration="<<al.countMigration()<<endl;
	}
	return 0;
}
