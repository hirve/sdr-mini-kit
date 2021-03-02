#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <string.h>
#include "filters/lp2.c"
#include "filters/lp8lq.c"
#include "filters/lp16.c"
#include "filters/0to20_30of192ks.c"

#define DEFAULT_DIVIDER 2

static Filter_lp16 filter_lp16_r, filter_lp16_i;
static Filter_lp8lq filter_lp8lq_r, filter_lp8lq_i;
static Filter_lp2 filter2_r, filter2_i;
static Filter_0to20_30of192ks filter4_r, filter4_i;

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

    int divider = DEFAULT_DIVIDER;
    if (argc == 2) { // todo make filter arg filter name: ./filter [lp2|lp4|lp8]
        int dividerArg = atoi(argv[1]);
        if (dividerArg == 2 || dividerArg == 4 || dividerArg == 8 || dividerArg == 16) {
            divider = dividerArg;
        }
    }

    fprintf(stderr, "[%s] low-pass filter 0..1/%i, IQ is from STDIN ...\n", argv[0], divider);

    float iq[2], output[2];
    if (divider == 8) {
        filter_lp8lq_init(&filter_lp8lq_r);
        filter_lp8lq_init(&filter_lp8lq_i);
        while(fread(iq, 4, 2, stdin) != 0) {
            output[0] = filter_lp8lq(&filter_lp8lq_r, iq[0]);
            output[1] = filter_lp8lq(&filter_lp8lq_i, iq[1]);
            write_two_floats(output);
        }
    } else if (divider == 16) {
        filter_lp16_init(&filter_lp16_r);
        filter_lp16_init(&filter_lp16_i);
        while(fread(iq, 4, 2, stdin) != 0) {
            output[0] = filter_lp16(&filter_lp16_r, iq[0]);
            output[1] = filter_lp16(&filter_lp16_i, iq[1]);
            write_two_floats(output);
        }
    } else if (divider == 2) {
        filter_lp2_init(&filter2_r);
        filter_lp2_init(&filter2_i);
        while(fread(iq, 4, 2, stdin) != 0) {
            output[0] = filter_lp2(&filter2_r, iq[0]);
            output[1] = filter_lp2(&filter2_i, iq[1]);
            write_two_floats(output);
        }
    } else {
        filter_0to20_30of192ks_init(&filter4_r);
        filter_0to20_30of192ks_init(&filter4_i);
        while(fread(iq, 4, 2, stdin) != 0) {
            output[0] = filter_0to20_30of192ks(&filter4_r, iq[0]);
            output[1] = filter_0to20_30of192ks(&filter4_i, iq[1]);
            write_two_floats(output);
        }
    }

    fprintf(stderr, "[%s] done\n", argv[0]);
    return EXIT_SUCCESS;
}
