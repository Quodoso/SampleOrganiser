#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Aug 22 14:50:26 2020

@author: lars
"""
import os, glob

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.spatial import distance
from scipy.sparse.csgraph import minimum_spanning_tree
from datetime import datetime

import scipy.cluster.hierarchy as shc

from ControlFlowParams import getStatic


from utilitieFuncs import makeDir

import pygraphviz as pgv
# Optionally depends on graphviz package
import graphviz as gv

STAT = getStatic()



"""
https://programtalk.com/python-examples/IPython.display.SVG/
"""
def _view_dot_graph(graph, filename=None, view=False):
    """
    View the given DOT source.  If view is True, the image is rendered
    and viewed by the default application in the system.  The file path of
    the output is returned.  If view is False, a graphviz.Source object is
    returned.  If view is False and the environment is in a IPython session,
    an IPython image object is returned and can be displayed inline in the
    notebook.
 
    This function requires the graphviz package.
 
    Args
    ----
    - graph [str]: a DOT source code
    - filename [str]: optional.  if given and view is True, this specifies
                      the file path for the rendered output to write to.
    - view [bool]: if True, opens the rendered output file.
 
    """

 
    src = gv.Source(graph)
    if view:
        print("Render landmap or at least I'm trying to save a god damn svg. Beep boop")
        # Returns the output file path
        if ".svg" in filename:
            filename = filename.replace(".svg", "")
        return src.render(filename, view=view)
    else:
        # Attempts to show the graph in IPython notebook
        try:
            __IPYTHON__
        except NameError:
            return src
        else:
            import IPython.display as display
            format = 'svg'
            return display.SVG(data=src.pipe(format))

def plotHistogram(x,savePath):
    fig, axs = plt.subplots(1, 1, sharey=True, tight_layout=True)
    
    # We can set the number of bins with the `bins` kwarg
    axs.hist(x, bins=len(np.unique(x)))
    axs.set_title("Cluster Distribution Histogram")
    
    if(os.path.exists(savePath)):
        plt.savefig(os.path.join(os.path.abspath(savePath), "Histogram.png"))
    else:
        plt.show()



def plotHistograms(x,y, savePath):
    fig, axs = plt.subplots(1, 2, sharey=True, tight_layout=True)
    
    # We can set the number of bins with the `bins` kwarg
    axs[0].hist(x, bins=len(np.unique(x)))
    axs[0].set_title("Annotation Histogram")
    axs[1].hist(y, bins=len(np.unique(y)))
    axs[1].set_title("New Label Histogram")
    
    if(os.path.exists(savePath)):
        plt.savefig(os.path.join(os.path.abspath(savePath), "Histogram.png"))
    else:
        plt.show()

def plotSampleLengthBarChar(le):
    
    res = {}
    res["X<0.001"]   = len(np.where(le < 0.01)[0])
    res["0.001<=X<0.1"] = len(np.where((0.01 < le) & (le < 0.1))[0])
    res["0.1<=X<0.5"] = len(np.where((0.1 < le) & (le < 0.5))[0])
    res["0.5<=X<1"] =  len(np.where((0.5 < le) & (le < 1))[0])
    res["1<=X<2"] =  len(np.where((1 < le) & (le < 2))[0])
    res["2<=X<5"] =  len(np.where((2 < le) & (le < 5))[0])
    res["5<=X<10"] =  len(np.where((5 < le) & (le < 10))[0])
    res["10<=X"] =  len(np.where(le > 10)[0])
    
    keys = [*res]
    values =  res.values()
    
    fig,ax = plt.subplots(constrained_layout=True)
    ax.bar(keys,values)
    
    ax.set_xticks(np.arange(len(keys)))
    ax.set_xticklabels(keys, rotation=65)
    ax.set_yticks(np.arange(0,max(values),50))
    plt.xlabel("Duration [sec]")
    plt.title("Sample length distribution")
    plt.show()
    
def boxplotOneFeatureAllGTs(data, featureName=""):
    fig, ax = plt.subplots()
    ax.set_title('Boxplot of feature: '+featureName)
    ax.boxplot(data)
    plt.show()
    


def plotDendrogram(X, savePath = "", justPath = False):
    plt.figure(figsize=(20, 20))
    plt.title("Dendrogram of subset of samples")
    dend = shc.dendrogram(shc.linkage(X, method=STAT["hierarchMode"]),truncate_mode='level', p=6,  orientation='right',leaf_font_size=12.,)
    if(os.path.exists(savePath)):
        if justPath is False:
            savePath = sorted(glob.glob(os.path.join(savePath, '*/')), key=os.path.getmtime)[-1]
        plt.savefig(os.path.join(os.path.abspath(savePath), "Dendrogram.svg"), dpi=450)
        print("Dendrogram saved to {}".format(savePath))
    else:
        print("{} does not exist".format(savePath))
        plt.show()

def scatterPlot(x,y):
    plt.scatter(x, y)
    plt.show()


def generateLabelBasedGraphviz(featureVector, labelPairs, mode = "cluster"):
# =============================================================================
# Label based (clustering) visualization as a landmap
# Get all features and its labels in groups
# calculate average feature vector from group
# Make minimal Span tree fro each group out of distance matrix
# fit group into subgraph
# connect subgraphs through average feature vector
# =============================================================================
#concate foldername with file name_view_dot_graph

    blackListFeatureNames = ["newLabel", "paths"]    
    whiteListFeautureNames = labelPairs.drop(columns = blackListFeatureNames).keys().tolist()
    if "gt" in whiteListFeautureNames:
        whiteListFeautureNames.remove("gt")
        
    G=pgv.AGraph(ranksep='5')
    G.graph_attr["compound"] = "true"
    disjunctLabels = len(np.unique(labelPairs["newLabel"]))
    averages = []
    clusterRep = []  # for connecting clusters together
    dateTimeObj = datetime.now()
    for label in range(disjunctLabels):
        
        elements = labelPairs[labelPairs["newLabel"] == label]
        paths = [os.path.basename(os.path.dirname(i)) +os.path.splitext(os.path.basename(i))[0] for i in elements["paths"]]
        featureVector = elements[whiteListFeautureNames]
        
        
        if len(featureVector) > 1:
            #calculate average of each feature
            tempList = []
            for feat in whiteListFeautureNames:
                tempList.append(np.average(featureVector[feat]))
            averages.append( np.asarray(tempList))
            
            distanceMatrix = distance.cdist(featureVector, featureVector, 'euclidean')
            
            
            minSpan = minimum_spanning_tree(distanceMatrix).toarray().astype(float)
            minSpan = minSpan *100
            
            #if all elements in cluster are actually the same file
            if np.max(minSpan) == 0.0:
                G.add_subgraph("cluster_"+ str(label), name= "cluster_"+str(label), color="blue",label= "cluster_"+str(label))
                # G.add_node("Avg_"+str(label), shape="box")
                cl = G.get_subgraph("cluster_"+ str(label)) # iterate through cluster
                cl.graph_attr["margin"] = 20
                cl.graph_attr["clusterrank"] = "local"
                
                clusterRep.append(paths[-1])
                cl.add_node(paths[-1])
            else:
                # approximation for spring strength
                spriStren = 1/np.sqrt(minSpan)
                spriStren[spriStren == np.inf] = 0
                spriStren[spriStren == -np.inf] = 0
                
                G.add_subgraph("cluster_"+ str(label), name= "cluster_"+str(label), color="blue",label= "cluster_"+str(label))
                cl = G.get_subgraph("cluster_"+ str(label)) # iterate through cluster
                cl.graph_attr["margin"] = 20
                cl.graph_attr["clusterrank"] = "local"
                clusterSet = False
                for i in range(int(len(minSpan))):
                    for j in range(int(len(minSpan))):
                    # it is an unidirectional graph, no pairs doubled pairs
                    #     continue
                        if minSpan[i][j] != 0:
                            if clusterSet == False:
                                clusterRep.append(paths[i])
                                clusterSet = True
                            cl.add_edge(paths[i], paths[j])
                            edge = cl.get_edge(paths[i], paths[j])
                            # mkey += 1
                            edge.attr["len"] = minSpan[i][j]/10
                            
                            edge.attr["weight"] = spriStren[i][j]
                            # print("added {}-{} with len:{} weight:{}".format(str(i),str(j),minSpan[i][j], spriStren[i][j]))
        else:
            #calculate average of each feature
            tempList = []
            for feat in whiteListFeautureNames:
                tempList.append(np.average(featureVector[feat]))
            averages.append( np.asarray(tempList))
            
            
            G.add_subgraph("cluster_"+ str(label), name= "cluster_"+str(label), color="blue",label= "cluster_"+str(label))
            # G.add_node("Avg_"+str(label), shape="box")
            cl = G.get_subgraph("cluster_"+ str(label)) # iterate through cluster
            cl.graph_attr["margin"] = 20
            cl.graph_attr["clusterrank"] = "local"
            
            clusterRep.append(paths[-1])
            cl.add_node(paths[-1])
        # print("label {}   len of reps {}".format(label, len(clusterRep)))
    
    distanceMatrix = distance.cdist(averages, averages, 'euclidean')
    # distanceMatrix = distanceMatrix[:cut,:cut]
    minSpan = minimum_spanning_tree(distanceMatrix).toarray().astype(float)
    minSpan = minSpan *100
    # approximation for spring strength
    spriStren = 1/np.sqrt(minSpan)
    spriStren[spriStren == np.inf] = 0
    spriStren[spriStren == -np.inf] = 0
    for i in range(int(len(minSpan))):
        for j in range(int(len(minSpan))):
        # it is an unidirectional graph, no pairs doubled pairs
        #     continue
            if minSpan[i][j] != 0:
            # G.add_node(paths[i], shape="box")
                # print("add cluster reps {} and {}".format(i,j))
                G.add_edge(clusterRep[i],clusterRep[j] , key="temp"+str(i)+str(j),ltail="cluster_"+str(i), lhead="cluster_"+str(j))
                edge = G.get_edge(clusterRep[i],clusterRep[j] , key="temp"+str(i)+str(j))
                edge.attr["len"] = minSpan[i][j]/10
                # print("added {}-{} with len:{} weight:{}".format(clusterRep[i],clusterRep[j],minSpan[i][j], spriStren[i][j]))
                
    print("Start calculating DOT-Graphic. grab some coffee")
    G.layout("neato")
    G.graph_attr['overlap']="vpsc"
    G.graph_attr["splines"] = "curved"
    G.graph_attr["rankdir"] = "LR"
    G.graph_attr["mode"] = "sgd"
    G.graph_attr["maxiter"] = 100
    # G.graph_attr['orientation'] = "portrait"
    # G.graph_attr['page'] = "15,6"
    # G.graph_attr['model']="circuit"
    
    graphTime = dateTimeObj.strftime("%Y-%m-%d-%H-%M")
    
    makeDir("../landmaps/"+mode+"/")
    fileName = "../landmaps//"+mode+"/"+mode[:2]+"landmap" + "_" + graphTime 
    G.write( fileName + ".dot")
    _view_dot_graph(G, fileName, True)




def generateSimpleDotMap(distanceVector, filepaths, focusFilePath):
    # Simple Dot Map for N-Most similar features
    G=pgv.AGraph(ranksep='5')
    print("Generate Map")
    mkey = 0
    dateTimeObj = datetime.now()
    
    for i in range(len(filepaths)):
        filepaths[i] = os.path.basename(filepaths[i])
    focusFilePath = os.path.basename(focusFilePath)
    
    for i in range(len(filepaths)):
        G.add_edge(focusFilePath, filepaths[i])
        edge = G.get_edge(focusFilePath, filepaths[i])
        mkey += 1
        edge.attr["len"] = distanceVector[i]*10
        
        edge.attr["weight"] = distanceVector[i]
        print("Start calculating DOT-Graphic. get some coffee")
        G.layout("neato")
        G.graph_attr['mode']= "ipsep"
        G.graph_attr['overlap']="ipsep"
        G.graph_attr['splines']="polyline"
    graphTime = dateTimeObj.strftime("%Y-%m-%d-%H-%M")
        
    makeDir("../landmaps/N_Nearest/")
    fileName = "../landmaps/N_Nearest/"+focusFilePath+"landmap"+ "_" + graphTime 
    G.write( fileName + ".dot")
    _view_dot_graph(G, fileName, True)

def generateMinimalSpanTreeLandmap(featureVector, labels, mode = "Minimal"):
        #concate foldername with file name
        
        paths = [os.path.basename(os.path.dirname(i)) +os.path.splitext(os.path.basename(i))[0] for i in labels["paths"]]
        cut = -1
        
        distanceMatrix = distance.cdist(featureVector, featureVector, 'euclidean')
        distanceMatrix = distanceMatrix[:cut,:cut]
        minSpan = minimum_spanning_tree(distanceMatrix).toarray().astype(float)
            
        G=pgv.AGraph(ranksep='5') 
        
        minSpan = minSpan *100
        # approximation for spring strength
        spriStren = 1/np.sqrt(minSpan)
        spriStren[spriStren == np.inf] = 0
        spriStren[spriStren == -np.inf] = 0
        print("Generate Map")
        mkey = 0
        dateTimeObj = datetime.now()
        for i in range(int(len(minSpan))):
            for j in range(int(len(minSpan))):

                if minSpan[i][j] != 0:
                    G.add_edge(paths[i], paths[j], key = mkey)
                    edge = G.get_edge(paths[i], paths[j], key = mkey)
                    mkey += 1
                    edge.attr["len"] = minSpan[i][j]/10
                    
                    edge.attr["weight"] = spriStren[i][j]
                    # print("added {}-{} with len:{} weight:{}".format(str(i),str(j),minSpan[i][j], spriStren[i][j]))
                
        print("Start calculating DOT-Graphic. get some coffee")
        G.layout("neato")
        G.graph_attr['mode']= "ipsep"
        G.graph_attr['overlap']="ipsep"
        G.graph_attr['splines']="polyline"
        # G.graph_attr["rankdir"] = "LR"
        # G.graph_attr['orientation'] = "portrait"
        # G.graph_attr['page'] = "15,6"
        # G.graph_attr['model']="circuit"
        # G.
        graphTime = dateTimeObj.strftime("%Y-%m-%d-%H-%M")
        
        makeDir("../landmaps/"+mode+"/")
        fileName = "../landmaps/"+mode+"/"+mode[:2]+"landmap"+ "_" + graphTime 
        G.write( fileName + ".dot")
        _view_dot_graph(G, fileName, True)

def generateMultipleLabelBasedGraphviz(featureVector, labelPairs, mode = "Multiple"):
# =============================================================================
#     Save for each cluster a seperate .dot and "PDF" (_-_) file of its representing landmap
# =============================================================================
    disjunctLabels = len(np.unique(labelPairs["newLabel"]))
    
    blackListFeatureNames = ["newLabel", "paths", "gt"]
    
    dateTimeObj = datetime.now()
    for label in range(disjunctLabels):
        
        G=pgv.AGraph(name= "cluster_"+str(label), color="blue",label= "cluster_"+str(label),ranksep='5')
        G.graph_attr["compound"] = "true"
        G.graph_attr["margin"] = 20
        G.graph_attr["clusterrank"] = "local"   
        
        elements = labelPairs[labelPairs["newLabel"] == label]
        paths = [os.path.basename(os.path.dirname(i)) +os.path.splitext(os.path.basename(i))[0] for i in elements["paths"]]
        # generate subset from Dataframe, that there remain only the features of the files within one cluster 
        featureVector = elements[elements[elements["gt"] == label].drop(columns = blackListFeatureNames).keys().tolist()]
        
        
        if len(featureVector) > 1:
            distanceMatrix = distance.cdist(featureVector, featureVector, 'euclidean')
            
            
            minSpan = minimum_spanning_tree(distanceMatrix).toarray().astype(float)
            minSpan = minSpan *100
            
            #if all elements in cluster are actually the same file
            
            if np.max(minSpan) == 0.0:
                G.add_node(paths[-1])
            else:
                # approximation for spring strength
                spriStren = 1/np.sqrt(minSpan)
                spriStren[spriStren == np.inf] = 0
                spriStren[spriStren == -np.inf] = 0
                
                clusterSet = False
                for i in range(int(len(minSpan))):
                    for j in range(int(len(minSpan))):
                    # it is an unidirectional graph, no doubled pairs
                    #     continue
                        if minSpan[i][j] != 0:
                            G.add_edge(paths[i], paths[j])
                            edge = G.get_edge(paths[i], paths[j])
                            # mkey += 1
                            edge.attr["len"] = minSpan[i][j]/10
                            
                            edge.attr["weight"] = spriStren[i][j]
                            # print("added {}-{} with len:{} weight:{}".format(str(i),str(j),minSpan[i][j], spriStren[i][j]))
        else:        
            G.add_node(paths[-1])
            
        print("Start preparing and saving DOT-Graphic for label: {}. get some coffee".format(label))
        G.layout("neato")
        G.graph_attr['overlap']="vpsc"
        G.graph_attr["splines"] = "curved"
        G.graph_attr["rankdir"] = "LR"
        G.graph_attr["mode"] = "sgd"
        G.graph_attr["maxiter"] = 100
        # G.graph_attr['orientation'] = "portrait"
        # G.graph_attr['page'] = "15,6"
        # G.graph_attr['model']="circuit"
        graphTime = dateTimeObj.strftime("%Y-%m-%d-%H-%M")
        
        makeDir("../landmaps/"+mode+"/")
        fileName = "../landmaps//"+mode+"/"+mode[:2]+"landmap" + "_" + graphTime
        G.write( fileName + ".dot")
        _view_dot_graph(G, fileName, True)

def generateGraphviz(featureVector, labels, mode = "clusterBased"):    

    if(mode == "clusterBased"):
        
        generateLabelBasedGraphviz(featureVector, labels)
        
    elif(mode == "oneFilePerCluster"):
        generateMultipleLabelBasedGraphviz(featureVector, labels)
        
    elif(mode == "minimalSpan"):
        
        generateMinimalSpanTreeLandmap(featureVector, labels)
        