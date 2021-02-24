#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <string.h>

#define FEEDBACK_UP 0.01
#define FEEDBACK_DN 0.0001
#define LEVEL_MULTIPLIER 100000
#define AMPLIFIER 10
#define INIT_LEVEL 10.0

int main () {
    char IIII[100];
    memset(IIII, '|', 99);
    IIII[99] = '\0';

    fprintf(stderr, "AGC: taking IQ data from STDIN ...\n");

    float iq[2], out[2];
    double level = INIT_LEVEL;
    int i = 0;
    while(fread(iq, 4, 2, stdin) != 0) {
        if (iq[0] > level) {
            level = FEEDBACK_UP * (double)fabs(iq[0] * LEVEL_MULTIPLIER) + (1 - FEEDBACK_UP) * level;
        } else {
            level = FEEDBACK_DN * (double)fabs(iq[0] * LEVEL_MULTIPLIER) + (1 - FEEDBACK_DN) * level;
        }
        if (++i % 4000 == 0) {
            int scale = log10(level * 100) * 8.0;
            fprintf(stderr, "[ Level ] \e[32m%.*s\e[39m %1.2lf                \r", scale, IIII, level);
        }
        out[0] = iq[0] / level * AMPLIFIER;
        out[1] = iq[1] / level * AMPLIFIER;
        char *charReal = (char*)&out[0];
        char *charImag = (char*)&out[1];
        for (int k = 0; k < 4; k++) {
            printf("%c",charReal[k]);
        }
        for (int k = 0; k < 4; k++) {
            printf("%c", charImag[k]);
        }
    }

    fprintf(stderr, "AGC: done.\n");
    return EXIT_SUCCESS;
}
