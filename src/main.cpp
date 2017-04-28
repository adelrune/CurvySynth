#include <stdio.h>
#include <math.h>
#include "portaudio.h"
#include "audio_objects.h"

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 64
#define NUM_SECONDS 4

static int paCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData) {
    SineTableOsc sine = *((SineTableOsc*)userData);
    float* out = (float*) outputBuffer;
    unsigned long i;

    (void)timeInfo; /* Prevent unused variable warnings. */
    (void)statusFlags;
    (void)inputBuffer;

    for (i = 0; i < framesPerBuffer; i++) { 
        float sample = sine.next();
        *(out+i) = sample;
        //printf("%f", sample);
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
    SineTableOsc lfo = SineTableOsc(330.0, 1.0, 110.0);
    SineTableOsc sine = SineTableOsc(1.0, 60.0, 0.0);
    Sig a = Sig(3);
    printf("%f %f %f\n", a.next(), lfo.freq->next(), sine.amp->next());
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
        paCallback, &sine);
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