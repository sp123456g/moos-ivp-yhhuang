#!/usr/bin/env python
# coding: utf-8

# In[ ]:


import numpy as np
import soundfile as sf

dur = float(input("Get duration(ex. 0.1):  "))
f0 = int(input('Get the base frequency(4000):  '))
fe = int(input('Get the upperBound frequency(ex. 8000): '))
#fs = int(input('Get the sampling rate(ex. 48000):  '))
fs = 48000
Amp = 1
#Amp = int(input('Get the sound quantity(ex. 1):  '))

#dt = 1/fs
t = np.linspace(0,dur,fs*dur+1)
mu = (fe-f0)/(2*dur)
chirp_signal = Amp*np.cos(2*np.pi*(f0*t+mu*t*t))
filename = './Chirp.wav'
sf.write(filename, chirp_signal, fs, 'PCM_24')


# In[ ]:




