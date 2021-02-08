#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <string.h>
#include "filters/lp2.c"
#include "filters/0to20_30of192ks.c"
#include "filters/lp1of8_30db.c"

#define DEFAULT_DIVIDER 2

// todo optimize and add weaker filters to run it on raspberry pi zero
// todo argument should be a filter name: ./filter [lp2|lp4|lp8|...]

static Filter_lp2 filter2_r, filter2_i;
static Filter_0to20_30of192ks filter4_r, filter4_i;
static Filter_lp1of8_30db filter8_r, filter8_i;

int main (int argc, char *argv[]) {

    int divider = DEFAULT_DIVIDER;
    if (argc == 2) {
        int dividerArg = atoi(argv[1]);
        if (dividerArg == 2 || dividerArg == 4 || dividerArg == 8) {
            divider = dividerArg;
        }
    }

    fprintf(stderr, "[%s] low-pass filter 0..1/%i, IQ is from STDIN ...\n", argv[0], divider);

    if (divider == 8) {
        filter_lp1of8_30db_init(&filter8_r);
        filter_lp1of8_30db_init(&filter8_i);
    } else if (divider == 2) {
        filter_lp2_init(&filter2_r);
        filter_lp2_init(&filter2_i);
    } else {
        filter_0to20_30of192ks_init(&filter4_r);
        filter_0to20_30of192ks_init(&filter4_i);
    }

    float iq[2], output[2];
    char *charOutput[2];
    while(fread(iq, 4, 2, stdin) != 0) {
        if (divider == 8) {
            output[0] = filter_lp1of8_30db(&filter8_r, iq[0]);
            output[1] = filter_lp1of8_30db(&filter8_i, iq[1]);
        } else if (divider == 2) {
            output[0] = filter_lp2(&filter2_r, iq[0]);
            output[1] = filter_lp2(&filter2_i, iq[1]);
        } else {
            output[0] = filter_0to20_30of192ks(&filter4_r, iq[0]);
            output[1] = filter_0to20_30of192ks(&filter4_i, iq[1]);
        }
        for (int i = 0; i < 2; i++) {
            charOutput[i] = (char*)&output[i];
            for (int k = 0; k < 4; k++) {
                printf("%c", charOutput[i][k]);
            }
        }
    }

    fprintf(stderr, "[%s] done\n", argv[0]);
    return EXIT_SUCCESS;
}
