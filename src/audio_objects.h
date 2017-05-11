#ifndef AUDIO_OBJECTS
#define AUDIO_OBJECTS
#endif

class Audio {
public:
    virtual double next(){};
};

class Sig :public Audio {
public:
    double val;
    Sig(int val): val((double) val){};
    Sig(double val): val(val){};
    double next();
};

double lin_interpolate(double v1, double v2, double location);

class Oscillator {
protected:
    double cur_index;
public:
    Audio* amp;
    Audio* freq;
    Audio* add;
    Oscillator(Audio* amp, Audio* freq, Audio* add): amp(amp), freq(freq), add(add), cur_index(0.0){};
    Oscillator(double amp, double freq, double add): amp(new Sig(amp)), freq(new Sig(freq)), add(new Sig(add)), cur_index(0.0){};
    Oscillator(Audio* amp, Audio* freq, double add): amp(amp), freq(freq), add(new Sig(add)), cur_index(0.0){};
    Oscillator(Audio* amp, double freq, Audio* add): amp(amp), freq(new Sig(freq)), add(add), cur_index(0.0){};
    Oscillator(double amp, Audio* freq, Audio* add): amp(new Sig(amp)), freq(freq), add(add), cur_index(0.0){};
    Oscillator(double amp, double freq, Audio* add): amp(new Sig(amp)), freq(new Sig(freq)), add(add), cur_index(0.0){};
    Oscillator(double amp, Audio* freq, double add): amp(new Sig(amp)), freq(freq), add(new Sig(add)), cur_index(0.0){};
    Oscillator(Audio* amp, double freq, double add): amp(amp), freq(new Sig(freq)), add(new Sig(add)), cur_index(0.0){};
};

class SineTableOsc: public Oscillator, public Audio {
public:
    double next();
using Oscillator::Oscillator;
};

class NaiveSineOsc: public Oscillator, public Audio {
public:
    double next();
using Oscillator::Oscillator;
};

class PolySineOsc: public Oscillator, public Audio {
public:
    double next();
using Oscillator::Oscillator;
};

class RoughPolySineOsc: public Oscillator, public Audio {
public:
    double next();
using Oscillator::Oscillator;
};

class NaiveTriangleOsc: public Oscillator, public Audio {
public:
    double next();
using Oscillator::Oscillator;
};

class DirtSineOsc: public Oscillator, public Audio {
public:
    //This constructor syntax is unwieldy... Should find something better.
    DirtSineOsc(Audio* amp, Audio* freq, Audio* add, Audio* dirt):Oscillator(amp, freq, add) {this->dirt = dirt;};
    DirtSineOsc(double amp, double freq, double add, Audio* dirt):Oscillator(amp, freq, add) {this->dirt = dirt;};
    DirtSineOsc(Audio* amp, Audio* freq, double add, Audio* dirt):Oscillator(amp, freq, add) {this->dirt = dirt;};
    DirtSineOsc(Audio* amp, double freq, Audio* add, Audio* dirt):Oscillator(amp, freq, add) {this->dirt = dirt;};
    DirtSineOsc(double amp, Audio* freq, Audio* add, Audio* dirt):Oscillator(amp, freq, add) {this->dirt = dirt;};
    DirtSineOsc(double amp, double freq, Audio* add, Audio* dirt):Oscillator(amp, freq, add) {this->dirt = dirt;};
    DirtSineOsc(double amp, Audio* freq, double add, Audio* dirt):Oscillator(amp, freq, add) {this->dirt = dirt;};
    DirtSineOsc(Audio* amp, double freq, double add, Audio* dirt):Oscillator(amp, freq, add) {this->dirt = dirt;};

    DirtSineOsc(Audio* amp, Audio* freq, Audio* add, double dirt):Oscillator(amp, freq, add) {this->dirt = new Sig(dirt);};
    DirtSineOsc(double amp, double freq, double add, double dirt):Oscillator(amp, freq, add) {this->dirt = new Sig(dirt);};
    DirtSineOsc(Audio* amp, Audio* freq, double add, double dirt):Oscillator(amp, freq, add) {this->dirt = new Sig(dirt);};
    DirtSineOsc(Audio* amp, double freq, Audio* add, double dirt):Oscillator(amp, freq, add) {this->dirt = new Sig(dirt);};
    DirtSineOsc(double amp, Audio* freq, Audio* add, double dirt):Oscillator(amp, freq, add) {this->dirt = new Sig(dirt);};
    DirtSineOsc(double amp, double freq, Audio* add, double dirt):Oscillator(amp, freq, add) {this->dirt = new Sig(dirt);};
    DirtSineOsc(double amp, Audio* freq, double add, double dirt):Oscillator(amp, freq, add) {this->dirt = new Sig(dirt);};
    DirtSineOsc(Audio* amp, double freq, double add, double dirt):Oscillator(amp, freq, add) {this->dirt = new Sig(dirt);};
    Audio* dirt;
    double next();
};
