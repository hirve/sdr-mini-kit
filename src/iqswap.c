#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <string.h>

int main (int argc, char *argv[]) {

    (void)argc;
    fprintf(stderr, "[%s] swapping IQ, IQ is from STDIN ...\n", argv[0]);

    float iq[2], output[2];
    char *charOutput[2];
    while(fread(iq, 4, 2, stdin) != 0) {
        for (int i = 0; i < 2; i++) {
            output[i] = iq[1 - i];
            charOutput[i] = (char*)&output[i];
            for (int k = 0; k < 4; k++) {
                putchar(charOutput[i][k]);
            }
        }
    }

    fprintf(stderr, "[%s] done\n", argv[0]);
    return EXIT_SUCCESS;
}
