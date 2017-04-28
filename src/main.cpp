#include <stdio.h>
#include <vector>
#include <math.h>
#include "portaudio.h"
#include "audio_objects.h"

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 64
#define NUM_SECONDS 4

int num_harms = 1;

static int paCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData) {
    std::vector<NaiveSineOsc>* sines = (std::vector<NaiveSineOsc>*)userData;
    float* out = (float*) outputBuffer;
    int i;

    (void)timeInfo; /* Prevent unused variable warnings. */
    (void)statusFlags;
    (void)inputBuffer;

    for (i = 0; i < framesPerBuffer; i++) {
        float acc = 0.0;
        for (int j = 0; j < num_harms; j++) {
            acc += (*sines)[j].next();
        }
        float sample = acc/num_harms;
        *(out+i) = sample;
    }

    return paContinue;
};

//wat (called at the end of stream)
static void StreamFinished(void* userData) {
    return;
};

int main() {
    PaStreamParameters outputParameters;
    PaStream * stream;
    PaError err;
    std::vector<NaiveSineOsc> sines;
    sines.reserve(num_harms);
    float fundamental = 330.0;
    for (int i = 0; i < num_harms; i++) {
        printf("freq: %f\n", (i+1.0)*fundamental);
        sines.push_back(NaiveSineOsc(1.0/(i+1), (i+1.0)*fundamental, 0.0));
    }

    err = Pa_Initialize();
    if (err != paNoError) goto error;

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr, "Error: No default output device.\n");
        goto error;
    }
    outputParameters.channelCount = 1;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream( &stream,
        NULL, &outputParameters,
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        paClipOff,
        paCallback, &sines);
    if (err != paNoError) goto error;

    err = Pa_SetStreamFinishedCallback(stream, & StreamFinished);
    if (err != paNoError) goto error;

    err = Pa_StartStream(stream);
    if (err != paNoError) goto error;

    printf("Play for %d seconds.\n", NUM_SECONDS);
    Pa_Sleep(NUM_SECONDS * 1000);

    err = Pa_StopStream(stream);
    if (err != paNoError) goto error;

    err = Pa_CloseStream(stream);
    if (err != paNoError) goto error;

    Pa_Terminate();
    printf("Test finished.\n");

    return err;
    error:
        Pa_Terminate();
    fprintf(stderr, "An error occured while using the portaudio stream\n");
    fprintf(stderr, "Error number: %d\n", err);
    fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
    return err;
};