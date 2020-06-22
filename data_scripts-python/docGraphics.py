#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Aug 25 12:54:43 2020

@author: lars
"""
from scipy import signal
from scipy.fft import fftshift

import matplotlib.pyplot as plt
import numpy as np
import wave
import sys
import librosa

def db(a, a0=1, const = 20):
    if a0 == 1:
        return const* ((np.log(a) / np.log(10)))
    if a0 == 1:
        return const* ((np.log(a) / np.log(10))/(np.log(a0)/np.log(10)))



def plotSpectrogramm(timsig, sr= 44100):
    f, t, Sxx = signal.spectrogram(timsig, sr, window="hamming")
    plt.pcolormesh(t, f, Sxx, shading='gouraud')
    plt.ylabel('Frequenz [Hz]')
    plt.xlabel('Zeit [sec]')
    plt.show()

# soundfile = "/home/lars/Documents/Samples/Dirt-Samples/808/CL.WAV"
soundfile = "/home/lars/Documents/Samples/Dirt-Samples/glitch/007_SN.wav"
soundfile = "/home/lars/Documents/Samples/Dirt-Samples/toys/010_Numbers-Notes.wav"
# def oszi():
    # soundfile = "/home/lars/Documents/Samples/Dirt-Samples/808/CL.WAV"
spf = wave.open(soundfile, "r")
y, sr = librosa.load(soundfile, sr=44100)

plotSpectrogramm(y,sr)
# =============================================================================
# Zeit amplituden diagramm
# =============================================================================
# y = y[:221]
# x = np.asarray([round(i/44100, 8) for i in range(221)])
# fig, ax = plt.subplots(1)
# ax.plot(x,y)
# ax.set_xlabel("Zeit in s")
# ax.set_ylabel("Amplitude float Codierung")
# plt.title("Zeit-Amplituden-Diagramm")

# =============================================================================
# Spektrum
# =============================================================================

# cutoffBeg = 25
# cutoffEnd = 50+cutoffBeg

# x  = np.fft.fft(y, 44100)/len(y)
# x = np.abs(x[:int(len(x)/2)])
# fig, ax = plt.subplots(1)
# ax.plot(x)
# ax.set_xlabel("Frequenz in Hz")
# ax.set_ylabel("Amplitude")
# plt.title("Spektrum")

# =============================================================================
# Linienspektrum
# =============================================================================
# fig, ax3 = plt.subplots(1)
# nx = x[cutoffBeg:cutoffEnd]
# z = np.zeros(22050)[cutoffBeg:cutoffEnd]
# ar = np.arange(22050)[cutoffBeg:cutoffEnd]
# for i in range(len(z)):
#     ax3.vlines(x=ar, ymin=z, ymax=nx, color="blue")
    
# ax3.set_xlabel("Frequenz in Hz")
# ax3.set_ylabel("Amplitude")
# plt.title("Linienspektrum")
# =============================================================================
# LogSpektrum
# =============================================================================
# xlog = np.abs(x[:int(len(x)/2)])
# xlog = db(x, 1, 20)
# fig, ax1 = plt.subplots(1, dpi = 150)
# ax1.plot(xlog)
# ax1.set_xlabel("Frequenz in Hz")
# ax1.set_ylabel("Logarithmische Amplitude in dB FS")
# plt.title("Spektrum")

# dt = 1/44100
# Y = np.fft.fft(y)
# fa = 1.0/dt # scan frequency
# N = int(len(Y)/2)
# X = np.linspace(0, fa/2, N, endpoint=True)

# Y  = np.abs(Y[:N])/N
# fig, ax = plt.subplots(1)
# ax.plot(Y)





# def spektro():
# oszi()