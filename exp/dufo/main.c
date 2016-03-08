#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>

#define _ASSERT

#define MIN(a , b) (((a)>(b))?(b):(a))
#define MAX(a , b) (((a)>(b))?(a):(b))

#ifdef _DEBUG
#define DP(fmt, arg...) \
do {\
    printf(fmt, ##arg);\
}while(0)
#else //_DEBUG
#define DP(fmt, arg...) do {\
}while(0)
#endif //_DEBUG

unsigned long long gcd_2(unsigned long long a, unsigned long long b)
{
    unsigned long long tmp;
    unsigned long long min;
    unsigned long long max;

    if ( a <= 0 || b <= 0 ) {
        DP("gcd_2 argument error (#Minus number or zero)!\n");
        exit(0);
    }

    if ( a==b ) {
        return a;
    }

    min = MIN(a, b); 
    max = MAX(a, b); 

    do{
        tmp = max%min;
        max = min;
        min = tmp;
    }while(tmp);

    return max; 
}

unsigned long long lcm_2(unsigned long long a, unsigned long long b)
{
    unsigned long long gcd;

    if (a <=0 || b <= 0) {
        DP("lcm_2: argument error (#Minus number or zero)!\n");
        exit(0);
    }

    if (a == b) {
        return a;
    }

    gcd = gcd_2(a, b);

    return (a/gcd) * (b/gcd) * gcd;
}

int lcm(int n, int* a, int bound)
{
    unsigned long long lcm;
    unsigned long long upper_bound;
    unsigned int i;

    if ( n <= 0 || a==NULL) {
        DP("argument error (#Minus number or zero)!\n");
        exit(0);
    }

    i   = 0;
    lcm = 1;

    upper_bound = bound;
    while((i<n) && (lcm<upper_bound)) {
        lcm = lcm_2(lcm, a[i++]);
    }

    if (lcm<upper_bound) {
        return lcm;
    } 
    else {
        return 0;
    }
}

void order(int num[], int n)
{
    int i;
    int j;

    for (i=0; i<(n-1); i++) {
        for (j=i+1; j<n; j++) {
            if (num[i] > num[j]) {
                num[i] ^= num[j];
                num[j] ^= num[i];
                num[i] ^= num[j];
            }
        }
    }
}

int min(int num[], int n)
{
    int i;
    int mininum;
    
    for (i=0, mininum=num[0]; i<n; i++) {
        if (num[i] < mininum) {
            mininum = num[i];
        }
    }

    return mininum;
}

int max(int num[], int n)
{
    int i;
    int maxinum;
    
    for (i=0, maxinum=num[0]; i<n; i++) {
        if (num[i] > maxinum) {
            maxinum = num[i];
        }
    }

    return maxinum;
}

#define MAX_PERIOD_LENTH    100
#define MAX_TASK_NUM        64
#define MAX_PROCESSOR_NUM   32
#define MAX_DURATION        300000

#if 1
#define NONE         "\033[m"
#define RED          "\033[0;32;31m"
#define LIGHT_RED    "\033[1;31m"
#define GREEN        "\033[0;32;32m"
#define LIGHT_GREEN  "\033[1;32m"
#define BLUE         "\033[0;32;34m"
#define LIGHT_BLUE   "\033[1;34m"
#define DARY_GRAY    "\033[1;30m"
#define CYAN         "\033[0;36m"
#define LIGHT_CYAN   "\033[1;36m"
#define PURPLE       "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN        "\033[0;33m"
#define YELLOW       "\033[1;33m"
#define LIGHT_GRAY   "\033[0;37m"
#define WHITE        "\033[1;37m"
#define END          "\033[0;m"
#endif //0

#define STACK_FRAME_SIZE 4
#define MAX_STACK_SIZE (STACK_FRAME_SIZE*MAX_DURATION)
int stack[MAX_STACK_SIZE];
int stack_pointer = 0;

void init_stack(void)
{
    int i;

    for(i=0; i<MAX_STACK_SIZE; i++) {
        stack[i] = 0;
    }

    stack_pointer = 0;
}

int stack_not_empty()
{
    return (stack_pointer != 0);
}

int push(int val)
{
    if (stack_pointer >= MAX_STACK_SIZE) {
        printf("Stack overflow");
        exit(0);
        return 0;
    } 

    stack[stack_pointer++] = val;
    return 1;
}

int pop()
{
    if (stack_pointer == 0) {
        printf("Stack underflow");
        exit(0);
        return -1;
    }

    return stack[--stack_pointer];
}

int stack_valid() 
{
    return (stack_pointer == 0);
}

#if 0
#define NUL 0
#define SRC 1
#define MID 2
#define DST 3

int color_flag[MAX_TASK_NUM][MAX_PERIOD_LENTH];
void init_search_entity()
{
    int i;
    int j;

    for (i=0; i<MAX_TASK_NUM; i++) {
        for (j=0; j<MAX_PERIOD_LENTH; j++) {
            color_flag[i][j] = NUL;
        }
    }
    init_stack();
}

#define SET_COLOR_FLAG(t, pos, c) \
    do{\
        push(color_flag[t][pos]);\
        \
        color_flag[t][pos] = c; \
    }while(0)

#define RSTR_COLOR_FLAG(t, pos) \
    do{\
        color_flag[t][pos] = pop(); \
    }while(0)

#define SET_COLOR_FLAG_SRC(src, pos) \
    SET_COLOR_FLAG(src, pos, SRC)

#define RSTR_COLOR_FLAG_SRC(src, pos) \
    RSTR_COLOR_FLAG(src, pos)

#define SET_COLOR_FLAG_MID(mid, pos) \
    SET_COLOR_FLAG(mid, pos, MID)

#define RSTR_COLOR_FLAG_MID(mid, pos) \
    RSTR_COLOR_FLAG(mid, pos)

#define SET_COLOR_FLAG_DST(dst, pos) \
    SET_COLOR_FLAG(dst, pos, DST)

#define RSTR_COLOR_FLAG_DST(dst, pos) \
    RSTR_COLOR_FLAG(dst, pos)
#endif //0


int true_task_num;
int task_num;
int processor_num;
int lcm_period;
int cur_to_exe[MAX_TASK_NUM];
int total_to_exe[MAX_TASK_NUM];
int laxity[MAX_TASK_NUM];
int execute[MAX_TASK_NUM];
int period[MAX_TASK_NUM];
int execute_flag[MAX_TASK_NUM][MAX_DURATION];
int alloc[MAX_PROCESSOR_NUM][MAX_DURATION];

//int *preemption, *migration, *event;

FILE *fin, *flog;

void disp_task(int cur, int next)
{
    int i;
    int j;

    printf("\n");
    for (i=0; i<task_num; i++) {
        printf("task: %2d(%3d/%3d): ", i, execute[i], period[i]);
        for (j=cur; j<next; j++) {
            if(!(j%period[i])) {
                    printf(PURPLE"|"END);
            } else {
                printf(" ");
            }

            printf("%d", execute_flag[i][j]);

            /*
            switch (color_flag[i][j-cur]) {
                case NUL: printf("%d", execute_flag[i][j]); break;
                case SRC: printf(RED"%d"END, execute_flag[i][j]); break;
                case MID: printf(YELLOW"%d"END, execute_flag[i][j]); break;
                case DST: printf(BLUE"%d"END, execute_flag[i][j]); break;
                default:  printf("%d", execute_flag[i][j]); break;
            }
            */
        }
        printf(" cur: %d remain: %d laxity: %d", cur_to_exe[i], total_to_exe[i], laxity[i]);
        printf("\n");
    }
}

int next_deadline(int cur)
{
    int i;

    do{
        cur++;
        for(i=0; i<task_num; i++) {
            if(!(cur%period[i])) {
                break;
            }
        }
    }while (i==task_num);

    return cur;
}

int check_scheduling() 
{
    int i;
    int j;

    int capacity;
    int task_exe[MAX_TASK_NUM];

    for (i=0; i<task_num; i++) {
        task_exe[i] = 0;
    }

    for (i=0; i<lcm_period; i++) {
        for (j=0, capacity=0; j<task_num; capacity+=execute_flag[j][i], task_exe[j]+=execute_flag[j][i], j++); 

        for (j=0; j<task_num; j++) {
            if ((i+1)%period[j]==0) {
                if (task_exe[j]!=execute[j]) {
                    printf("Invalid Schedule(Task execution time error). Time: %d Task: %d\n", i, j);
										exit(1);
                    return i;
                }
                else {
                    task_exe[j] = 0;
                }
            }
        }

        if (capacity!=processor_num) {
            printf("Invalid Schduleing(Processor capacity error). Time:%d\n", i);
						exit(1);
            return i;
        }
    }

    return lcm_period;
}

#define JOB_RELEASE_TIME(task, time) ((time)/period[(task)]*period[(task)])
#define JOB_DEADLINE_TIME(task, time) (JOB_RELEASE_TIME(task, time)+period[(task)])
#define TASK_CUR_JOB_POS(task, time) ((time)/period[(task)])

void wind_mc(int cur, int next, int task_num, int cur_to_exe[], int processor_num)
{
    int i;
    int j;
    int execution;

    for (i=0; i<task_num; i++) {
        for (j=cur; j<next; j++) {
            execute_flag[i][j] = 0;
        }
    }

    j = cur;
    for (i=0; i<task_num; i++) {
        execution = cur_to_exe[i];
        while (execution-->0) {
            execute_flag[i][j++] = 1;
            if (j==next) {
                j = cur;
            }
        }
    }

    //checking

    for (j=cur; j<next; j++) {
        execution = 0;
        for (i=0; i<task_num; i++, execution+=execute_flag[i][j]); 
        if (execution != processor_num) {
            break;
            printf("window scheduling error!\n");
            exit(0);
        }
    }

    for (i=0; i<task_num; i++) {
        execution = 0;
        for (j=cur; j<next; j++, execution += execute_flag[i][j]); 
        if (execution != cur_to_exe[i]) {
            break;
            printf("window scheduling error!\n");
            exit(0);
        }
    }
}

int zone_flag[MAX_DURATION];
int time_to_zone[MAX_DURATION];
int zone_start[MAX_DURATION+1];

void winds_mc(int lcm_period, int task_num, int processor_num)
{
    
    int i;
    int j;
    int cur;
    int next;
    int zone_index;
    int execution[MAX_TASK_NUM];

    cur = 0;
    zone_index = 0;

    while (cur<lcm_period) {
        next = zone_start[++zone_index];

        for (i=0; i<task_num; i++) {
            execution[i] = 0;
            for (j=cur; j<next; j++){
                execution[i] += execute_flag[i][j];
            }
        }

        wind_mc(cur, next, task_num, execution, processor_num);

        cur  = next;
    }
}

void wind_llref(int begin, int end, int task_num, int cur_to_exe[], int processor_num)
{
    int wind_task_id[MAX_TASK_NUM];
    int wind_task_num = 0;
    int wind_total_to_exe[MAX_TASK_NUM];
    int wind_task_exe_flag[MAX_TASK_NUM];
    int wind_task_laxity[MAX_TASK_NUM];
    int wind_cur_capacity = processor_num;
    int i;
    int j;

    for(i=0, j=0; i<task_num; i++) {
        if (cur_to_exe[i]) {
            wind_task_id[j] = i;
            wind_total_to_exe[j] = cur_to_exe[i];
            wind_task_laxity[j] = end-begin-cur_to_exe[i];
            j++;
        }
    }

    wind_task_num = j;

    for(i=begin; i<end; i++) {
        wind_cur_capacity = processor_num;
        for(j=0; j<wind_task_num; j++) {
            wind_task_exe_flag[j] = 0;
        }

        for(j=0; (j<wind_task_num)&&(wind_cur_capacity>0); j++) {
            if (wind_total_to_exe[j] == 0) {
                continue;
            }

            if (wind_task_laxity[j] == 0) {
//                wind_total_to_exe[j] -= 1;
                wind_cur_capacity -= 1;
                wind_task_exe_flag[j] = 1;
//                printf("tick:(%d-%d) task:%d  ", i, i+1, wind_task_id[j]);
            }
        }

        for(j=0; (j<wind_task_num)&&(wind_cur_capacity>0); j++) {
            if (wind_total_to_exe[j] == 0) {
                continue;
            }

            if (wind_task_laxity[j] > 0) {
//                wind_total_to_exe[j] -= 1;
                wind_cur_capacity -= 1;
                wind_task_exe_flag[j] = 1;
//                printf("tick:(%d-%d) task:%d  ", i, i+1, wind_task_id[j]);
            }
        }

        for(j=0; j<wind_task_num; j++) {
            if (wind_total_to_exe[j] == 0) {
                continue;
            }

            if (!wind_task_exe_flag[j]) {
                wind_task_laxity[j] -= 1;
            }
            else {
                wind_total_to_exe[j] -= 1;
            }
        }

        /*
         * set the exection flag of task set in current window
         */
        DP("tick: [%d-%d) tasks:", i, i+1);
        for(j=0; j<wind_task_num; j++) {
            execute_flag[wind_task_id[j]][i] = wind_task_exe_flag[j];
            if (wind_task_exe_flag[j]) {
                DP("\t%d", wind_task_id[j]);
            }
        }
        DP("\n");
        /*
        for(j=0; j<wind_task_num; j++) {
            execute_flag[wind_task_id[j]][i] = wind_task_exe_flag[j];
            if (wind_task_exe_flag[j]) {
                printf("tick:[%d-%d) task:%d\n", i, i+1, wind_task_id[j]);
            }
        }
        */
    }

    return;
}

#define DISP_TASK_INFO(i) \
do {\
    DP("task %d(%d/%d)\t: least: %d, most: %d, cur: %d, left: %d, laxity: %d\n", \
            i, execute[i], period[i], least_to_exe[i], most_to_exe[i], cur_to_exe[i], total_to_exe[i], laxity[i]);\
} while(0)

/*
 * if "cur_capacity>0", it is sure that "cur_to_exe[i]==WINDOW" when "total_to_exe[i]>0"
 * "laxity[i]<period[i]-execute[i]" means that there is "__" after JOB_RELEASE_TIME(i, cur)
 */
#define WINDOW (next - cur)
/*
 * full version LESS_SRC_EXCHGBL(i):
 * 1. (cur_to_exe[i] < WINDOW)&&        this imply 3, which means that task has finished
 * 2. (cur_to_exe[i]<execute[i])&&      this means cur job also executed some where else, 
 *                                      together with 1, it can only before cur
 * 3. (total_to_exe[i]==0)&&            this implied by 1 
 * 4. (JOB_RELEASE_TIME(i, cur)<cur)    this means that task released before cur, can be 
 *                                      implied by 1&&2
 *  so, all in all, only 1&&2 is enough
 */
#define LESS_SRC_EXCHGBL(i) ((cur_to_exe[i]<WINDOW)&&(cur_to_exe[i]<execute[i]))
int less_src_exchgbl(int i, int cur, int next)
{
    return LESS_SRC_EXCHGBL(i);
}

/*
 * 1. (total_to_exe[i] > 0)&&               this imply 3, and means task has not finished
 * 2. (laxity[i]<(period[i]-execute[i]))&&  this means that cur job released before cur 
 * 3. (cur_to_exe[i] ==WINDOW)&&            this is implied by 1
 * 4. (JOB_RELEASE_TIME(i, cur)<cur)        this is implied by 2 
 *
 * so, all in all, only 1&&2 is enough
 */ 
#define LESS_DST_EXCHGBL(i) ((total_to_exe[i]>0)&&(laxity[i]<(period[i]-execute[i])))
int less_dst_exchgbl(int i, int cur, int next)
{
    return LESS_DST_EXCHGBL(i);
}

/*
 * Q1: the src and dst must exist (schedulable theory)
 * Q2: the path that conncet src and dst must exist (graph theory)
 */


/*
 * when "cur_capacity<0", it's sure that "cur_to_exe[i]==0" when "laxity[i] > 0"
 * so, if total_to_exe[i]<execute[i], it means that the job is executed before "cur"
 * and of course the job is release before cur
 *
 * 1. (laxity[i]>0)&&                       this means that time has not arrived deadline.
 *                                          and also has enough laxity, so cur_to_exe[i]==0
 * 2. (cur_to_exe[i]==0)&&                  this' implied by 1 
 *
 * 3. (JOB_RELEASE_TIME(i, cur)<cur)&&      this means that the job released before cur, imp
 *                                          -lied by 4
 * 4. (total_to_exe[i]<execute[i])          because cur_to_exe[i]==0, so this means that job
 *                                          begin to execute before current, which implies 3
 *                                          so, there is "1" that can be exchanged
 */
#define MORE_SRC_EXCHGBL(i) ((laxity[i]>0)&&(total_to_exe[i]<execute[i]))
int more_src_exchgbl(int i, int cur, int next)
{
    return MORE_SRC_EXCHGBL(i);
}

/*
 * if laxity[i]==0, there are two cases: 1) cur_to_exe[i]=0; 2) cur_to_exe[i]>0. 
 * so if cur_to_exe[i]>0, then it's sure that laxity[i]==0
 * the task should be executed before cur.
 *
 * 1. (laxity[i] == 0)&&                             this means that there is no laxity after next
 *                                                   impled by 2.
 * 2. (cur_to_exe[i] > 0)&&                          this imply 1.
 * 3. (JOB_RELEASE_TIME(i, cur)<cur)&&               this means that the job is released before cur
 * 4. (WINDOW-cur_to_exe[i])<(period[i]-execute[i])  this means that there is "0" before cure, imply 3
 */
#define MORE_DST_EXCHGBL(i) ((cur_to_exe[i]>0)&&((WINDOW-cur_to_exe[i])<(period[i]-execute[i])))
int more_dst_exchgbl(int i, int cur, int next)
{
    return MORE_DST_EXCHGBL(i);
}

int less_exchg_prcs(int s, int d, int cur_capacity)
{
    cur_to_exe[s]  += 1;
    total_to_exe[d]-= 1;
    laxity[d]      += 1;
    cur_capacity   -= 1;

#ifdef _ASSERT
    assert((cur_to_exe[s]>=0)&&(cur_to_exe[s]<=execute[s]));
    assert((total_to_exe[d]>=0)&&(total_to_exe[d]<=execute[d]));
    assert((laxity[d]>=0)&&(laxity[d]<=(period[d]-execute[d])));
#endif //_ASSERT

    return cur_capacity;
}

int more_exchg_prcs(int s, int d, int cur_capacity)
{
    cur_to_exe[d]  -= 1;
    total_to_exe[s]+= 1;
    laxity[s]      -= 1;
    cur_capacity   += 1;

#ifdef _ASSERT
    assert((cur_to_exe[d]>=0)&&(cur_to_exe[d]<=execute[d]));
    assert((total_to_exe[s]>=0)&&(total_to_exe[s]<=execute[s]));
    assert((laxity[s]>=0)&&(laxity[s]<=(period[s]-execute[s])));
#endif //_ASSERT

    return cur_capacity;
}

typedef int (*src_exchgbl_func)(int i, int cur, int next);
typedef int (*dst_exchgbl_func)(int i, int cur, int next);
typedef int (*exchg_prcs_func)(int s, int d, int cur_capacity);

/*
 * job_flag here is used to incur if the job is 
 * searched before, if so, then we should not
 * search via it again.
 */


void clear_zone_flag(void)
{
    int j;

    for (j=0; j<MAX_DURATION; j++) {
        zone_flag[j] = 0;
    }
}

int chk_zone_flag(int j)
{
    return zone_flag[j];
}

void config_zone_flag(int j, int value)
{
    if (value != 0) {
        zone_flag[j] = 1;
    }
    else {
        zone_flag[j] = 0;
    }
}


void store_task_set(int);
void _exechange_execution_zone(int src, int dst, int zone_begin, int zone_end)
{
    int j;

    int mid;
    int mid_s;
    int mid_d;

    for (j=zone_begin; j<zone_end; j++){
        if ((execute_flag[src][j]==1) &&
            (execute_flag[dst][j]==0)) {
            execute_flag[src][j] = 0;
            execute_flag[dst][j] = 1;
            return;
        }
    }

    for (j=zone_begin; j<zone_end; j++) {
        if (execute_flag[src][j]==1) {
            mid_s = j;
            break;
        }
    }

    for (j=zone_begin; j<zone_end; j++) {
        if (execute_flag[dst][j]==0) {
            mid_d = j;
            break;
        }
    }

    for (mid=0; mid<task_num; mid++) {
        if (mid==src || mid==dst) {
            continue;
        }

        if ((execute_flag[mid][mid_s]==0)&&
            (execute_flag[mid][mid_d]==1)) {
            execute_flag[src][mid_s] = 0;
            execute_flag[mid][mid_s] = 1;
            execute_flag[mid][mid_d] = 0;
            execute_flag[dst][mid_d] = 1;
            return;
        }
    }

    printf("_exechange_execution_zone error!\n");
    store_task_set(2);
    exit(0);
}

int exchange_execution_zone_no_recur(int src, int src_zone, int cur, int next, 
                               src_exchgbl_func src_exchgbl, dst_exchgbl_func dst_exchgbl)
{
    int j;
    int dst;
    int mid;
    int ret;

    int cur_zone;
    int cur_task;
    int mid_zone;
    int zone_begin;
    int zone_end;
    int arrival;
    int deadline;

    cur_task = src;
    cur_zone = src_zone;

    init_stack();

in_recur:
    zone_begin = zone_start[cur_zone];
    zone_end   = zone_start[cur_zone+1]; // last zone lcm

    // search for the destination job first
    for (dst=0; dst<task_num; dst++) {
        if ((cur_task == dst)||
                (!dst_exchgbl(dst, cur, next))||
                (JOB_RELEASE_TIME(dst, zone_begin) != JOB_RELEASE_TIME(dst, cur))){
            continue;
        }

        for (j=zone_begin; j<zone_end; j++) {
            if (execute_flag[dst][j]==0) {
                _exechange_execution_zone(cur_task, dst, zone_begin, zone_end);

                while(stack_not_empty()){

                    cur_zone = pop();
                    cur_task = pop();
                    mid      = pop();
                    j        = pop();

                    zone_begin = zone_start[cur_zone];
                    zone_end   = zone_start[cur_zone+1]; // last zone lcm

                    _exechange_execution_zone(cur_task, mid, zone_begin, zone_end);
                }

                return dst;
            }
        }
    }

    // if we failed to find the destination, find a middle one
    for (mid=0; mid<task_num; mid++) {
        if (cur_task == mid) {
            continue;
        }

        for (j=zone_begin; j<zone_end; j++) {
            if (execute_flag[mid][j]==0) {
                break;
            }
        }
        if (j==zone_end) {
            continue;
        }

        arrival = JOB_RELEASE_TIME(mid, zone_begin);
        deadline = MIN(JOB_DEADLINE_TIME(mid, zone_begin), cur);

        for (j=arrival; j<deadline; j++) {
            // a zone may have no destination, left dirty , for directly
            // a zone may be already accessed, but not viable, left dirty , for indirectly
            if (zone_flag[time_to_zone[j]]!=0) {
                j = zone_start[time_to_zone[j]+1]-1;
                continue;
            }

            if (execute_flag[mid][j]==0) { 
                continue;
            }

            push(j);
            push(mid);
            push(cur_task);
            push(cur_zone);

            cur_task = mid;
            cur_zone = time_to_zone[j];
            zone_flag[cur_zone] = 1;

            goto in_recur;

out_recur:
            do{}while(0);
        }
    } 

    if (!stack_not_empty()) {
        return -1;
    }

    cur_zone = pop();
    cur_task = pop();
    mid      = pop();
    j        = pop();

    zone_begin = zone_start[cur_zone];
    zone_end   = zone_start[cur_zone+1]; // last zone lcm

    arrival = JOB_RELEASE_TIME(mid, zone_begin);
    deadline = MIN(JOB_DEADLINE_TIME(mid, zone_begin), cur);
    goto  out_recur;

}

int exchange_execution_zone(int src, int src_zone, int cur, int next,
                            src_exchgbl_func src_exchgbl, dst_exchgbl_func dst_exchgbl) 
{
    int j;
    int dst;
    int mid;
    int ret;

    int cur_zone;
    int mid_zone;
    int zone_begin;
    int zone_end;

    cur_zone   = src_zone;
    zone_begin = zone_start[cur_zone];
    zone_end   = zone_start[cur_zone+1]; // last zone lcm


    // search for the destination job first
    for (dst=0; dst<task_num; dst++) {
        if ((src == dst)||
            (!dst_exchgbl(dst, cur, next))||
            (JOB_RELEASE_TIME(dst, zone_begin) != JOB_RELEASE_TIME(dst, cur))){
            continue;
        }

        for (j=zone_begin; j<zone_end; j++) {
            if (execute_flag[dst][j]==0) {
                _exechange_execution_zone(src, dst, zone_begin, zone_end);
                return dst;
            }
        }
    }

    for (mid=0; mid<task_num; mid++) {
        if (src == mid) {
            continue;
        }

        for (j=zone_begin; j<zone_end; j++) {
            if (execute_flag[mid][j]==0) {
                break;
            }
        }
        if (j==zone_end) {
            continue;
        }

        int arrival;
        int deadline;
        arrival = JOB_RELEASE_TIME(mid, zone_begin);
        deadline = MIN(JOB_DEADLINE_TIME(mid, zone_begin), cur);

        for (j=arrival; j<deadline; j++) {
            if ((execute_flag[mid][j]==1) &&
                (zone_flag[time_to_zone[j]]==0)) {
                
                mid_zone = time_to_zone[j];
                zone_flag[mid_zone] = 1;
                ret = exchange_execution_zone(mid, mid_zone, cur, next,
                                                 src_exchgbl, dst_exchgbl); 
                if (ret>=0) {
                    _exechange_execution_zone(src, mid, zone_begin, zone_end);
                    return ret;
                }
            }
        }
    }

    return -1;
}

int search_src_zone(int task_set_src[], int src_num, int cur, int next, src_exchgbl_func src_exchgbl,
                    int* src)
{
    int i;
    int j;
    int s;
    int arrival;
    int deadline;

    if (src_num<=0){
        printf("number of source job error!\n");
        store_task_set(2);
        exit(0);
    }
    
    for (i=0; i<src_num; i++) {
        //the element in task_set_src[] may become invalid after last tuning
        s = task_set_src[i];
        if (!src_exchgbl(s, cur, next)) { 
            continue;
        }

        arrival = JOB_RELEASE_TIME(s, cur);
        deadline= MIN(JOB_DEADLINE_TIME(s, cur), cur);

        for (j=arrival; j<deadline; j++)
        {
            // a zone may have no destination, left dirty , for directly
            // a zone may be already accessed, but not viable, left dirty , for indirectly
            if (zone_flag[time_to_zone[j]]!=0) {
                j = zone_start[time_to_zone[j]+1]-1;
                continue;
            }

            if (execute_flag[s][j]==0) { 
                continue;
            }

            *src = s;
            return time_to_zone[j];
        }
    }

    *src = -1;
    return -1;

}

int search_zone_dst(int task_set_dst[], int dst_num, int cur, int next, dst_exchgbl_func dst_exchgbl,
                    int zone)
{
    int i;
    int j;
    int d;

    if (dst_num<=0){
        printf("number of destination job error!\n");
        store_task_set(2);
        exit(0);
    }

    int zone_begin;
    int zone_end;
    zone_begin = zone_start[zone];
    zone_end   = zone_start[zone+1];

    for (i=0; i<dst_num; i++) {
        d = task_set_dst[i];

        if ((!dst_exchgbl(d, cur, next))||
            (JOB_RELEASE_TIME(d, zone_begin) != JOB_RELEASE_TIME(d, cur))){
            continue;
        }

        for (j=zone_begin; j<zone_end; j++) {
            if (execute_flag[d][j]==0) {
                return d;
            }
        }
    }

    return -1; //no destination zone found
}

int exchange_execution_zone_directly(int cur, int next, int cur_capacity,
    src_exchgbl_func src_exchgbl, dst_exchgbl_func dst_exchgbl, exchg_prcs_func exchg_prcs)
{

    int i;
    int j;
    int s;
    int d;
    int src;
    int dst;
    int src_zone;
    int __counter;

    int task_set_src[MAX_TASK_NUM];
    int task_set_dst[MAX_TASK_NUM];

    for (i=0, src=0, dst=0; i<task_num; i++) {
        if (src_exchgbl(i, cur, next)) {
            task_set_src[src++] = i; 
        }

        if (dst_exchgbl(i, cur, next)) {
            task_set_dst[dst++] = i;
        }
    }

    clear_zone_flag();
//    init_search_entity();

    __counter = 0;
    while(cur_capacity!=0) {
        // no source zone found because of all are flagged by search_src_zone(...)
        if ((src_zone = search_src_zone(task_set_src, src, cur, next, src_exchgbl, &s))<0) {
            return cur_capacity; 
        }

        if ((d = search_zone_dst(task_set_dst, dst, cur, next, dst_exchgbl, src_zone))<0){
            zone_flag[src_zone] = 1; //flag it to never search it again for search_src_zone(...);
            continue;
        }

        int zone_begin;
        int zone_end;

        zone_begin = zone_start[src_zone];
        zone_end   = zone_start[src_zone+1]; // last zone lcm
        _exechange_execution_zone(s, d, zone_begin, zone_end);
        cur_capacity = exchg_prcs(s, d, cur_capacity);

        /*
        if (++__counter>=(WINDOW*processor_num)) {
            printf("Exchange error!\n");
            store_task_set(2);
            exit(0);
        }
        */
    }

    return cur_capacity;
}

int exchange_execution_zone_indirectly(int cur, int next, int cur_capacity,
    src_exchgbl_func src_exchgbl, dst_exchgbl_func dst_exchgbl, exchg_prcs_func exchg_prcs)
{

    int i;
    int j;
    int s;
    int d;
    int src;
    int dst;
    int src_zone;
    int __counter;

    int task_set_src[MAX_TASK_NUM];
    int task_set_dst[MAX_TASK_NUM];

    for (i=0, src=0, dst=0; i<task_num; i++) {
        if (src_exchgbl(i, cur, next)) {
            task_set_src[src++] = i; 
        }

        if (dst_exchgbl(i, cur, next)) {
            task_set_dst[dst++] = i;
        }
    }

    clear_zone_flag();

    __counter = 0;
    while(cur_capacity!=0) {
        if ((src_zone = search_src_zone(task_set_src, src, cur, next, src_exchgbl, &s))<0) {
            printf("No source zone!\n");
            store_task_set(2);
            exit(0);
        }

        zone_flag[src_zone] = 1;

        //d = exchange_execution_zone(s, src_zone, cur, next, src_exchgbl, dst_exchgbl); 
        d = exchange_execution_zone_no_recur(s, src_zone, cur, next, src_exchgbl, dst_exchgbl); 
#ifdef _ASSERT
        assert(stack_valid());
#endif //_ASSERT

        if (d>=0) {
            cur_capacity = exchg_prcs(s, d, cur_capacity);
            clear_zone_flag();

            /*
            if (++__counter>=(WINDOW*processor_num)) {
                printf("Exchange error!\n");
                store_task_set(2);
                exit(0);
            }
            */
        }
    } 

    return cur_capacity;
}

int assigne_cur_execution(int cur, int pre, int cur_capacity, int least_to_exe[], int most_to_exe[])
{
    int DR_used[MAX_TASK_NUM];
    int DR_id[MAX_TASK_NUM];
    int DR_tmp, DR_to_exe;
    int i;
    int j;

    for(i=0; i<task_num; i++)
    {
        DR_used[i]=0;
        DR_id[i]=i;
        for(j=pre; j<cur; j++)
        {
            DR_used[i]+=execute_flag[i][j];
        }
    }


    for(i=0; i<task_num-1; i++) {
        for(j=i+1; j<task_num; j++)
        {
            if(DR_used[i]<DR_used[j])
            {
                DR_tmp=DR_id[i];
                DR_id[i]=DR_id[j];
                DR_id[j]=DR_tmp;
                DR_tmp=DR_used[i];
                DR_used[i]=DR_used[j];
                DR_used[j]=DR_tmp;
            }
        }
    }

    if(cur_capacity>0)
    {
        for(i=0; i<task_num; i++)
        {
            DR_to_exe=most_to_exe[DR_id[i]]-cur_to_exe[DR_id[i]];
            if(DR_to_exe>cur_capacity)
            {
                DR_to_exe=cur_capacity;
            }

            cur_capacity          -=DR_to_exe;
            cur_to_exe[DR_id[i]]  +=DR_to_exe;
            total_to_exe[DR_id[i]]-=DR_to_exe;
            laxity[DR_id[i]]      +=DR_to_exe;

            if(cur_capacity==0)
                break;
        }
    }

    return cur_capacity;
}


int schedule_laxity_hierarchy()
{
    int i;
    int j;
    int cur;
    int next;
    int pre;
    int cur_capacity;
    int old_capacity;

    int least_to_exe[MAX_TASK_NUM];
    int most_to_exe[MAX_TASK_NUM];

    for(i=0; i<MAX_TASK_NUM; i++) {
        total_to_exe[i] = 0;
        least_to_exe[i] = 0;
        most_to_exe[i]  = 0;
        cur_to_exe[i]   = 0;
        laxity[i]       = 0;
    }

    /*
     * clear the execute_flag of the whole task set;
     */
    for(i=0; i<task_num; i++) {
        for(j=0; j<lcm_period; j++) {
            execute_flag[i][j] = 0;
        }
    }

    cur = 0;
    int zone_index = 0;

    while (cur<lcm_period) {
        next = next_deadline(cur);

        zone_start[zone_index] = cur;
        for (j=cur; j<next; j++) {
            time_to_zone[j]=zone_index;
        }

        /*
         * printf("cur: %d next: %d, zone_start[%d]=%d\n", cur, next, zone_index, zone_start[zone_index]);
         */
        zone_index++;
        cur = next;
    }
    zone_start[zone_index]=cur;

    cur = 0;
    pre = 0;
    zone_index = 0;
    while (cur<lcm_period) {
#if 0
        next = next_deadline(cur);
#else
        next = zone_start[++zone_index];
#endif 
////        printf("cur: %d next: %d percentage: %4.2f\n", cur, next, (float)next/(float)lcm_period*100);
        cur_capacity = WINDOW*processor_num;
        DP("time: [%d--%d) capacity: %d\n", cur, next, cur_capacity);

        /*
         * to check if we need a fresh total_to_exe again
         */
        for (i=0; i<task_num; i++) {
            if (!(cur%period[i])) {
                if (total_to_exe[i]>0) {
                    DP("Deadline miss! Task: %d(%d/%d)\n", i, execute[i], period[i]);
                    return cur;
                }
                else {
                    total_to_exe[i] = execute[i];
                    laxity[i] = period[i] - execute[i];
                }
            }
        }

        /* 
         * here may be the xxx
         */
        for (i=0; i<task_num; i++) {
            least_to_exe[i] = MAX(0, WINDOW-laxity[i]);
            most_to_exe[i]  = MIN(total_to_exe[i], WINDOW);

            laxity[i]      -= WINDOW; 
            /*
             * maybe negtive number, but that's ok because it's tmperary
             */
        }

        /*
         * step 0: schedule the jobs with least execution
         */
        for (i=0; i<task_num; i++) {
            cur_to_exe[i]   = least_to_exe[i];
            total_to_exe[i]-= least_to_exe[i];
            laxity[i]      += least_to_exe[i];
            cur_capacity   -= least_to_exe[i];
        }

        //case 1: the sum of least execution time is more than current capacity
        /*
         * step 1: search the exchangable zone quickly
         */
        if (cur_capacity < 0) {
            DP("Capacity Overflow! Can not schedule the task set!\n");
            for (i=0; i<task_num; i++) {
                DISP_TASK_INFO(i);
            }

            cur_capacity = exchange_execution_zone_directly(cur, next, cur_capacity, 
                    more_src_exchgbl, more_dst_exchgbl, more_exchg_prcs);
        }

        /*
         * step 2: search the exchangable bits via middle zone 
         */
        if (cur_capacity < 0) {
            DP("Capacity Overflow! Can not schedule the task set!\n");
            for (i=0; i<task_num; i++) {
                DISP_TASK_INFO(i);
            }

            cur_capacity = exchange_execution_zone_indirectly(cur, next, cur_capacity, 
                    more_src_exchgbl, more_dst_exchgbl, more_exchg_prcs);
        }

        if (cur_capacity < 0) {
            return cur;
        }

        // case 3: the sum of least execution time is less than current capacity
        //     and the sum of most  execution time is more/less than current capacity
#if 1
        cur_capacity = assigne_cur_execution(cur, pre, cur_capacity, least_to_exe, most_to_exe);
#else //0
        int min_laxity;
        int max_laxity;
        int cur_laxity;

        min_laxity = min(laxity, task_num);
        max_laxity = max(laxity, task_num);
        cur_laxity = min_laxity;

        /*
         * step 1: schedule the jobs according to the laxity
         */
        old_capacity = cur_capacity;
        while (cur_capacity>0) {
            for (i=0; (i<task_num)&&(cur_capacity>0); i++) {

                if ((laxity[i] == cur_laxity)&&(cur_capacity>0)&&(most_to_exe[i]-cur_to_exe[i]>0)) {
                    cur_to_exe[i]  += 1;
                    total_to_exe[i]-= 1;
                    laxity[i]      += 1;
                    cur_capacity   -= 1;
                }
            }

            /*
             * no extra load added when all of the tasks are scanned
             */
            if ((old_capacity==cur_capacity)&&(cur_laxity>=max_laxity)) {
                break;
            }

            old_capacity = cur_capacity;
            cur_laxity++;
        }
#endif //0

        /*
         * for (i=0; i<task_num; i++) {
         * total_to_exe[i] -= cur_to_exe[i];
         * } 
         */

        // case 2: the sum of most execution time is less than current capacity
        /*
         * step 1: search the exchangable zone quickly
         */
        if (cur_capacity>0) {
            DP("Warnning! Capacity not fully used!\n");

            for (i=0; i<task_num; i++) {
                DISP_TASK_INFO(i);
            }

            cur_capacity = exchange_execution_zone_directly(cur, next,  cur_capacity,
                    less_src_exchgbl, less_dst_exchgbl, less_exchg_prcs);
        }

        /*
         * step 2: search the exchangable bits via middle zones
         */
        if (cur_capacity>0) {
            DP("Warnning! Capacity not fully used!\n");

            for (i=0; i<task_num; i++) {
                DISP_TASK_INFO(i);
            }

            cur_capacity = exchange_execution_zone_indirectly(cur, next,  cur_capacity,
                    less_src_exchgbl, less_dst_exchgbl, less_exchg_prcs);
        }

        if (cur_capacity>0) {
            return cur;
        }

        for (i=0; i<task_num; i++) {
            DISP_TASK_INFO(i);
        }

//        wind_llref(cur, next, task_num, cur_to_exe, processor_num);
        wind_mc(cur, next, task_num, cur_to_exe, processor_num);

        cur = next;
        pre = cur;
        DP("\n");
    }

    winds_mc(lcm_period, task_num, processor_num);

    return cur;
}


int get_rand(int mod)
{
    int ret;

    /*
     * 1...(mod-1)
     */
    while(!(ret = rand()%mod));

    return ret;
}

void generate_task_set(int bound)
{
    int i;
    int capacity = 0;
    struct timeval ts;

    gettimeofday(&ts, NULL);
    srand(ts.tv_usec);
//    srand((int)time(0)*100);

    do {
        task_num = get_rand(MAX_TASK_NUM-1);

        for(i=0; i<task_num; i++) {
            period[i] = get_rand(MAX_PERIOD_LENTH)+1;
            execute[i] = get_rand(period[i]);
            //while((execute[i] = get_rand(MAX_PERIOD_LENTH))>=period[i]);
        }

        lcm_period = lcm(task_num, period, bound);
    }while (lcm_period==0);

    for(i=0; i<task_num; i++) {
        capacity += lcm_period/period[i]*execute[i];
    }


    if (capacity%(lcm_period)) {
        processor_num = capacity/lcm_period+1;
        period[task_num] = lcm_period;
        execute[task_num] = lcm_period*processor_num - capacity;
        
        int tmp = gcd_2(period[task_num], execute[task_num]);
        period[task_num] /= tmp;
        execute[task_num] /= tmp;

        task_num += 1;
    }
    else {
        processor_num = capacity/(lcm_period);
    }

    return ; 
}

void show_task_set()
{
    int i;

    printf("Task_num: %d  Processor_num: %d  LCM: %d\n", task_num, processor_num, lcm_period);
    for (i=0; i<task_num; i++) {
        printf("%4d (%d) ", execute[i], lcm_period/period[i]*execute[i]);
    }
    printf("\n");

    for (i=0; i<task_num; i++) {
        printf("%4d (%d) ", period[i], lcm_period);
    }
    printf("\n");

    int tmp;
    tmp = lcm_period/period[0]*execute[0];
    printf("%d", lcm_period/period[0]*execute[0]);
    for (i=1; i<task_num; i++) {
        tmp += lcm_period/period[i]*execute[i];
        printf(" + %d", lcm_period/period[i]*execute[i]);
    }
    printf(" = %d\n\n", tmp);
}

void store_task_set(int choice)
{
    int i;

    FILE *fp;

    switch (choice) {
        case 0: {
            fp = fopen("task_set.tmp", "w");
            break;
        }
        case 1: {
            fp = fopen("task_set.succ", "a+");
            break;
        }
        case 2: {
            fp = fopen("task_set.fail", "a+");
            break;
        }
        default: break;
    }

    fprintf(fp, "Task_num: %d  Processor_num: %d  LCM: %d\n", task_num, processor_num, lcm_period);
    for (i=0; i<task_num; i++) {
        fprintf(fp, "%4d (%d)\t", execute[i], execute[i]*lcm_period/period[i]);
    }
    fprintf(fp, "\n");

    for (i=0; i<task_num; i++) {
        fprintf(fp, "%4d (%d)\t ", period[i], lcm_period);
    }
    fprintf(fp, "\n");

    int tmp;
    tmp = execute[0]*lcm_period/period[0];
    fprintf(fp, "%d", execute[0]*lcm_period/period[0]);
    for (i=1; i<task_num; i++) {
        tmp += execute[i]*lcm_period/period[i];
        fprintf(fp, " + %d", execute[i]*lcm_period/period[i]);
    }
    fprintf(fp, " = %d\n\n", tmp);

    fclose(fp);
}

void load_task_set(int choice)
{
    int i;
    char tmp[255];

    FILE *fp;

    if (!(fp = fopen("task_set.tmp", "r"))) {
        printf("Task set open error!\n");
        return;
    }

    fscanf(fp, "%s %d %s %d %s %d", tmp, &task_num, tmp, &processor_num, tmp, &lcm_period);

    for(i=0; i<task_num; i++) {
        fscanf(fp, "%d %s", execute+i, tmp);
    }
    for(i=0; i<task_num; i++) {
        fscanf(fp, "%d %s", period+i, tmp);
    }

//    show_task_set(*task_num, *processor_num, *lcm_period, execute, period);
    fclose(fp);
}

#define PERIODIC_GRAPH
int schedule_task_set(int policy, int save)
{
    int i;
    int j;
    FILE* fp;
    int duration;

    switch (policy) {
        case 0: {
//            duration = schedule_random(task_num, processor_num, lcm_period, execute, period);
//            check_scheduling(task_num, processor_num, lcm_period, execute, period);
            break;
        }
        case 1: {
            duration = schedule_laxity_hierarchy();
//            check_scheduling(task_num, processor_num, lcm_period, execute, period);
            break;
        }
        default: break;
    }

    /*
    for(i=0; i<task_num; i++) {
        printf("[Task: %d]", i);
        for(j=0; j<duration; j++) {
            if (!(j%period[i])) 
                printf(YELLOW"|"END"%d", execute_flag[i][j]);
            else
                printf(" %d", execute_flag[i][j]);
        }
        printf("\n");
    }
    */

    duration = check_scheduling();

    return duration;
}

void get_time(void)
{
    time_t rawtime;
    rawtime = time(NULL);
    printf("%s\n", ctime(&rawtime));
}

void allocate() {
	int *flag, *time;
	int i, j, k, kk;
	int *to_exe, ind, maxt, tid, *selected;

	flag=(int*)malloc(sizeof(int)*task_num);
	time=(int*)malloc(sizeof(int)*task_num);
	to_exe=(int*)malloc(sizeof(int)*processor_num);
	selected=(int*)malloc(sizeof(int)*processor_num);

	for(i=0; i<task_num; i++)
	{
		flag[i]=-1;
		time[i]=-1;
	}

	for(i=0; i<lcm_period; i++) {
		ind=0;
		for(j=0; j<task_num; j++) {
			if(execute_flag[j][i]==1) {
				to_exe[ind]=j;
				ind++;
			}
		}

		for(j=0; j<processor_num; j++)
			selected[j]=0;
		for(j=0; j<processor_num; j++) {
			maxt=-1;
			tid=-1;
			for(k=0; k<processor_num; k++) {
				if((time[to_exe[k]]>maxt)&&(flag[to_exe[k]]==j))
				{
					maxt=time[to_exe[k]];
					tid=to_exe[k];
					kk=k;
				}
			}
			if(tid!=-1) {
				alloc[j][i]=tid;
				time[tid]=i;
				flag[tid]=j;
				selected[kk]=1;
			}
		}

		for(j=0; j<processor_num; j++) {
			if(alloc[j][i]==-1) {
				for(k=0; k<processor_num; k++) {
					if(selected[k]==1)
						continue;
					alloc[j][i]=to_exe[k];
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

/*void countPreemption() {
	int i, j;
	int k, m;
	int tnum=true_task_num;
	int pnum=processor_num;
	for(i=0; i<tnum; i++)
	{
		int count=0;
		for(j=0; j<lcm_period/period[i]; j++) 
		{
			int executed=0;
			for(m=0; m<period[i]; m++)
				for(k=0; k<pnum; k++)
				{
					if(alloc[k][j*period[i]+m]==i) {
						executed++;
						if(executed==execute[i])
							continue;
						if(alloc[k][j*period[i]+m+1]!=i)
							count++;
					}
				}
		}
		preemption[i]=count;
	}
}*/

float countPreemption() {
	int i, j;
	int k, m;
	int tnum=true_task_num;
	int pnum=processor_num;
	float res=0.0;
	for(i=0; i<tnum-1; i++)
	{
		int count=0;
		for(j=0; j<lcm_period/period[i]; j++) 
		{
			int executed=0;
			for(m=0; m<period[i]; m++)
				for(k=0; k<pnum; k++)
				{
					if(alloc[k][j*period[i]+m]==i) {
						executed++;
						if(executed==execute[i])
							continue;
						if(alloc[k][j*period[i]+m+1]!=i)
							count++;
					}
				}
		}
		res+=(float)count/((float)(lcm_period/period[i]));
	}
	res=res/(float)(tnum-1);
	return res;
}

/*void countMigration() {
	int i, j;
	int k, m;
	int tnum=true_task_num;
	int pnum=processor_num;
	for(i=0; i<tnum; i++)
	{
		int count=0;
		int loc=-1;
		for(j=0; j<lcm_period; j++)
		{
			for(k=0; k<pnum; k++)
			{
				if(alloc[k][j]==i) {
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
		migration[i]=count;
	}
}*/

float countMigration() {
	int i, j;
	int k, m;
	int tnum=true_task_num;
	int pnum=processor_num;
	float res=0.0;
	for(i=0; i<tnum-1; i++)
	{
		int count=0;
		int loc=-1;
		for(j=0; j<lcm_period; j++)
		{
			for(k=0; k<pnum; k++)
			{
				if(alloc[k][j]==i) {
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
		res=res+(float)count/((float)(lcm_period/period[i]));
	}
	res=res/(float)(tnum-1);
	return res;
}

/*void countEvent() {
	int i, j;
	int k, m;

	int jstart, loc;
	int tnum=true_task_num;
	int pnum=processor_num;

	for(i=0; i<tnum; i++) {
		int count=0;
		int loc=-1;
		for(j=0; j<lcm_period/period[i]; j++) {
			jstart=j*period[i];
			loc=-1;
			for(k=0; k<period[i]; k++) 
				for(m=0; m<pnum; m++) {
					if(alloc[m][jstart+k]==i) {
						if(loc==-1)
							loc=m;
						else {
							if(loc!=m)
								count++;
							loc=m;
						}
					}
				}
		}
		event[i]=preemption[i]+migration[i]-count;
	}
}*/

int main(int argc, char* argv[])
{
	int i, j, k;

	if(argc<2)
	{
		printf("usage: ./algo <taskset_filename>\n");
		return 0;
	}
	fin=fopen(argv[1], "r");
//	fout=fopen(argv[2], "w");
	flog=fopen(argv[2], "w");

	fscanf(fin, "%d%d%d%d", &true_task_num, &task_num, &processor_num, &lcm_period);
	for(j=0; j<task_num; j++)
		fscanf(fin, "%d", &period[j]);
	for(j=0; j<task_num; j++)
		fscanf(fin, "%d", &execute[j]);
	for(j=0; j<task_num; j++)
		for(k=0; k<lcm_period; k++)
			execute_flag[j][k]=-1;
	for(j=0; j<processor_num; j++)
		for(k=0; k<lcm_period; k++)
			alloc[j][k]=-1;

/*	preemption=(int*)malloc(sizeof(int)*true_task_num);
	migration=(int*)malloc(sizeof(int)*true_task_num);
	event=(int*)malloc(sizeof(int)*true_task_num);
*/
	if (lcm_period != schedule_task_set(1, 1)) {
		printf("%s: Error!!!\n", argv[1]);
   }
//	printf("%s: Successfull Scheduling\n", argv[1]);

	allocate();

	printf("%f\t%f\n", countPreemption(), countMigration());

	for(j=0; j<processor_num; j++)
	{
		for(k=0; k<lcm_period; k++)
		{
			printf("%d ", alloc[j][k]);
		}
		printf("\n");
	}
/*
	countPreemption();
	countMigration();
	countEvent();

	fprintf(flog, "preemption:\n");
	for(i=0; i<true_task_num; i++)
	{
		double ppj=((double)preemption[i])/((double)(lcm_period/period[i]));
		fprintf(flog, "%lf ", ppj);
	}
	fprintf(flog, "\n");
	fprintf(flog, "migration:\n");
	for(i=0; i<true_task_num; i++)
	{
		double mpj=((double)migration[i])/((double)(lcm_period/period[i]));
		fprintf(flog, "%lf ", mpj);
	}
	fprintf(flog, "\n");
	fprintf(flog, "event:\n");
	for(i=0; i<true_task_num; i++)
	{
		double epj=((double)event[i])/((double)(lcm_period/period[i]));
		fprintf(flog, "%lf ", epj);
	}
	fprintf(flog, "\n");

	free(preemption);
	free(migration);
	free(event);*/

//	fflush(fout);
//	fflush(flog);
 // fclose(flog);
//  fclose(fout);
  fclose(fin);
  return 0;
}



