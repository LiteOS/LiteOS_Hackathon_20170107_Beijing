#!usr/bin/env python  
#coding=utf-8  
import numpy as np
import pyaudio  
import wave  
from single_stm32_control import *
#define stream chunk   
from write_color import write_color
chunk = 256 

#open a wav format music  
f = wave.open(r"output.wav","rb")  
#instantiate PyAudio  
p = pyaudio.PyAudio()  
#open stream  
stream = p.open(format = p.get_format_from_width(f.getsampwidth()),  
                channels = f.getnchannels(),  
                rate = f.getframerate(),  
                output = True,
		output_device_index = 2)  
#read data  
data = f.readframes(chunk)  
i = 1
#play stream  
while data:  
    stream.write(data)  
    data = f.readframes(chunk)  
    wave_data = np.fromstring(data, dtype=np.short)
    wave_data.shape = -1, 2
    wave_data = wave_data.T
    time = np.arange(0, chunk) * (1.0 / 44100)
    g,r,b = wave_data[0][:3]
    i += 1
    write_color(g,r,b)

#stop stream  
stream.stop_stream()  
stream.close()  

#close PyAudio  
p.terminate()  
