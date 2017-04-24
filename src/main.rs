extern crate portaudio;

use portaudio as pa;
mod audio_objects;
use audio_objects::Osc;
use audio_objects::Audio;
use audio_objects::SineTableOsc;
use std::sync::Arc;

const CHANNELS: i32 = 1;
const NUM_SECONDS: i32 = 5;
const SAMPLE_RATE: f64 = 44100.0;
const FRAMES_PER_BUFFER: u32 = 64;


fn main() {
    unsafe{run().unwrap()}
}

unsafe fn run() -> Result<(), pa::Error> {

    let mut freq: i32 = 330;
    let mut offset: i32 = 110;
    let mut change_freq: i32 = 1;
    let mut amp: i32 = 1;
    let mut add: i32 = 0;

    let mut sine2: Arc<SineTableOsc<i32, i32, i32>> = Osc::new(&mut freq, &mut change_freq, &mut offset,  SAMPLE_RATE as f32);
    let mut sine1: Arc<SineTableOsc<SineTableOsc<i32, i32, i32>, i32, i32>> = Osc::new(&mut sine2, &mut amp, &mut add, SAMPLE_RATE as f32);

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