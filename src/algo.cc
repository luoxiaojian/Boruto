#include "algo.h"
#include <iostream>
#include <set>
#include <stdlib.h>

#define MIN(a, b) ((a)>(b)?(b):(a))
#define MAX(a, b) ((a)>(b)?(a):(b))

#define DISPLAY
#define DETAIL
#define SEARCHTASK
//#define MANDP
#define STRIPTASK

#define DETAILBINGO

#define ALLOCATE_OPTIMIZE 3

using std::cout;
using std::endl;
using std::set;

algo::algo(taskset& ts) {
	tnum=ts.tnum;
	rnum=ts.rnum;
	pnum=ts.pnum;
	execute=ts.execute;
	period=ts.period;
	rwidth=ts.rwidth;
	b=ts.b;
	hp=ts.hp;

	alloc=(int**)malloc(sizeof(int*)*tnum);
	for(int i=0; i<tnum; i++)
		alloc[i]=(int*)malloc(sizeof(int)*rnum);
	for(int i=0; i<tnum; i++)
		for(int j=0; j<rnum; j++)
			alloc[i][j]=0;

	result=(int**)malloc(sizeof(int*)*pnum);
	for(int i=0; i<pnum; i++)
		result[i]=(int*)malloc(sizeof(int)*hp);
	for(int i=0; i<pnum; i++)
		for(int j=0; j<hp; j++)
			result[i][j]=-1;	

	remains=(int*)malloc(sizeof(int)*rnum);
	for(int i=0; i<rnum; i++)
		remains[i]=pnum*rwidth[i];

	most_to_exe=(int*)malloc(sizeof(int)*tnum);
	least_to_exe=(int*)malloc(sizeof(int)*tnum);
	alloced_to_exe=(int*)malloc(sizeof(int)*tnum);
	laxity=(int*)malloc(sizeof(int)*tnum);
	jstart=(int*)malloc(sizeof(int)*tnum);

	for(int i=0; i<tnum; i++) {
		most_to_exe[i]=0;
		least_to_exe[i]=0;
		alloced_to_exe[i]=0;
		laxity[i]=period[i]-execute[i];
#ifdef DISPLAY
		cout<<"laxity["<<i<<"]="<<laxity[i]<<endl;
#endif
		jstart[i]=0;
	}

	rflag=(int*)malloc(sizeof(int)*rnum);
	for(int i=0; i<rnum; i++)
		rflag[i]=0;

	totalEmpty=0;
}

void algo::clearFlags() {
	for(int i=0; i<rnum; i++)
		rflag[i]=0;
}

int algo::isSrcTask(int tid) {
	if(alloc[tid][currentRowId]>0 && jstart[tid]<b[currentRowId])
		return 1;
	else
		return 0;
}

int algo::isDstTask(int tid) {
	if(laxity[tid]>0)
		return 1;
	else
		return 0;
}

int algo::searchEmptyDirectly(int tid, int overflow) {
	int totalModified=0;
	for(int rid=jstart[tid]; rid<currentRowId; rid++) {
		if(alloc[tid][rid]<rwidth[rid] && remains[rid]>0) {
			int toModify=MIN(remains[rid], rwidth[rid]-alloc[tid][rid]);
			toModify=MIN(overflow, toModify);
			toModify=MIN(overflow, alloc[tid][currentRowId]);
			alloc[tid][rid]+=toModify;
			alloc[tid][currentRowId]-=toModify;
			remains[rid]-=toModify;
			totalEmpty-=toModify;
			totalModified+=toModify;
			overflow-=toModify;
		}
	}
	return totalModified;
}

int algo::searchEmptyInDirectly(int tid, int rid) {
	rflag[rid]=1;
	int startTime=(b[rid]/period[tid])*period[tid];
	int endTime=startTime+period[tid];
	int startRowId=rid;
	while(b[startRowId]>startTime)
		startRowId--;
	int endRowId=rid;
	while(b[endRowId+1]<endTime)
		endRowId++;
	endRowId=MIN(endRowId, currentRowId-1);
	for(int i=startRowId; i<=endRowId; i++) {
		if(remains[i]>0 && alloc[tid][i]<rwidth[i]) {
			remains[i]--;
			totalEmpty--;
			alloc[tid][i]++;
#ifdef DETAIL
			cout<<"("<<i<<")<==";
#endif
			return 1;
		}
	}
	for(int i=startRowId; i<=endRowId; i++) {
		if(rflag[i]==1)
			continue;
		if(alloc[tid][i]==rwidth[i])
			continue;
		if(rid==i)
			continue;
		for(int j=0; j<tnum; j++) {
			if(alloc[j][i]>0) {
				if(searchEmptyInDirectly(j, i)==1) {
					alloc[j][i]--;
					alloc[tid][i]++;
#ifdef DETAIL
					cout<<"["<<tid<<", "<<rid<<"]<==";
#endif
					return 1;
				}
			}
		}
	}
	return 0;
}

int algo::searchTaskDirectly(int tid, int overflow) {
	int rstart=jstart[tid];
	int rend=currentRowId;
	int totalModified=0;
	for(int kid=0; kid<tnum; kid++) {
		if(kid==tid)
			continue;
		if(!isDstTask(kid))
			continue;
		for(int i=MAX(rstart, jstart[kid]); i<rend; i++) {
			if(alloc[tid][i]<rwidth[i] && alloc[kid][i]>0) {
				int toModify=MIN(alloc[tid][currentRowId], overflow);
				toModify=MIN(toModify, laxity[kid]);
				toModify=MIN(toModify, rwidth[i]-alloc[tid][i]);
				toModify=MIN(toModify, alloc[kid][i]);
				alloc[tid][currentRowId]-=toModify;
				alloc[tid][i]+=toModify;
				alloc[kid][i]-=toModify;
				laxity[kid]-=toModify;
				alloced_to_exe[kid]-=toModify;
				overflow-=toModify;
				totalModified+=toModify;
#ifdef SEARCHTASK
				if(toModify>0)
					cout<<"[DIRECTLY]: task"<<tid<<" ==> task"<<kid<<" changed="<<toModify<<endl;
#endif
			}
			if(overflow==0 || alloc[tid][currentRowId]==0)
				return totalModified;
			if(laxity[kid]==0)
				break;
		}
	}
	return totalModified;
}

int algo::searchTaskInDirectly(int tid, int rid) {
	int startTime=(b[rid]/period[tid])*period[tid];
	int endTime=startTime+period[tid];
	int rstart=rid;
	while(b[rstart]>startTime)
		rstart--;
	int rend=rid;
	while(b[rend+1]<endTime)
		rend++;
	rflag[rid]=1;
	rend=MIN(rend, currentRowId-1);
	for(int i=rstart; i<=rend; i++) {
		if(rflag[i]==1)
			continue;
		if(alloc[tid][i]==rwidth[i])
			continue;
		for(int j=0; j<tnum; j++) {
			if(j==tid)
				continue;
			if(alloc[j][i]==0)
				continue;
			if(isDstTask(j) && i>=jstart[j]) {
				alloc[tid][i]++;
				alloc[j][i]--;
				laxity[j]--;
				alloced_to_exe[j]--;
#ifdef SEARCHTASK
				cout<<"[INDIRECTLY]: ["<<j<<", "<<i<<"]<==";
#endif
				return 1;
			}
		}
	}
	for(int i=rstart; i<=rend; i++) {
		if(rflag[i]==1)
			continue;
		if(alloc[tid][i]==rwidth[i])
			continue;
		for(int j=0; j<tnum; j++) {
			if(j==tid)
				continue;
			if(alloc[j][i]==0)
				continue;
			if(searchTaskInDirectly(j, i)) {
#ifdef SEARCHTASK
				cout<<"["<<j<<", "<<i<<"]<==";
#endif
				alloc[tid][i]++;
				alloc[j][i]--;
				return 1;
			}
		}
	}
	return 0;
} 

int algo::basicAssignCurrentExecution() {
	remains[currentRowId]=0;
	int rstart=b[currentRowId];
	int rend=b[currentRowId+1];
	int window=rend-rstart;
	int capacity=window*pnum;
	int left=capacity;
	for(int i=0; i<tnum; i++) {
		alloc[i][currentRowId]=least_to_exe[i];
		left-=least_to_exe[i];
	}
	for(int i=0; i<tnum; i++) {
		int extra=MIN(left, most_to_exe[i]-least_to_exe[i]);
		alloc[i][currentRowId]+=extra;
		left-=extra;
		if(left==0)
			break;
	}
	for(int i=0; i<tnum; i++) {
		alloced_to_exe[i]+=alloc[i][currentRowId];
		laxity[i]=laxity[i]+alloc[i][currentRowId]-window;
		if(rend%period[i]==0) {
			if(alloced_to_exe[i]!=execute[i]) {
				cout<<"error: task"<<i<<" deadline miss..."<<endl;
				return -1;
			}
			alloced_to_exe[i]=0;
			laxity[i]=period[i]-execute[i];
			jstart[i]=currentRowId+1;
		}
	}
	return 1;
}

int algo::newAssignCurrentExecution() {
	int *DR_used, *DR_id;

	DR_used=(int*)malloc(sizeof(int)*tnum);
	DR_id=(int*)malloc(sizeof(int)*tnum);

	remains[currentRowId]=0;

	for(int i=0; i<tnum; i++) {
		if(currentRowId>0)
			DR_used[i]=alloc[i][currentRowId-1];
		else
			DR_used[i]=0;
		DR_id[i]=i;
	}

	for(int i=0; i<tnum-1; i++)
		for(int j=i+1; j<tnum; j++) {
			if(DR_used[i]<DR_used[j]) {
				int DR_tmp=DR_id[i];
				DR_id[i]=DR_id[j];
				DR_id[j]=DR_tmp;
				DR_tmp=DR_used[i];
				DR_used[i]=DR_used[j];
				DR_used[j]=DR_tmp;
			}
		}

	int rstart=b[currentRowId];
	int rend=b[currentRowId+1];
	int window=rend-rstart;
	int capacity=window*pnum;
	int left=capacity;
	for(int i=0; i<tnum; i++) {
		alloc[i][currentRowId]=least_to_exe[i];
		left-=least_to_exe[i];
	}

	for(int i=0; i<tnum; i++) {
		int id=DR_id[i];
		int extra=MIN(left, most_to_exe[id]-least_to_exe[id]);
		alloc[id][currentRowId]+=extra;
		left-=extra;
		if(left==0)
			break;
	}

	for(int i=0; i<tnum; i++) {
		alloced_to_exe[i]+=alloc[i][currentRowId];
		laxity[i]=laxity[i]+alloc[i][currentRowId]-window;
		if(rend%period[i]==0) {
			if(alloced_to_exe[i]!=execute[i]) {
				cout<<"error: task"<<i<<" deadline miss..."<<endl;
				return -1;
			}
			alloced_to_exe[i]=0;
			laxity[i]=period[i]-execute[i];
			jstart[i]=currentRowId+1;
		}
	}

	free(DR_used);
	free(DR_id);
	return 1;
}

int algo::minRemainsAllocate() {
	int *exeRemains, *taskIds;

	remains[currentRowId]=0;
	exeRemains=(int*)malloc(sizeof(int)*tnum);
	taskIds=(int*)malloc(sizeof(int)*tnum);
	
	for(int i=0; i<tnum; i++) {
		exeRemains[i]=execute[i]-alloced_to_exe[i];
		taskIds[i]=i;
	}

	for(int i=0; i<tnum-1; i++)
		for(int j=i+1; j<tnum; j++) {
			if(exeRemains[i]>exeRemains[j]) {
				int tmp=exeRemains[i];
				exeRemains[i]=exeRemains[j];
				exeRemains[j]=tmp;
				tmp=taskIds[i];
				taskIds[i]=taskIds[j];
				taskIds[j]=tmp;
			}
		}

	int rstart=b[currentRowId];
	int rend=b[currentRowId+1];
	int window=rend-rstart;
	int capacity=window*pnum;
	int left=capacity;
	for(int i=0; i<tnum; i++) {
		alloc[i][currentRowId]=least_to_exe[i];
		left-=least_to_exe[i];
	}

	for(int i=0; i<tnum; i++) {
		int id=taskIds[i];
		int extra=MIN(left, most_to_exe[id]-least_to_exe[id]);
		alloc[id][currentRowId]+=extra;
		left-=extra;
		if(left==0)
			break;
	}

	for(int i=0; i<tnum; i++) {
		alloced_to_exe[i]+=alloc[i][currentRowId];
		laxity[i]=laxity[i]+alloc[i][currentRowId]-window;
		if(rend%period[i]==0) {
			if(alloced_to_exe[i]!=execute[i]) {
				cout<<"error: task"<<i<<" deadline miss..."<<endl;
				return -1;
			}
			alloced_to_exe[i]=0;
			laxity[i]=period[i]-execute[i];
			jstart[i]=currentRowId+1;
		}
	}

	free(taskIds);
	free(exeRemains);

	return 1;
}

int algo::maxAllocedAllocate() {
	int *exeRemains, *taskIds;

	remains[currentRowId]=0;
	exeRemains=(int*)malloc(sizeof(int)*tnum);
	taskIds=(int*)malloc(sizeof(int)*tnum);
	
	for(int i=0; i<tnum; i++) {
		exeRemains[i]=alloced_to_exe[i];
		taskIds[i]=i;
	}

	for(int i=0; i<tnum-1; i++)
		for(int j=i+1; j<tnum; j++) {
			if(exeRemains[i]<exeRemains[j]) {
				int tmp=exeRemains[i];
				exeRemains[i]=exeRemains[j];
				exeRemains[j]=tmp;
				tmp=taskIds[i];
				taskIds[i]=taskIds[j];
				taskIds[j]=tmp;
			}
		}

	int rstart=b[currentRowId];
	int rend=b[currentRowId+1];
	int window=rend-rstart;
	int capacity=window*pnum;
	int left=capacity;
	for(int i=0; i<tnum; i++) {
		alloc[i][currentRowId]=least_to_exe[i];
		left-=least_to_exe[i];
	}

	for(int i=0; i<tnum; i++) {
		int id=taskIds[i];
		int extra=MIN(left, most_to_exe[id]-least_to_exe[id]);
		alloc[id][currentRowId]+=extra;
		left-=extra;
		if(left==0)
			break;
	}

	for(int i=0; i<tnum; i++) {
		alloced_to_exe[i]+=alloc[i][currentRowId];
		laxity[i]=laxity[i]+alloc[i][currentRowId]-window;
		if(rend%period[i]==0) {
			if(alloced_to_exe[i]!=execute[i]) {
				cout<<"error: task"<<i<<" deadline miss..."<<endl;
				return -1;
			}
			alloced_to_exe[i]=0;
			laxity[i]=period[i]-execute[i];
			jstart[i]=currentRowId+1;
		}
	}

	free(taskIds);
	free(exeRemains);

	return 1;
}

int algo::schedule() {
	for(int rid=0; rid<rnum; rid++) {
		currentRowId=rid;
		int rbegin=b[rid];
		int rend=b[rid+1];
#ifdef DISPLAY
		cout<<"rbegin="<<rbegin<<", rend="<<rend<<endl;
#endif
		int window=rend-rbegin;
		int capacity=window*pnum;
		int ub=0, lb=0;

		for(int i=0; i<tnum; i++) {
			most_to_exe[i]=MIN(window, execute[i]-alloced_to_exe[i]);
			ub+=most_to_exe[i];
			least_to_exe[i]=MAX(0, window-laxity[i]);
			lb+=least_to_exe[i];
#ifdef DISPLAY
			cout<<"lte["<<i<<"]="<<least_to_exe[i]<<endl;
#endif
		}

#ifdef DISPLAY
		cout<<"lb="<<lb<<endl;
#endif

		if(ub<=capacity) {
			remains[currentRowId]=capacity-ub;	
			totalEmpty+=capacity-ub;
			for(int i=0; i<tnum; i++) {
				alloc[i][currentRowId]=most_to_exe[i];
				alloced_to_exe[i]+=alloc[i][currentRowId];
				laxity[i]=laxity[i]+alloc[i][currentRowId]-window;
				if(rend%period[i]==0) {
					if(alloced_to_exe[i]!=execute[i]) {
						cout<<"error: task"<<i<<" deadline miss..."<<endl;
						return -1;
					}
					alloced_to_exe[i]=0;
					laxity[i]=period[i]-execute[i];
					jstart[i]=currentRowId+1;
				}
			}
		} else if(ub>capacity && lb<=capacity) {
/*			remains[currentRowId]=0;
			int left=capacity;
			for(int i=0; i<tnum; i++) {
				alloc[i][currentRowId]=least_to_exe[i];
				left-=least_to_exe[i];
			}
			for(int i=0; i<tnum; i++) {
				int extra=MIN(left, most_to_exe[i]-least_to_exe[i]);
				alloc[i][currentRowId]+=extra;
				left-=extra;
				if(left==0)
					break;
			}
			for(int i=0; i<tnum; i++) {
				alloced_to_exe[i]+=alloc[i][currentRowId];
				laxity[i]=laxity[i]+alloc[i][currentRowId]-window;
				if(rend%period[i]==0) {
					if(alloced_to_exe[i]!=execute[i]) {
						cout<<"error: task"<<i<<" deadline miss..."<<endl;
						return -1;
					}
					alloced_to_exe[i]=0;
					laxity[i]=period[i]-execute[i];
					jstart[i]=currentRowId+1;
				}
			}*/
			
#if ALLOCATE_OPTIMIZE==0
			if(basicAssignCurrentExecution()==-1)
				return -1;
#elif ALLOCATE_OPTIMIZE==1
			if(newAssignCurrentExecution()==-1)
				return -1;
#elif ALLOCATE_OPTIMIZE==2
			if(minRemainsAllocate()==-1)
				return -1;
#elif ALLOCATE_OPTIMIZE==3
			if(maxAllocedAllocate()==-1)
				return -1;
#endif
		} else {
			int overflow=lb-capacity;
			remains[currentRowId]=0;
			for(int i=0; i<tnum; i++) {
				alloc[i][currentRowId]=least_to_exe[i];
				alloced_to_exe[i]+=alloc[i][currentRowId];
				laxity[i]=laxity[i]+alloc[i][currentRowId]-window;
			}
			if(totalEmpty>0) {
#ifdef DETAILBINGO
				cout<<"bingo"<<endl;
#endif
				for(int i=0; i<tnum; i++) {
					if(alloc[i][currentRowId]>0) {
						int toModify=searchEmptyDirectly(i, overflow);
						overflow=overflow-toModify;
						if(overflow==0)
							break;
					}
				}	
				int locflag;
				do{
					locflag=0;
					for(int i=0; i<tnum; i++) {
						while(alloc[i][currentRowId]>0 && overflow>0 && totalEmpty>0) {
							clearFlags();
							if(searchEmptyInDirectly(i, currentRowId)) {
								alloc[i][currentRowId]--;
								overflow--;
								locflag=1;
#ifdef DETAIL
								cout<<"["<<i<<", "<<currentRowId<<"]"<<endl;
#endif
/*								for(int j=0; j<rnum; j++) 
									cout<<rflag[j]<<" ";
								cout<<endl;*/
							}
							else {
/*								for(int j=0; j<rnum; j++) 
									cout<<rflag[j]<<" ";
								cout<<endl;*/
								break;
							}
						}
						if(overflow==0 || totalEmpty==0)
							break;
					}
				}while(locflag);
			}
			if(overflow>0) {
				for(int i=0; i<tnum; i++) {
					if(!isSrcTask(i))
						continue;
					int toModify=searchTaskDirectly(i, overflow);
					overflow=overflow-toModify;
					if(overflow==0)
						break;
				}
			}
			if(overflow>0) {
				for(int i=0; i<tnum; i++) {
					if(!isSrcTask(i))
						continue;
					clearFlags();
					while(searchTaskInDirectly(i, currentRowId)) {
#ifdef SEARCHTASK
						cout<<"["<<i<<", "<<currentRowId<<"]"<<endl;
#endif
						alloc[i][currentRowId]--;
						overflow--;
						clearFlags();
						if(overflow==0)
							break;
						if(alloc[i][currentRowId]==0)
							break;
					}
					if(overflow==0)
						break;
				}
			}
			if(overflow>0) {
				cout<<"error: something unexpected happened..."<<endl;
				return -1;
			}
			for(int i=0; i<tnum; i++) {
				if(rend%period[i]==0) {
					if(alloced_to_exe[i]!=execute[i]) {
						cout<<"error: task"<<i<<" deadline miss..."<<endl;
						return -1;
					}
					alloced_to_exe[i]=0;
					laxity[i]=period[i]-execute[i];
					jstart[i]=currentRowId+1;
				}	
			}
		}
#ifdef DISPLAY
		cout<<"currentRow is "<<currentRowId<<endl;
		for(int i=0; i<=currentRowId; i++) {
			cout<<"row"<<i<<":\t";
			for(int j=0; j<tnum; j++)
				cout<<alloc[j][i]<<" ";
			cout<<"remains="<<remains[i]<<endl;
		}
#endif
	}
	return 1;
}

int algo::checkSchedule() {
	for(int i=0; i<tnum; i++)
		for(int j=0; j<rnum; j++)
		{
			if(alloc[i][j]<0) {
				cout<<"error: alloc error number..."<<endl;
				return -1;
			}
		}
	int *sumExe;
	sumExe=(int*)malloc(sizeof(int)*tnum);
	for(int i=0; i<tnum; i++)
		sumExe[i]=0;
	for(int i=0; i<rnum; i++) {
		int tmpSum=0;
		for(int j=0; j<tnum; j++) {
			tmpSum+=alloc[j][i];
			if(alloc[j][i]>rwidth[i]) {
				cout<<"error: alloc exceed row width..."<<endl;
				return -1;
			}
		}
		int capacity=rwidth[i]*pnum;
		if(tmpSum>capacity) {
			cout<<"error: sum of a row exceed capacity..."<<endl;
			return -1;
		}
	}
	for(int i=0; i<rnum; i++) {
		for(int j=0; j<tnum; j++) {
			sumExe[j]+=alloc[j][i];
			if(b[i+1]%period[j]==0) {
				if(sumExe[j]!=execute[j]) {
					cout<<"task"<<j<<"sumExe="<<sumExe[j]<<", execute="<<execute[j]<<endl;
					cout<<"error: a task miss deadline..."<<endl;
					return -1;
				}
				sumExe[j]=0;
			}
		}
	}
	return 1;
}

void algo::allocate() {
	for(int i=0; i<rnum; i++) {
		int rstart=b[i];
		int rend=b[i+1];
		int window=b[i+1]-b[i];
		int count=0;
		for(int j=0; j<tnum; j++) {
			for(int k=0; k<alloc[j][i]; k++) {
				int locx=count/window;
				int locy=count%window+rstart;
				result[locx][locy]=j;
				count++;
			}
		}
	}
}

void algo::myAllocate() {
	for(int i=0; i<rnum; i++) {
		int rstart=b[i];
		int rend=b[i+1];
		int window=rend-rstart;
		int count=0;
		int bubbles=remains[i];
		for(int j=0; j<tnum; j++) {
			if(alloc[j][i]==0)
				continue;
			int fromP=count/window;
			int toP=(count+alloc[j][i]-1)/window;
			int toFill=window-count%window;
			if(fromP!=toP && toFill<=bubbles) {
				bubbles-=toFill;
				count+=toFill;
			}
			for(int k=0; k<alloc[j][i]; k++) {
				int locx=count/window;
				int locy=count%window+rstart;
				result[locx][locy]=j;
				count++;
			}
		}
	}
}

void algo::myOldAllocate() {

	myAllocate();


	int *to_exe, *selected, *time, *flag;
	to_exe=(int*)malloc(sizeof(int)*pnum);
	selected=(int*)malloc(sizeof(int)*pnum);
	time=(int*)malloc(sizeof(int)*tnum);
	flag=(int*)malloc(sizeof(int)*tnum);

	for(int i=0; i<tnum; i++) {
		flag[i]=-1;
		time[i]=-1;
	}

	for(int i=0; i<hp; i++) {
		int ind=0;
		for(int j=0; j<pnum; j++) {
			if(result[j][i]!=-1) {
				to_exe[ind]=result[j][i];
				ind++;
			}
		}
		for(int j=0; j<pnum; j++)
			result[j][i]=-1;
		for(int j=0; j<pnum; j++) 
			selected[j]=0;
		for(int j=0; j<pnum; j++) {
			int maxt=-1;
			int tid=-1;
			int kk;
			for(int k=0; k<ind; k++) {
				if((time[to_exe[k]]>maxt) && (flag[to_exe[k]]==j)) {
					maxt=time[to_exe[k]];
					tid=to_exe[k];
					kk=k;
				}
			}
			if(tid!=-1) {
				result[j][i]=tid;
				time[tid]=i;
				flag[tid]=j;
				selected[kk]=1;
			}
		}

		for(int j=0; j<pnum; j++) {
			if(result[j][i]==-1) {
				for(int k=0; k<ind; k++) {
					if(selected[k]==1)
						continue;
					result[j][i]=to_exe[k];
					time[to_exe[k]]=i;
					flag[to_exe[k]]=j;
					selected[k]=1;
					break;
				}
			}	
		}
	}

	free(flag);
	free(to_exe);
	free(selected);
	free(time);
}

float algo::countPreemption() {
	float res=0.0f;
#ifdef MANDP
	cout<<"preemption:"<<endl;
#endif
#ifdef STRIPTASK
	for(int i=0; i<tnum-1; i++) {
#else
	for(int i=0; i<tnum; i++) {
#endif
		int count=0;
		for(int j=0; j<hp/period[i]; j++) {
			int jstart=j*period[i];
			int jend=jstart+period[i];
			int executed=0;
			for(int k=jstart; k<jend; k++) {
				for(int m=0; m<pnum; m++) {
					if(result[m][jstart+k]==i) {
						executed++;
						if(executed==execute[i])
							continue;
						if(result[m][jstart+k+1]!=i)
							count++;
					}
				}
			}
		}
		float thisPreemption=(float)count/((float)(hp/period[i]));
#ifdef MANDP
		cout<<"task"<<i<<"\t"<<thisPreemption<<endl;
#endif
		res=res+thisPreemption;
	}
#ifdef STRIPTASK
	res=res/(float)(tnum-1);
#else
	res=res/(float)tnum;
#endif
	return res;
}

float algo::countMigration() {
	float res=0.0f;	
#ifdef MANDP
	cout<<"migration:"<<endl;
#endif
#ifdef STRIPTASK
	for(int i=0; i<tnum-1; i++) {
#else
	for(int i=0; i<tnum; i++) {
#endif
		int count=0;
		int loc=-1;
		for(int j=0; j<hp; j++) {
			for(int k=0; k<pnum; k++) {
				if(result[k][j]==i) {
					if(loc==-1)
						loc=k;
					else {
						if(k!=loc)
							count++;
						loc=k;
					}
				}
			}
		}
		float thisMigration=(float)count*(float)period[i]/(float)hp;
#ifdef MANDP
		cout<<"task"<<i<<"\t"<<thisMigration<<endl;
#endif
		res=res+thisMigration;
	}
#ifdef STRIPTASK
	res=res/(float)(tnum-1);
#else
	res=res/(float)tnum;
#endif
	return res;
}

void algo::displayResult() {
	for(int i=0; i<pnum; i++) {
		cout<<"p"<<i<<": ";
		for(int j=0; j<hp; j++) {
			cout<<result[i][j]<<" ";
		}
		cout<<endl;
	}
}
