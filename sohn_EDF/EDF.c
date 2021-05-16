#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int period[101];
int burst[101];
int deadline[101];
int process_num;
int hyper_period;


int gcd(int a, int b){

    if(a % b == 0){
        return b;
    }
    return gcd(b, a % b);
}


int lcm(int a, int b){
    return a * b / gcd(a,b);
}


int n_lcm(int num[]){
    if(process_num == 1){
        return num[0];
    }
    int i = 1;
    int result = num[0];
    while(num[i] != 0){
        result = lcm(result, num[i]);
        i++;
    }
    return result;
}


int load_data(){

    FILE* fp = fopen("input.txt", "r");

    int process_num = 0;
    while(feof(fp) == 0){
        fscanf(fp, "%d %d %d", &period[process_num], &burst[process_num], &deadline[process_num]);
        process_num++;
    }
    fclose(fp);
    return process_num;
}


double schedulability(){

    double result = 0;
    for(int i = 0; i < process_num; i++){
        result += (double)burst[i]/(double)period[i];
    }

    return result;
}


void GanttChart(int* schedule){

    FILE* fp = fopen("output.txt", "w");
    char* gantt[101];

    printf("\n");
    for(int i = 0; i < process_num; i++){
        gantt[i] = (char*)malloc(sizeof(char)*hyper_period);
        memset(gantt[i], 0, sizeof(char));
    }

    for(int i = 0; i < hyper_period; i++){
        
        for(int j = 0; j < process_num; j++){
            if(schedule[i] == j){
                gantt[j][i]= '-';
            }
            else{
                gantt[j][i] = ' ';
            }
        
        }
    }
    for(int i = 0; i < process_num; i++){
        printf("P%d ", i+1);
        fprintf(fp, "P%d |", i+1);
        for(int j = 0; j < hyper_period; j++){
            printf("%c",gantt[i][j]);
            fprintf(fp, "%c%c|", gantt[i][j], gantt[i][j]);
        }
        fprintf(fp,"\n\n");
        printf("\n");
    }
    printf("\n");

    fclose(fp);

}


void edf(){
    
    int* schedule = (int*)malloc(sizeof(int)*hyper_period);

    //check remain period, burst, deadline in each time
    int r_period[101];
    int r_burst[101];
    int r_deadline[101];

    for(int i = 0; i < process_num; i++){
        r_period[i] = period[i];
        r_burst[i] = burst[i];
        r_deadline[i] = deadline[i];
    }

    for(int i = 0; i < hyper_period; i++){
        
        //find earliest deadline process
        int min_deadline = hyper_period;
        int index = -1;
        for(int j = 0; j < process_num; j++){
            
            if(r_burst[j] > 0){
                if(min_deadline > r_deadline[j] && r_deadline[j] > 0){
                    min_deadline = r_deadline[j];
                    index = j;
                }
            }

        }
        //cpu burst, time spent
        r_burst[index]--;
        schedule[i] = index;

        for(int j = 0; j < process_num; j++){

            r_period[j]--;
            r_deadline[j]--;
            if(r_period[j] == 0){
                r_period[j] = period[j];
                r_burst[j] = burst[j];
                r_deadline[j] = deadline[j];
            }
            else{
                if(r_deadline[j] <= 0 && r_burst[j] > 0){
                    printf("Process P%d can't be completed\n", j);
                }
            }
            
        }
        /////
        // printf("%d\n", index);
        // for(int j = 0; j < process_num; j++){
        //     printf("%d %d %d\n", r_period[j], r_burst[j], r_deadline[j]);
        // }
        // printf("\n");
    }
    
    GanttChart(schedule);

}


int main(){

    process_num = load_data();
    
    hyper_period = n_lcm(period);

    double schedulable = schedulability();
    printf("\n%lf ", schedulable);
    if(schedulable <= 1){
        printf("can schedule\n\n");
    }
    else{
        printf("not to schedule\n\n");
        return 0;
    }

    for(int i = 0; i < process_num; i++){
        printf("%d %d %d\n", period[i], burst[i], deadline[i]);
    }
  
    edf();

    return 0;
}