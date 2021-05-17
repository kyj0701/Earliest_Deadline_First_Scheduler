#define TRUE    1
#define FALSE   0
#define IDLE    -1

// process info
typedef struct _PROC {
    int arrival;
    int burst;
    int deadline;
    int period;
} PROC;

// running process info
typedef struct _RPROC {
    int l_burst;
    int l_deadline;
    int n_period;
} RPROC;

int gcd(int a, int b);
int lcm(int a, int b);

void get_process_info(PROC *p, int proc_num);
int check_cpu_util(PROC *p, int proc_num);
int cal_hyper_period(PROC *p, int proc_num);
int* edf_schedule(PROC *p, int proc_num, int hyper_period);
void make_gantt_chart(int *result, int proc_num, int hyper_period);