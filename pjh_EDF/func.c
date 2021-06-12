#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "process.h"

int get_process_info(PROC *p, int *proc_num) {
    int largest_deadline = -1;

    FILE *InputFile = fopen("input.txt", "r");

    while(!feof(InputFile)) {
        char buf[255];
        memset(buf, 0, 255);
        fgets(buf, 255, InputFile);

        char *a = strtok(buf, " \n");
        char *b = strtok(NULL, " \n");
        char *d = strtok(NULL, " \n");
        p->arrival = atoi(a);
        p->burst = atoi(b);
        p->deadline = atoi(d);

        if(largest_deadline < p->deadline) {
            largest_deadline = p->deadline;
        }

        p++;
        *proc_num += 1;
    }

    fclose(InputFile);

    return largest_deadline;
}

int* edf_schedule(PROC *p, int proc_num, int largest_deadline) {
    int selected_proc;
    int selected_proc_deadline;
    RPROC *r_proc = malloc(proc_num * sizeof(RPROC));
    int *result = (int *)malloc(largest_deadline * sizeof(int));
    int r_proc_cnt = proc_num;

    memset(r_proc, 0, proc_num * sizeof(RPROC));
    memset(result, 0, largest_deadline);
    
    for(int timer=0; timer < largest_deadline; timer++) {
        selected_proc = -1;
        selected_proc_deadline = largest_deadline;

        // arrival time에 맞춰서
        if(r_proc_cnt > 0) {
            for(int j=0; j<proc_num; j++) {
                if(p[j].arrival == timer) {
                    if(p[j].arrival >= p[j].deadline) continue;

                    r_proc[j].l_burst = p[j].burst;
                    r_proc[j].l_deadline = p[j].deadline;
                    r_proc[j].isrun = 1;
                    r_proc_cnt--;
                }
            }
        }

        // 현재 실행되고 있는 process가 없음
        if(r_proc_cnt == proc_num) {
            result[timer] = selected_proc;
            printf("(%d %d) : CPU IDLE\n", timer, timer+1);
            continue;
        }

        // CPU에 올릴 process 선택
        for(int j=0; j<proc_num; j++) {
            if(r_proc[j].l_burst > 0 && r_proc[j].isrun == 1) {
                if(selected_proc_deadline >= r_proc[j].l_deadline) {
                    selected_proc = j;
                    selected_proc_deadline = r_proc[j].l_deadline;
                }
            }
        }

        result[timer] = selected_proc;

        if(selected_proc == -1) {
            printf("(%d %d) : CPU IDLE\n", timer, timer+1);
        }
        else {
            r_proc[selected_proc].l_burst--;
            printf("(%d %d) : CPU %d, left burst %d\n", timer, timer+1, selected_proc+1, r_proc[selected_proc].l_burst);
        }

        for(int j=0; j<proc_num; j++) {
            // arrival time에 맞추지 못한 process는 거름
            if(r_proc[j].isrun == 0) continue;

            if(r_proc[j].l_burst == 0 || r_proc[j].l_deadline <= timer+1) {
                r_proc[j].isrun = 0;
            }
        }
    }

    return result;
}