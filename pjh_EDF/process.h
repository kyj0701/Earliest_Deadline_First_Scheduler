#define TRUE    1
#define FALSE   0
#define IDLE    -1

// process info
typedef struct _PROC {
    int arrival;
    int burst;
    int deadline;
} PROC;

// running process info
typedef struct _RPROC {
    int l_burst;
    int l_deadline;
    int isrun;
} RPROC;

int get_process_info(PROC *p, int *proc_num);
int check_cpu_util(RPROC *r_proc, int cur_proc, float *util, int flag);
int* edf_schedule(PROC *p, int proc_num, int largest_deadline);