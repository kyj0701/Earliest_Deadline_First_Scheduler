#include <stdio.h>
#include <stdlib.h>
#include "process.h"

int proc_num;
int util;
PROC *p;

int main() {
    p = malloc(101 * sizeof(PROC));

    int largest_deadline = get_process_info(p, &proc_num);

    int *result = edf_schedule(p, proc_num, largest_deadline);

    FILE *output = fopen("output.txt", "w");
    for(int i=0; i < largest_deadline; i++) {
        fprintf(output, "%d ", result[i]);
    }

    free(p);
}