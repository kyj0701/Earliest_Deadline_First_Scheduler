#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "process.h"

int gcd(int a, int b) {
    return a%b == 0 ? b : gcd(b, a%b);
}

int lcm(int a, int b) {
    return a * b / gcd(a, b);
}

void get_process_info(PROC *p, int proc_num) {

    FILE *InputFile = fopen("input.txt", "r");

    for(int i=0; i<proc_num; i++) {
        char buf[255];
        memset(buf, 0, 255);
        fgets(buf, 255, InputFile);

        char *a = strtok(buf, " \n");
        char *b = strtok(NULL, " \n");
        char *d = strtok(NULL, " \n");
        p[i].arrival = atoi(a);
        p[i].burst = atoi(b);
        p[i].deadline = atoi(d);
        p[i].period = atoi(d);
    }

    printf("  Pi |   Ai   Bi   Di   Pi\n");
    printf("-----|--------------------\n");
    for(int i=0; i<proc_num; i++) {
        printf("%4d | %4d %4d %4d %4d\n", i+1, p[i].arrival, p[i].burst, p[i].deadline, p[i].period);
    }

    fclose(InputFile);
}

int check_cpu_util(PROC *p, int proc_num) {
    float util = 0;

    for(int i=0; i<proc_num; i++, p++) {
        util = util + ((float)p->burst) / ((float)p->deadline);
    }

    printf("\nCPU Utilization : %f\n", util);

    return util < 1 ? TRUE : FALSE;
}

int cal_hyper_period(PROC *p, int proc_num) {
    if(proc_num == 1) return p[0].period;

    int res = p->period;
    p++;
    for(int i=1; i<proc_num; i++, p++) {
        res = lcm(res, p->period);
    }

    return res;
}

int* edf_schedule(PROC *p, int proc_num, int hyper_period) {
    int selected_proc;
    int selected_proc_deadline;
    RPROC *r_proc = malloc(proc_num * sizeof(RPROC));
    int *result = (int *)malloc(hyper_period * sizeof(int));
    int r_proc_cnt = proc_num;

    memset(result, 0, hyper_period);

    for(int timer=0; timer<hyper_period; timer++) {
        selected_proc = -1;
        selected_proc_deadline = hyper_period;

        // arrival time에 맞춰서
        if(r_proc_cnt > 0) {
            for(int j=0; j<proc_num; j++) {
                if(p[j].arrival == timer) {
                    r_proc[j].n_period = p[j].period;
                    r_proc[j].l_burst = p[j].burst;
                    r_proc[j].l_deadline = p[j].deadline;
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
            if(r_proc[j].l_burst > 0) {
                if(selected_proc_deadline > r_proc[j].l_deadline) {
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
            if(r_proc[j].n_period == 0) continue;

            if((timer+1)%r_proc[j].n_period == 0) { // 주기가 지났다면
                r_proc[j].l_burst = p[j].burst;
                r_proc[j].l_deadline = p[j].deadline;
            }
            else { // 아직 지나지 않았다면
                r_proc[j].l_deadline--;
                if(r_proc[j].l_deadline <= 0 && r_proc[j].l_burst > 0) { // deadline은 지났지만 burst time이 남아있다면
                    printf("P %d : cant run\n", j);
                }
            }
        }
    }

    return result;
}

void make_gantt_chart(int *result, int proc_num, int hyper_period) {
    char *res[101];
    FILE *OutputFile = fopen("output.txt", "w");

    for(int i=0; i<proc_num; i++) {
        res[i] = (char *)malloc((hyper_period * 2) * sizeof(char));
        memset(res[i], 0, hyper_period * 2);

        char num[10];
        sprintf(num, "P%d |", i+1);

        strcat(res[i], num);
    }

    for(int timer=0; timer<hyper_period; timer++) {
        int cur_burst = result[timer];
        for(int i=0; i<proc_num; i++) {
            if(cur_burst == i) strcat(res[i], "-");
            else strcat(res[i], " ");
        }
    }
    printf("\n");
    
    for(int i=0; i<proc_num; i++) {
        strcat(res[i], "|");
        printf("%s\n", res[i]);
        fprintf(OutputFile, "%s\n", res[i]);
    }

    for(int i=0; i<proc_num; i++) free(res[i]);
    fclose(OutputFile);
}