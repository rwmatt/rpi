# sow - sound over wifi

Experiment

## Step 1 - produce RAW files

The first step is to try to achieve lift-off. This means that we need to be able to :
* install prerequisites
* download a song and convert it into a suitable RAW format
* test that we can play the RAW format in Python


```
# Download a song (in this case, Tutti Frutti) (48kHz 2ch s16le (2 bytes per sample))
curl https://ia801204.us.archive.org/25/items/78_tutti-frutti_la-bostrie-peniman-pat-boone-with-orchestra_gbia0000491a/_78_tutti-frutti_la-bostrie-peniman-pat-boone-with-orchestra_gbia0000491a_01_2.0-ct_eq.mp3 -o song.mp3

# Check that it works:
mplayer song.mp3

sudo apt-install ffmpeg # to convert mp3 to wav
sudo apt install sox # to convert wav to raw

ffmpeg -i song.mp3 song.wav

# Convert to a raw format, single channel, 44.1kHz, 8 bits per sample, unsigned integer
sox song.wav -t raw -c 1 -r 44100  -b 8 -e unsigned-integer song.raw

# check that it istill plays OK
aplay   -r 44100 song.raw

pip3 install --user numpy
pip3 install --user sounddevice

python3 01play.py
```

Formats like MP3 and WAV contain header sections, compression algorithms, etc.. We want to get rid of all this, and
produce just a stream of bytes. This is the RAW format. There are two concepts to grasp:
* the sample rate, expressed in kHz, repesenting the number of times per second that we sample the audio to obtain a "volume", `aplay' assumes 8kHz, i.e. 8000 samples per second. This is quite low, and the output will be of fairly low quality. Other popular sample rates are 44.1kHz and 68kHz
* volume encoding. This is how we choose to repesent the volume. We could use 8 bits, 16 bits, and others. We could express the volumes as signed or unsigned, little-endian or big-endian

For convience of playing back on our ESP, we will choose 8 bit unsigned integers
