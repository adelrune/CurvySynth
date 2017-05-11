#include <stdio.h>
#include <vector>
#include <math.h>
#include "portaudio.h"
#include "audio_objects.h"

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 64
#define NUM_SECONDS 45
#define OSCTYPE DirtSineOsc

int num_harms = 1;
double fundamental = 80.0;

static int paCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData) {
    std::vector<OSCTYPE>* sines = (std::vector<OSCTYPE>*)userData;
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

//Tests multiple oscillators to create partials of a sound.
/*void partial_test(std::vector<OSCTYPE>* sines) {
    sines->reserve(num_harms);
    for (int i = 0; i < num_harms; i++) {
        printf("freq: %f\n", (i+1.0) * fundamental);
        sines->push_back(OSCTYPE(1.0/(i+1), (i+1.0) * fundamental, 0.0));
    }
}*/

void dirt_test(std::vector<OSCTYPE>* sines){
    sines->reserve(1);
    sines->push_back(OSCTYPE(1.0, 110.0, 0.0, new NaiveTriangleOsc(0.5, 0.3, 0.5)));
}

//sweep from 20 to 20000hz using a triangle lfo.
/*void sweep_test(std::vector<OSCTYPE>* sines) {
    sines->reserve(1);
    sines->push_back(OSCTYPE(1.0, new NaiveTriangleOsc(9990.0, 0.01, 10010.0), 0.0));
}*/

int main() {
    PaStreamParameters outputParameters;
    PaStream * stream;
    PaError err;
    std::vector<OSCTYPE> sines;

    dirt_test(&sines);

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