#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <string.h>
#include "kissfft/kiss_fft.h"

void write_complex_to_stdout(kiss_fft_cpx cpx_value) {
    char *charOutput;
    charOutput = (char*)&cpx_value.r;
    for (int k = 0; k < 4; k++) { putchar(charOutput[k]); }
    charOutput = (char*)&cpx_value.i;
    for (int k = 0; k < 4; k++) { putchar(charOutput[k]); }
}

int main (int argc, char *argv[]) {

    if (argc != 5) {
        fprintf(stderr, "Usage: %s sample_width filter_begin filter_end amplifier\n", argv[0]);
        return EXIT_FAILURE;
    }

    int packet_size = atoi(argv[1]);
    int packet_size_half = packet_size >> 1;
    int filter_begin = atoi(argv[2]);
    int filter_end = atoi(argv[3]);
    float amplifier = atof(argv[4]);

    kiss_fft_cfg cfg = kiss_fft_alloc(packet_size, 0, 0, 0);
    kiss_fft_cpx input[packet_size], output[packet_size];
    kiss_fft_cpx input_mid[packet_size], output_mid[packet_size];

    kiss_fft_cfg cfg_back = kiss_fft_alloc(packet_size, 1, 0, 0);
    kiss_fft_cpx input_back[packet_size], output_back[packet_size];
    kiss_fft_cpx input_back_mid[packet_size], output_back_mid[packet_size];

    kiss_fft_cpx output_acc[packet_size], output_acc_mid[packet_size], output_acc_prev[packet_size];

    int is_input_finished = 0;

    while (1) {

        // Taking input
        for (int i = 0; i < packet_size; i++) {
            float iq[2];
            int n = fread(iq, 4, 2, stdin);
            if (n == 0) {
                is_input_finished = 1;
                break;
            }
            input[i].r = iq[0];
            input[i].i = iq[1];
        }
        if (is_input_finished == 1) { break; }

        // Fill 2nd half of middle input <- with 1st half of current input
        // The 1st half  of middle input <- already taken from prev input
        for (int i = 0; i < packet_size_half; i++) {
            input_mid[i + packet_size_half].r = input[i].r;
            input_mid[i + packet_size_half].i = input[i].i;
        }

        // Apply filter to current and middle packets
        kiss_fft(cfg, input, output);
        kiss_fft(cfg, input_mid, output_mid);
        for (int i = 0; i < packet_size; i++) {
            if (i >= filter_begin && i <= filter_end) {
                input_back[i].r = output[i].r;
                input_back[i].i = output[i].i;
                input_back_mid[i].r = output_mid[i].r;
                input_back_mid[i].i = output_mid[i].i;
            } else {
                input_back[i].r = 0.0;
                input_back[i].i = 0.0;
                input_back_mid[i].r = 0.0;
                input_back_mid[i].i = 0.0;
            }
        }
        kiss_fft(cfg_back, input_back, output_back);
        kiss_fft(cfg_back, input_back_mid, output_back_mid);

        // Fill acc and middle acc <- with filter outputs
        for (int i = 0; i < packet_size; i++) {
            float k = (i >= (packet_size_half >> 1) && i < packet_size_half + (packet_size_half >> 1)) ? 1.0 : 0.0;
            output_acc[i].r = output_back[i].r * amplifier * k;
            output_acc[i].i = output_back[i].i * amplifier * k;
            output_acc_mid[i].r = output_back_mid[i].r * amplifier * k;
            output_acc_mid[i].i = output_back_mid[i].i * amplifier * k;
        }

        // Add to 1st half of output acc <- second half of middle acc
        for (int i = 0; i < packet_size_half; i++) {
            output_acc[i].r += output_acc_mid[i + packet_size_half].r;
            output_acc[i].i += output_acc_mid[i + packet_size_half].i;
        }

        // Add to 2nd half of prev output acc <- first half of middle acc
        for (int i = packet_size_half; i < packet_size; i++) {
            output_acc_prev[i].r += output_acc_mid[i - packet_size_half].r;
            output_acc_prev[i].i += output_acc_mid[i - packet_size_half].i;
        }

        // Output prev acc
        for (int i = 0; i < packet_size; i++) {
            write_complex_to_stdout(output_acc_prev[i]);
        }

        // Fill prev acc <- with current acc
        for (int i = 0; i < packet_size; i++) {
            output_acc_prev[i].r = output_acc[i].r;
            output_acc_prev[i].i = output_acc[i].i;
        }

        // Fill 1st half of middle input <- with second half of current input
        for (int i = 0; i < packet_size_half; i++) {
            input_mid[i].r = input[i + packet_size_half].r;
            input_mid[i].i = input[i + packet_size_half].i;
        }
    }

    fprintf(stderr, "[%s] done.\n", argv[0]);
    return EXIT_SUCCESS;
}
