#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <string.h>

int main (int argc, char *argv[]) {

    fprintf(stderr, "[%s] demodulating AM, IQ is from STDIN ...\n", argv[0]);
    fprintf(stderr, "[%s] output is 2 channels with equal signal, the same samplerate.\n", argv[0]);
    fprintf(stderr, "[%s] use low-pass filters on input and output.\n", argv[0]);

    (void)argc;
    float iq[2], output;
    char *charOutput;
    while(fread(iq, 4, 2, stdin) != 0) {

        output = sqrt(iq[0] * iq[0] + iq[1] * iq[1]);

        for (int i = 0; i < 2; i++) {
            charOutput = (char*)&output;
            for (int k = 0; k < 4; k++) {
                putchar(charOutput[k]);
            }
        }
    }

    fprintf(stderr, "[%s] done\n", argv[0]);
    return EXIT_SUCCESS;
}
