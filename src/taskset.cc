#include "taskset.h"

taskset::taskset(ifstream &fin) {
	fin>>tnum>>pnum>>hp;

	execute=(int*)malloc(sizeof(int)*tnum);
	period=(int*)malloc(sizeof(int)*tnum);

	for(int i=0; i<tnum; i++)
		fin>>period[i];
	for(int i=0; i<tnum; i++)
		fin>>execute[i];

	int *flag=(int*)malloc(sizeof(int)*hp);
	for(int i=0; i<hp; i++)
		flag[i]=0;
	for(int i=0; i<tnum; i++)
		for(int j=0; j<(hp/period[i]); j++)
			flag[j*period[i]]=1;
	rnum=0;
	for(int i=0; i<hp; i++)
		rnum+=flag[i];

	rwidth=(int*)malloc(sizeof(int)*rnum);
	b=(int*)malloc(sizeof(int)*(rnum+1));

	int pre=hp;
	int ind=rnum-1;
	for(int i=hp-1; i>=0; i--) {
		if(flag[i]==1) {
			rwidth[ind]=pre-i;
			pre=i;
			ind--;
		}
	}

	b[0]=0;
	for(int i=0; i<rnum; i++)
		b[i+1]=b[i]+rwidth[i];
	free(flag);
}
