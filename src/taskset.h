#include <fstream>
#include <stdlib.h>

#ifndef _TASKSET_H_

using std::ifstream;

class taskset {
	public:
		explicit taskset(ifstream&);

		int tnum, pnum, hp;
		int *execute, *period;
		int rnum;
		int *b, *rwidth;
};

#define _TASKSET_H_
#endif
