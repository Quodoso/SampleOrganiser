#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Aug 22 14:48:03 2020

@author: lars
"""
import os
import json

def covnertMusicKeyToInteger(key):
    if(key == "Ab"):
        return 0
    if(key == "A"):
        return 1
    if(key == "A#"):
        return 2
    if(key == "Bb"):
        return 3
    if(key == "B"):
        return 4
    if(key == "C"):
        return 5
    if(key == "C#"):
        return 6
    if(key == "Db"):
        return 7
    if(key == "D"):
        return 8
    if(key == "D#"):
        return 9
    if(key == "Eb"):
        return 10
    if(key == "E"):
        return 11
    if(key == "F"):
        return 12
    if(key == "F#"):
        return 13
    if(key == "Gb"):
        return 14
    if(key == "G"):
        return 15
    if(key == "G#"):
        return 16

# def listsContainUnordered(list1=[[]], list2=[[]]):
#     if (len(list1) > len(list2)):
#         tmp= list1
#         list1 = list2
#         list2 = tmp
#     for singlist in list1:
#         if
        
def writeJsonFile(filepath = "./foo.json", data = {"bar":"2000"}):
    with open(filepath, 'w', encoding='utf-8') as f:
        json.dump(data, f, ensure_ascii=False, indent=4)
    print("written dataFile: {}".format(filepath))

def loadJsonFile(filePath):
    jf = {}
    with open(filePath, "r") as read_file:
        print("Converting JSON encoded data into Python dictionary")
        jf = json.load(read_file)
    return jf
        
def makeDir(path):
    path = os.path.abspath(path)
    if not os.path.exists(path):
        os.makedirs(path)


def calcSheetFreqs(delta_f = 10):
    """
    calculates the sheet frequencies with low bound, central and high bound for filter banks
    Parameters
    ----------
    delta_f : frequency distance of the original spectrum
        DESCRIPTION. The default is 10.

    Returns
    -------
    None.

    """
    x = []
    for i in range(-48, 68):
        x = x + [440*2**(i/12)]
    for i in range(1,len(x)):
        x[i] = (x[i-1]+x[i])/2
    