#include <SoapySDR/Device.h>
#include <SoapySDR/Formats.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <string.h>

#define SAMPLE_PACKET_SIZE 2048
#define MAX_GAINS 8
#define MAX_GAIN_SIZE 32

int main (int argc, char *argv[]) {

    if (argc != 7) {
        fprintf(stderr, "[%s] usage: ./sdr device antenna gain1=vol1,gain2=vol2 freq samplerate seconds > output_file\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *driver = argv[1];
    char *antenna = argv[2];
    char *gains = argv[3];
    float freq = atof(argv[4]);
    int sample_rate = atoi(argv[5]);
    long seconds = atoi(argv[6]);
    long sample_packets = (long)(sample_rate / SAMPLE_PACKET_SIZE * seconds);

    size_t length;

    SoapySDRKwargs args = {};
    SoapySDRKwargs_set(&args, "driver", driver);
    SoapySDRDevice *sdr = SoapySDRDevice_make(&args);
    SoapySDRKwargs_clear(&args);

    if (sdr == NULL) {
        fprintf(stderr, "[%s] SoapySDRDevice_make fail: %s\n", argv[0], SoapySDRDevice_lastError());
        return EXIT_FAILURE;
    }

    char** names = SoapySDRDevice_listAntennas(sdr, SOAPY_SDR_RX, 0, &length);
    fprintf(stderr, "[%s] RX antennas: ", argv[0]);
    for (size_t i = 0; i < length; i++) fprintf(stderr, "    %s, ", names[i]);
    fprintf(stderr, "\n");
    SoapySDRStrings_clear(&names, length);

    names = SoapySDRDevice_listGains(sdr, SOAPY_SDR_RX, 0, &length);
    fprintf(stderr, "[%s] RX gains: ", argv[0]);
    for (size_t i = 0; i < length; i++) fprintf(stderr, "%s, ", names[i]);
    fprintf(stderr, "\n");
    SoapySDRStrings_clear(&names, length);

    fprintf(stderr, "[%s] setting gains: ", argv[0]);
    char *gain = strtok(gains, ",");
    char gainArr[MAX_GAINS][MAX_GAIN_SIZE];
    int gainCount = 0;
    while (gain != NULL) {
        if (gainCount == MAX_GAINS) {
            fprintf(stderr, "\n[%s] max gains is %i, trying to set more.\n", argv[0], MAX_GAINS);
            return EXIT_FAILURE;
        }
        strcpy(gainArr[gainCount], gain);
        gainCount ++;
        gain = strtok(NULL, ",");
    }
    for (int i = 0; i < gainCount; i++) {
        fprintf(stderr, "%s, ", gainArr[i]);
        char *gainName = strtok(gainArr[i], "=");
        float gainValue = atof(strtok(NULL, "="));
        if (SoapySDRDevice_setGainElement(sdr, SOAPY_SDR_RX, 0, gainName, gainValue) != 0) {
            fprintf(stderr, "\n[%s] set %s gain fail: %s\n", argv[0], gainName, SoapySDRDevice_lastError());
        }
    }
    fprintf(stderr, "\n");

    SoapySDRRange *ranges = SoapySDRDevice_getFrequencyRange(sdr, SOAPY_SDR_RX, 0, &length);
    fprintf(stderr, "[%s] RX freq ranges: ", argv[0]);
    for (size_t i = 0; i < length; i++) { fprintf(stderr, "[%g Hz -> %g Hz], ", ranges[i].minimum, ranges[i].maximum); }
    fprintf(stderr, "\n");
    free(ranges);

    if (SoapySDRDevice_setSampleRate(sdr, SOAPY_SDR_RX, 0, sample_rate) != 0) {
        fprintf(stderr, "[%s] set sample rate fail: %s\n", argv[0], SoapySDRDevice_lastError());
    } else {
        fprintf(stderr, "[%s] sample rate: %i Hz\n", argv[0], sample_rate);
    }
    if (SoapySDRDevice_setAntenna(sdr, SOAPY_SDR_RX, 0, antenna) != 0) {
        fprintf(stderr, "[%s] set antenna fail: %s\n", argv[0], SoapySDRDevice_lastError());
    }

    if (SoapySDRDevice_setFrequency(sdr, SOAPY_SDR_RX, 0, freq, NULL) != 0) {
        fprintf(stderr, "[%s] set frequency fail: %s\n", argv[0], SoapySDRDevice_lastError());
    } else {
        fprintf(stderr, "[%s] frequency: %f kHz\n", argv[0], freq / 1000);
    }

    SoapySDRStream *rxStream = SoapySDRDevice_setupStream(sdr, SOAPY_SDR_RX, SOAPY_SDR_CF32, NULL, 0, NULL);
    if (rxStream == NULL) {
        fprintf(stderr, "[%s] setup stream fail: %s\n", argv[0], SoapySDRDevice_lastError());
        SoapySDRDevice_unmake(sdr);
        return EXIT_FAILURE;
    }
    SoapySDRDevice_activateStream(sdr, rxStream, 0, 0, 0);

    if (sample_packets != 0) {
        fprintf(stderr, "[%s] taking %li seconds of IQ data ...\n", argv[0], seconds);
    }

    complex float buff[SAMPLE_PACKET_SIZE];
    float iq[2];
    char *charOutput[2];

    unsigned long limit = sample_packets == 0 ? 1 : sample_packets;
    int step = sample_packets == 0 ? 0 : 1;
    for (size_t i = 0; i < limit; i += step) {
        void *buffs[] = { buff };
        int flags;
        long long timeNs;
        int ret = SoapySDRDevice_readStream(sdr, rxStream, buffs, SAMPLE_PACKET_SIZE, &flags, &timeNs, 100000);
        if (ret <= 0) { continue; }
        for (int j = 0; j < SAMPLE_PACKET_SIZE; j++) {
            iq[0] = creal(buff[j]);
            iq[1] = cimag(buff[j]);
            for (int i = 0; i < 2; i++) {
                charOutput[i] = (char*)&iq[i];
                for (int k = 0; k < 4; k++) {
                    putchar(charOutput[i][k]);
                }
            }
        }
    }

    SoapySDRDevice_deactivateStream(sdr, rxStream, 0, 0);
    SoapySDRDevice_closeStream(sdr, rxStream);
    SoapySDRDevice_unmake(sdr);

    fprintf(stderr, "\n[%s] done.\n", argv[0]);
    return EXIT_SUCCESS;
}
