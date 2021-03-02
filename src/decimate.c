#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <string.h>

#define DEFAULT_DIVIDER 2

int main (int argc, char *argv[]) {

    int divider = DEFAULT_DIVIDER;
    if (argc == 2) {
        divider = atoi(argv[1]);
        if (divider > 128) {
            fprintf(stderr, "[%s] max divider is 128\n", argv[0]);
            divider = 128;
        }
    }

    fprintf(stderr, "[%s] decimating by %i, IQ is from STDIN ...\n", argv[0], divider);

    float iq[256], output[2];
    char *charOutput[2];
    while(fread(iq, 4, 2 * divider, stdin) != 0) {
        for (int i = 0; i < 2; i++) {
            output[i] = iq[i];
            charOutput[i] = (char*)&output[i];
            for (int k = 0; k < 4; k++) {
                putchar(charOutput[i][k]);
            }
        }
    }

    fprintf(stderr, "[%s] done\n", argv[0]);
    return EXIT_SUCCESS;
}
