#include <stdio.h>
#include <stdlib.h>
#include "process.h"

int proc_num;
int util;
PROC *p;

int main(int argc, char **argv) {
    
    // printf("input # of process : ");
    // scanf("%d", &proc_num);

    if(argc != 2) {
        printf("Usage : ./a.out [proc_num]\n");
        exit(1);
    }

    proc_num = atoi(argv[1]);

    p = malloc(proc_num * sizeof(PROC));

    get_process_info(p, proc_num);

    util = check_cpu_util(p, proc_num);

    if(util == TRUE) {
        printf("can scheduling..\n");

        int hyper_period = cal_hyper_period(p, proc_num);
        printf("hyper period : %d\n\n", hyper_period);

        int *result = edf_schedule(p, proc_num, hyper_period);

        make_gantt_chart(result, proc_num, hyper_period);
    } else {
        printf("cannot scheduling..\n");
        exit(0);
    }

}