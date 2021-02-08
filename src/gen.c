#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <string.h>

#define SUBJECT "nothing"
#define LEVEL 1e-3
#define SECONDS 60
#define SAMPLERATE 256000

int main (int argc, char *argv[]) {

    char *subject = SUBJECT;
    if (argc >= 2) { subject = argv[1]; }
    double level = LEVEL;
    if (argc >= 3) { level = atof(argv[2]); }
    double seconds = SECONDS;
    if (argc >= 4) { seconds = atof(argv[3]); }
    long samplerate = SAMPLERATE;
    if (argc >= 5) { samplerate = atoi(argv[4]); }

    fprintf(stderr, "[%s] generating \"%s\", level is %f sample rate is %li ...\n", argv[0], subject, level, samplerate);

    float output[2];
    char *charOutput[2];
    int di = seconds == 0.0 ? 0 : 1;
    long samples = seconds == 0.0 ? 1 : samplerate * seconds;
    for (long i = 0; i < samples; i += di) {

        if (strcmp(subject, "nothing") == 0) {
            for (int i = 0; i < 2; i++) { 
                output[i] = 0; 
            } 
        } else if (strcmp(subject, "noise") == 0) {
            for (int i = 0; i < 2; i++) { 
                output[i] = level * ((float)rand() / (float)RAND_MAX - 0.5); 
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
