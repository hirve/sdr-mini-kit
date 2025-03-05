#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <string.h>
#include "kissfft/kiss_fft.h"

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

    if (argc != 5) {
        fprintf(stderr, "Usage: %s sample_width filter_begin filter_end amplifier\n", argv[0]);
        return EXIT_FAILURE;
    }

    int sample_packet_size = atoi(argv[1]);
    int filter_begin = atoi(argv[2]);
    int filter_end = atoi(argv[3]);
    float amplifier = atof(argv[4]);

    kiss_fft_cfg cfg = kiss_fft_alloc(sample_packet_size, 0, 0, 0);
    kiss_fft_cpx input[sample_packet_size], output[sample_packet_size];

    kiss_fft_cfg cfg_back = kiss_fft_alloc(sample_packet_size, 1, 0, 0);
    kiss_fft_cpx input_back[sample_packet_size], output_back[sample_packet_size];

    int is_input_finished = 0;

    while (1) {
        for (int i = 0; i < sample_packet_size; i++) {
            float iq[2];
            int n = fread(iq, 4, 2, stdin);
            if (n == 0) {
                is_input_finished = 1;
                break;
            }
            input[i].r = iq[0];
            input[i].i = iq[1];
        }

        if (is_input_finished == 1) {
            break;
        }

        kiss_fft(cfg, input, output);

        for (int i = 0; i < sample_packet_size; i++) {
            if (i >= filter_begin && i <= filter_end) {
                input_back[i].r = output[i].r;
                input_back[i].i = output[i].i;
            } else {
                input_back[i].r = 0.0;
                input_back[i].i = 0.0;
            }
        }

        kiss_fft(cfg_back, input_back, output_back);

        float float_output[2];
        for (int i = 0; i < sample_packet_size; i++) {
            float_output[0] = output_back[i].r * amplifier;
            float_output[1] = output_back[i].i * amplifier;
            write_two_floats(float_output);
        }
    }

    fprintf(stderr, "[%s] done.\n", argv[0]);
    return EXIT_SUCCESS;
}
