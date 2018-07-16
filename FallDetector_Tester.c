/*
 * Runs FallDetector.c on an accelerometer trace.
 *
 * Author: Zach Zhou
 */

#include <stdio.h>
#include <stdbool.h>
#include "FallDetector.h"

int main(int argc, char *argv[]) {
    FILE *f;
    char *line = NULL;
    size_t len;
    int length;
    
    double t;
    double x, y, z;
    
    int line_num = 0;
    bool fall_detected;
    int falls_detected;
    
    f = fopen(argv[1], "r");
    
    while ((length = getline(&line, &len, f)) != -1) {
        if (line[0] == '@') {
            break;
        }
    }
    while (fscanf(f, "%lf, %lf, %lf, %lf", &t, &x, &y, &z) != EOF) {
        //printf("%lf, %lf, %lf\n", x, y, z);
        line_num++;
        
        fall_detected = run_fall_detector(x, y, z);
        
        if (fall_detected) {
            //printf("Line %d\n", line_num);
            falls_detected++;
        }
    }
    
    fclose(f);
    
    return falls_detected;
}
