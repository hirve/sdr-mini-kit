#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <string.h>

#define FEEDBACK_UP 0.01
#define FEEDBACK_DN 0.0001
#define LEVEL_MULTIPLIER 100000
#define AMPLIFIER 100
#define INIT_LEVEL 10.0
#define LEVEL_TICK 2048

void write_two_floats (float output[2]) {
    char *charOutput[2];
    for (int i = 0; i < 2; i++) {
        charOutput[i] = (char*)&output[i];
        for (int k = 0; k < 4; k++) {
            putchar(charOutput[i][k]);
        }
    }
}

int main (int argc, char *argv[]) {
    double sql = 0;
    if (argc == 2) {
        sql = atof(argv[1]);
    } 

    char IIII[100];
    memset(IIII, '|', 99);
    IIII[99] = '\0';

    fprintf(stderr, "AGC: taking IQ data from STDIN ...\n");
    fprintf(stderr, "[%s] SQL is %f\n", argv[0], sql);

    float iq[2], out[2];
    double level = INIT_LEVEL;
    int i = 0;
    while(fread(iq, 4, 2, stdin) != 0) {
        if (iq[0] > level) {
            level = FEEDBACK_UP * (double)fabs(iq[0] * LEVEL_MULTIPLIER) + (1 - FEEDBACK_UP) * level;
        } else {
            level = FEEDBACK_DN * (double)fabs(iq[0] * LEVEL_MULTIPLIER) + (1 - FEEDBACK_DN) * level;
        }
        if (++i % LEVEL_TICK == 0) {
            int scale = log10(level * AMPLIFIER) * 8.0;
            fprintf(stderr, "[ Level ] \e[32m%.*s\e[39m %1.2lf                \r", scale, IIII, level);
        }
        if (level >= sql) {
            out[0] = iq[0] / level * AMPLIFIER;
            out[1] = iq[1] / level * AMPLIFIER;
        } else {
            out[0] = 0;
            out[1] = 0;
        }
        write_two_floats(out);
    }

    fprintf(stderr, "AGC: done.\n");
    return EXIT_SUCCESS;
}
