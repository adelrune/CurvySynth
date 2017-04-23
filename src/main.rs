extern crate portaudio;

use portaudio as pa;
mod audio_objects;
use audio_objects::Osc;
use audio_objects::Audio;

const CHANNELS: i32 = 1;
const NUM_SECONDS: i32 = 5;
const SAMPLE_RATE: f64 = 44100.0;
const FRAMES_PER_BUFFER: u32 = 64;


fn main() {
    run().unwrap()
}

fn run() -> Result<(), pa::Error> {

    let mut sine2: audio_objects::SineTableOsc = audio_objects::Osc::new(1, 1, 330,  SAMPLE_RATE as f32);
    let mut sine1: audio_objects::SineTableOsc = audio_objects::Osc::new(&sine2, 1, 0, SAMPLE_RATE as f32);

    let pa = try!(pa::PortAudio::new());

    let settings = try!(pa.default_output_stream_settings(CHANNELS, SAMPLE_RATE, FRAMES_PER_BUFFER));

    let callback = move |pa::OutputStreamCallbackArgs { buffer, frames, .. }| {
        for idx in 0..frames {
            buffer[idx] = sine1.next();
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