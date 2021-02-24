#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <string.h>

#define DEFAULT_MODE "LSB"
#define PI 3.1415926535897932384626433832795

int main (int argc, char *argv[]) {

    char *mode = DEFAULT_MODE;
    if (argc == 2) {
        if (strcmp("usb", argv[1]) == 0 || strcmp("USB", argv[1]) == 0) {
            mode = "USB";
        }
    }

    fprintf(stderr, "[%s] demodulating %s, IQ is from STDIN ...\n", argv[0], mode);
    fprintf(stderr, "[%s] output is 2 channels with equal signal, the same samplerate.\n", argv[0]);
    fprintf(stderr, "[%s] use low-pass filters on input and output.\n", argv[0]);

    double t = 0, demodT = 0;
    double dt = PI / 2, demodDt = PI / 2 + PI / 8;

    if (strcmp(mode, "USB") == 0) {
        demodDt = PI / 2 - PI / 8;
    }

    float iq[2], shifted, output;
    char *charOutput;
    while(fread(iq, 4, 2, stdin) != 0) {

        shifted = iq[0] * cos(t) - iq[1] * sin(t);
        output = shifted * sin(demodT);

        for (int i = 0; i < 2; i++) {
            charOutput = (char*)&output;
            for (int k = 0; k < 4; k++) {
                printf("%c", charOutput[k]);
            }
        }

        t += dt;
        demodT += demodDt;
    }

    fprintf(stderr, "[%s] done\n", argv[0]);
    return EXIT_SUCCESS;
}
