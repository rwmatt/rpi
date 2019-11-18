import sounddevice as sd
import numpy as np


#arr = np.array( dtype = np.uint8)
arr = np.fromfile("song.raw", dtype =np.uint8)

sd.play(arr,samplerate=44100)
sd.wait()
