#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Sep  3 23:40:59 2020

@author: lars
"""

import pygraphviz as pgv
from pygraphviz import *

import matplotlib.pyplot as plt
import matplotlib.image as mpimg

"""
attributes to consider for drawing map of samples:
    ->layouter neato (default) or fdp ot sfdp (multiscale version)
    
    ->edge has:
        -len  := desired length of an edge
        -weight := weight of spring (how "important" is the length). 
    
"""





G=pgv.AGraph(ranksep='0.1') 

G.add_edge("a","b","ab")
ab = G.get_edge("a", "b")
ab.attr["len"] =2

G.add_edge("c","b","cb")
ab = G.get_edge("c", "b")
ab.attr["len"] =2

G.add_edge("c","a","ca")
ab = G.get_edge("c", "a")
ab.attr["len"] =9
ab.attr["weight"] =0

G.layout()

G.draw('file.png')


img = mpimg.imread('file.png')
imgplot = plt.imshow(img)
plt.show()
