# sow - sound over wifi

Experiment

```
# Download a song (in this case, Tutti Frutti) (48kHz 2ch s16le (2 bytes per sample))
curl https://ia801204.us.archive.org/25/items/78_tutti-frutti_la-bostrie-peniman-pat-boone-with-orchestra_gbia0000491a/_78_tutti-frutti_la-bostrie-peniman-pat-boone-with-orchestra_gbia0000491a_01_2.0-ct_eq.mp3 -o song.mp3

# Check that it works:
mplayer song.mp3

sudo apt-install ffmpeg # to convert mp3 to wav
sudo apt install sox # to convert wav to raw

ffmpeg -i song.mp3 song.wav

# Convert to a raw format, single channel, 44.1kHz, 8 bits per sample,  unsigned integer
sox song.wav -t raw -c 1 -r 44100 -b 8 -e unsigned-integer song.raw

# check that it plays OK
aplay -r 44100 song.raw

pip3 install --user sounddevice
```
