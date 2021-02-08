#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <string.h>

#define SUBJECT "nothing"
#define LEVEL 1e-3
#define TICK_PERIOD 100

int main (int argc, char *argv[]) {

    char *subject = SUBJECT;
    if (argc >= 2) { subject = argv[1]; }
    double level = LEVEL;
    if (argc >= 3) { level = atof(argv[2]); }
    double tick_period = TICK_PERIOD;
    if (argc >= 4) { tick_period = atoi(argv[3]); }

    fprintf(stderr, "[%s] adding \"%s\", level is %f, IQ is from STDIN ...\n", argv[0], subject, level);

    float iq[2], output[2];
    char *charOutput[2];
    long t = 0;
    while(fread(iq, 4, 2, stdin) != 0) {

        if (strcmp(subject, "nothing") == 0) {
            for (int i = 0; i < 2; i++) { 
                output[i] = iq[i]; 
            } 
        } else if (strcmp(subject, "tick") == 0) {
            for (int i = 0; i < 2; i++) {
                output[i] = iq[i]; 
            } 
            if (++t == tick_period) {
                t = 0;
                output[0] += level;
            }
        } else if (strcmp(subject, "noise") == 0) {
            for (int i = 0; i < 2; i++) { 
                output[i] = iq[i] + level * ((float)rand() / (float)RAND_MAX - 0.5); 
            } 
        }

        for (int i = 0; i < 2; i++) {
            charOutput[i] = (char*)&output[i];
            for (int k = 0; k < 4; k++) {
                printf("%c", charOutput[i][k]);
            }
        }
    }

    fprintf(stderr, "[%s] done.\n", argv[0]);
    return EXIT_SUCCESS;
}
