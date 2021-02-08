#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <string.h>
#include "kissfft/kiss_fft.h"

int main (int argc, char *argv[]) {

    if (argc != 5) {
        fprintf(stderr, "Usage: %s width height brightness output_file_prefix\n", argv[0]);
        return EXIT_FAILURE;
    }

    int sample_packet_size = atoi(argv[1]);
    int h = atoi(argv[2]);
    int brightness = atof(argv[3]);
    char *output_file_name_prefix = argv[4];

    kiss_fft_cfg cfg = kiss_fft_alloc(sample_packet_size, 0, 0, 0);
    kiss_fft_cpx input[sample_packet_size], output[sample_packet_size];

    fprintf(stderr, "[%s] saving multi images, width is %i, height is %i, IQ is from STDIN ...\n", argv[0], sample_packet_size, h);

    int output_file_num = 0, is_input_finished = 0;
    while (is_input_finished == 0) {
        char output_file_name[128] = "", output_file_name_ending[32] = "";
        strcpy(output_file_name, output_file_name_prefix);
        sprintf(output_file_name_ending, "-%06i.pgm", output_file_num);
        strcat(output_file_name, output_file_name_ending);

        fprintf(stderr, "[%s] output file is %s ...\n", argv[0], output_file_name);

        FILE *fo = fopen(output_file_name, "w");
        if (fo == NULL) {
            fprintf(stderr, "[%s] Cannot open output file.\n\n", argv[0]);
            return EXIT_FAILURE;
        }
        fprintf(fo, "P5\n%i %i\n255\n#Power graph\n", sample_packet_size, h);

        for (int y = 0; y < h; y++) {
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

            double power_row[sample_packet_size];
            for (int i = 0; i < sample_packet_size; i++) {
                double power = sqrt(output[i].r * output[i].r + output[i].i * output[i].i);
                power_row[((sample_packet_size >> 1) + i) % sample_packet_size] = power;
            }
            for (int i = 0; i < sample_packet_size; i++) {
                fprintf(fo, "%c", (char)fmin(255.0, power_row[i] * brightness));
            }
        }

        fclose(fo);
        if (++output_file_num == 1000000) {
            output_file_num = 0;
        }
    }

    fprintf(stderr, "[%s] done.\n", argv[0]);
    return EXIT_SUCCESS;
}
