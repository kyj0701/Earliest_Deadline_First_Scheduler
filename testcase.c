#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRIVE_MAX  20
#define BURST_MAX   25
#define DEAD_MAX    200

int isunique(int* arr, int size, int data){
    int unique = 1;
    for(int i = 0; i < size; i++){
        if(arr[i] == data){
            unique = 0;
            break;
        }
    }
    return unique;
}

int compare(const void *a, const void *b)
{
    
    const int* num1 = *(int**)a;  
    const int* num2 = *(int**)b; 
    if (num1[0] < num2[0])   
        return -1;     
    
    if (num1[0] > num2[0])   
        return 1;      
    
    return 0;  
}

int main(){

    srand(time(NULL));
    int process_num = 0;
    scanf("%d", &process_num);

    int** arrive_arr = (int**)malloc(sizeof(int*)*process_num);
    for(int i=0; i<process_num; i++){
        arrive_arr[i] = (int*)malloc(sizeof(int)*3);
    }

    FILE* fp = fopen("input.txt", "w");

    for(int i = 0; i < process_num; i++){

        int newarrive = rand()%ARRIVE_MAX;
        int newburst = (rand()%(BURST_MAX-1))+1;
        int newdead = newarrive + rand()%DEAD_MAX;
        // while(newarrive >= newdead){
        //     newarrive = rand()%ARRIVE_MAX;
        //     newdead = rand()%DEAD_MAX;
        // }
        
        arrive_arr[i][0] = newarrive;
        arrive_arr[i][1] = newburst;
        arrive_arr[i][2] = newdead;

    }
    qsort(arrive_arr, process_num, sizeof(arrive_arr[0]), compare);

    for(int i = 0; i < process_num; i++){
        if(i == process_num-1){
            fprintf(fp, "%d %d %d", arrive_arr[i][0], arrive_arr[i][1], arrive_arr[i][2]);
            break;
        }
        fprintf(fp, "%d %d %d\n", arrive_arr[i][0], arrive_arr[i][1], arrive_arr[i][2]);
    }



    fclose(fp);
    return 0;

}