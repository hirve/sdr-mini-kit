#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <string.h>
#include "kissfft/kiss_fft.h"

int main (int argc, char *argv[]) {

    if (argc != 5) {
        fprintf(stderr, "Usage: %s width height brightness use-Nth-sample\n", argv[0]);
        return EXIT_FAILURE;
    }

    int sample_packet_size = atoi(argv[1]);
    int h = atoi(argv[2]);
    int brightness = atof(argv[3]);
    int nth = atoi(argv[4]);
    char **image;

    kiss_fft_cfg cfg = kiss_fft_alloc(sample_packet_size, 0, 0, 0);
    kiss_fft_cpx input[sample_packet_size], output[sample_packet_size];

    image = calloc(h, sizeof(char *));
    for (int i = 0; i < h; i++) {
        image[i] = calloc(sample_packet_size, sizeof(char));
    }

    fprintf(stderr, "[%s] saving multi raw images to STDOUT, pipe it to video codec to make video stream\n", argv[0]);
    fprintf(stderr, "[%s] width is %i, height is %i, BW, 1 byte per px, IQ is from STDIN ...\n", argv[0], sample_packet_size, h);

    int y = 0, n = 0, is_input_finished = 0;

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

        n++;
        if (n == nth) {
            kiss_fft(cfg, input, output);

            double power_row[sample_packet_size];
            for (int i = 0; i < sample_packet_size; i++) {
                double power = sqrt(output[i].r * output[i].r + output[i].i * output[i].i);
                power_row[((sample_packet_size >> 1) + i) % sample_packet_size] = power;
            }
            for (int i = 0; i < sample_packet_size; i++) {
                image[y][i] = (char)fmin(255.0, power_row[i] * brightness);
            }

            for (int y_out = h - 1; y_out >= 0; y_out--) {
                for (int x_out = 0; x_out < sample_packet_size; x_out++) {
                    putchar(image[(y_out + y + 1) % h][x_out]);
                }
            }

            n = 0;
            y++;
            if (y == h) y = 0;
        }
    }

    fprintf(stderr, "[%s] done.\n", argv[0]);
    return EXIT_SUCCESS;
}
