#ifndef AUDIO_OBJECTS
#define AUDIO_OBJECTS
#endif

class Audio {
public:
    virtual float next(){};
};

class Sig :public Audio {
public:
    float val;
    Sig(int val): val((float) val){};
    Sig(float val): val(val){};
    float next();
};

float lin_interpolate(float v1, float v2, float location);

class Oscillator {
protected:
    float cur_index;
public:
    Audio* amp;
    Audio* freq;
    Audio* add;
    Oscillator(Audio* amp, Audio* freq, Audio* add): amp(amp), freq(freq), add(add), cur_index(0.0){};
    Oscillator(float amp, float freq, float add): amp(new Sig(amp)), freq(new Sig(freq)), add(new Sig(add)), cur_index(0.0){};
    Oscillator(Audio* amp, Audio* freq, float add): amp(amp), freq(freq), add(new Sig(add)), cur_index(0.0){};
    Oscillator(Audio* amp, float freq, Audio* add): amp(amp), freq(new Sig(freq)), add(add), cur_index(0.0){};
    Oscillator(float amp, Audio* freq, Audio* add): amp(new Sig(amp)), freq(freq), add(add), cur_index(0.0){};
    Oscillator(float amp, float freq, Audio* add): amp(new Sig(amp)), freq(new Sig(freq)), add(add), cur_index(0.0){};
    Oscillator(float amp, Audio* freq, float add): amp(new Sig(amp)), freq(freq), add(new Sig(add)), cur_index(0.0){};
    Oscillator(Audio* amp, float freq, float add): amp(amp), freq(new Sig(freq)), add(new Sig(add)), cur_index(0.0){};
};

class SineTableOsc: public Oscillator, public Audio {
public:
    float next();
using Oscillator::Oscillator;
};

class NaiveSineOsc: public Oscillator, public Audio {
public:
    float next();
using Oscillator::Oscillator;
};

class PolySineOsc: public Oscillator, public Audio {
public:
    float next();
using Oscillator::Oscillator;
};

class RoughPolySineOsc: public Oscillator, public Audio {
public:
    float next();
using Oscillator::Oscillator;
};

class NaiveTriangleOsc: public Oscillator, public Audio {
public:
    float next();
using Oscillator::Oscillator;
};
