#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define PI 3.1415926535897932384626433832795
#define GEN_FREQ 0
#define GEN_LEVEL 1e-3
#define MODE "am"
#define MODULATION_LEVEL 0.8
#define SAMPLERATE 256e3

#define FADING_LEVEL 0.0
#define FADING_N 5
#define FADING_INIT_ORDER 0.1
#define FADING_ORDER_DIFF 3.0
#define FADING_DT_MULTIPLIER 0.00001

static double fadingT[FADING_N], fadingDT[FADING_N], fadingLevel;

void init_fading (unsigned seed) {
    srand(seed);
    double order = FADING_INIT_ORDER;
    fadingLevel = 1.0 / FADING_N;
    for (int i = 0; i < FADING_N; i++) {
        fadingT[i] = 0;
        fadingDT[i] = FADING_DT_MULTIPLIER * (order + order * 0.25 * (double)rand() / (double)RAND_MAX - 0.5);
        order *= FADING_ORDER_DIFF;
    }
}

double get_fading () {
    double fading = 1.0;
    for (int i = 0; i < FADING_N; i++) {
        fading += fadingLevel * sin(fadingT[i]);
        fadingT[i] += fadingDT[i];
    }
    return fading;
}

int main (int argc, char *argv[]) {

    char *sound_file_name = argv[1];
    double gen_freq = GEN_FREQ;
    if (argc >= 3) { gen_freq = atof(argv[2]); }
    double gen_level = GEN_LEVEL;
    if (argc >= 4) { gen_level = atof(argv[3]); }
    char *mode = MODE;
    if (argc >= 5) { mode = argv[4]; }
    int mode_num = strcmp(mode, "dsb") == 0 ? 3 : strcmp(mode, "pm") == 0 ? 2 : strcmp(mode, "fm") == 0 ? 1 : 0;
    double samplerate = SAMPLERATE;
    if (argc >= 6) { samplerate = atoi(argv[5]); }
    double fading_level = FADING_LEVEL;
    if (argc >= 7) {
        fading_level = atof(argv[6]);
        fading_level = fading_level < 0.0 ? 0.0 : (fading_level > 1.0 ? 1.0 : fading_level);
    }

    fprintf(stderr,
        "[%s] open \"%s\", \"%s\" mode @ %1.1f Hz, level is %1.1f, fading is %1.1lf, IQ is from STDIN ...\n",
         argv[0],  argv[1], mode,        gen_freq,          gen_level,       fading_level
    );

    init_fading((unsigned)(gen_freq));

    FILE *sound_file = fopen(sound_file_name, "r");
    if (sound_file == NULL) {
        fprintf(stderr, "[%s] cannot open sound file \"%s\".\n", argv[0], argv[1]);
        return EXIT_FAILURE;
    }

    double t = 0;
    double dt = 2 * PI * gen_freq / samplerate;

    float iq[2], output[2], sound[1];
    char *charOutput[2];
    while(fread(iq, 4, 2, stdin) != 0) {
        if (fread(sound, 4, 1, sound_file) == 0) { break; }

        double level = gen_level;
        if (fading_level != 0.0) {
            level = (1.0 - fading_level) * gen_level + fading_level * get_fading() * gen_level;
        }

        if (mode_num == 2) {
            output[0] = iq[0] + level * cos(t + 0.8 * MODULATION_LEVEL * sound[0]);
            output[1] = iq[1] + level * sin(t + 0.8 * MODULATION_LEVEL * sound[0]);
        } else if (mode_num == 3) {
            output[0] = iq[0] + level * MODULATION_LEVEL * sound[0] * cos(t); 
            output[1] = iq[1] + level * MODULATION_LEVEL * sound[0] * sin(t); 
        } else if (mode_num == 1) {
            double dtFm = dt * (1 + 0.5 * MODULATION_LEVEL * sound[0]);
            output[0] = iq[0] + level * cos(t);
            output[1] = iq[1] + level * sin(t);
            t -= dt;
            t += dtFm;
        } else {
            output[0] = iq[0] + level * cos(t) + level * MODULATION_LEVEL * sound[0] * cos(t); 
            output[1] = iq[1] + level * sin(t) + level * MODULATION_LEVEL * sound[0] * sin(t); 
        }

        for (int i = 0; i < 2; i++) {
            charOutput[i] = (char*)&output[i];
            for (int k = 0; k < 4; k++) {
                putchar(charOutput[i][k]);
            }
        }

        t += dt;
    }

    fclose(sound_file);
    fprintf(stderr, "[%s] done.\n", argv[0]);
    return EXIT_SUCCESS;
}
