extern crate portaudio;

use portaudio as pa;
mod oscillators;
use oscillators::Osc;

const CHANNELS: i32 = 1;
const NUM_SECONDS: i32 = 5;
const SAMPLE_RATE: f64 = 44100.0;
const FRAMES_PER_BUFFER: u32 = 64;


fn main() {
    run().unwrap()
}

fn run() -> Result<(), pa::Error> {

    let mut sine1: oscillators::SineTableOsc = oscillators::Osc::new(440.0, 1.0, SAMPLE_RATE as f32);
    let mut sine2: oscillators::SineTableOsc = oscillators::Osc::new(1.0, 110.0, SAMPLE_RATE as f32);

    let pa = try!(pa::PortAudio::new());

    let settings = try!(pa.default_output_stream_settings(CHANNELS, SAMPLE_RATE, FRAMES_PER_BUFFER));

    let callback = move |pa::OutputStreamCallbackArgs { buffer, frames, .. }| {
        for idx in 0..frames {
            buffer[idx] = sine1.next();
            sine1.freq = sine2.next() + 330.0;
        }
        pa::Continue
    };

    let mut stream = try!(pa.open_non_blocking_stream(settings, callback));

    try!(stream.start());

    pa.sleep(NUM_SECONDS * 1_000);

    try!(stream.stop());
    try!(stream.close());

    Ok(())
}