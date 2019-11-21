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

## Step 2 - Timing UDP transfer without ESP32

Edit file sowsettings.py to set `host` to the right IP address.

We want to be able to time how long it takes for a song to be transferred from the server to the client. Run the following:
```
python3 02server.py &
python3 02client.py
```

`02server.py` send UDP internet packets, and `02client.py` receives them.

The song Tutti Frutti is transmitted as 53156 blocks of 256 bytes each. The 
song is 2m34s duration. `02client.py` receives it in 1.49s. That 1.49s is 
spread over 53156 blocks, so the delay between each block is 0.028ms 
(=1.49/53156). The test was run on an x86_64 with both server and residing on 
the same machine. So it is not a test of wifi. However, it seems worth a 
gamble to assume that the latency for receiving a packet will be so low that we 
will not have to resort to double-buffering of input.

Using different machines with wired connections, the transfer time was 11.0s, a factor of 10 increase. This is a bit worrying, but not yet siasterous.


The server does what appears to be a curious thing:
* it doubles the size of the RAW song. Data is stored in the even byte, and 0 is stored in the odd byte. This is because the I2S interface on the ESP does not allow for 8-bit data as-is. It accepts it as 16-bit data, and discards the second byte. It does mean that twice as much data is transmitted than is strictly necessary
* the array is padded to a 256 boundary. This makes transmission and reception easier. Data is sent as whole blocks, without needing to worry about the size of the last block


## Step 3 - Timing UDP transfer with ESP32

ESP-IDF v4.0-beta2 was used

Compile the `sow32` project for the ESP32:

```
cd sow32
make

Using make menuconfig if necessary, set the following configurations:
  Example Configuration:
    IP4 address: as per your sowsettings.py
    Port: 2001 (same as sowsettings.py)
  Example Connection Configuration:
    Set Wifi SSID and password for your network

On build completion:
make flash

```

Start the server in a different shell:
```
python3 02server.py
```

Connect to the ESP serial port, using `picocom` or similar. A menu of options is displayed. Type `3` to start the timing sequence for step 3. 
The timing reported on my computer was 161s for the transfer. This means that each transfer of 256 bytes took c. 3ms (161/53156*1000). 
Clearly, transferring date over wifi is an order of magnitude slower.
I will assume, for now, that the latency is still acceptable.


## Step 4 - Playing a hard-coded sample

Test that your speaker has been set up correctly. Use pin D26 for output.

Over the serial port, type `4` to test the functionality. You must wait until the test is completed before trying it again. 


## Step 5 - Playing hard-coded sample with double-buffering

After a lot of experimentation, it seems that fetching and filling the I2S buffer is more time-consuming than I hoped for.
I therefore rsort to double-buffering.

## Step 6

It seems as though 44.1k *might* be too ambition, so let's downsample the song to 8kHz:
```
sox song.wav -t raw -c 1 -r 8000  -b 8 -e unsigned-integer song.raw
... buf info

Test out the re-written server and client:
```
python3 06server.py &
python3 06client.py
```

The server now plays the song in a continuous loop.

