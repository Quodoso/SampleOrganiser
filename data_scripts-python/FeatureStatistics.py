#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Aug 22 14:49:18 2020

@author: lars
"""


import json
import pandas as pd
import numpy as np

from plotting import boxplotOneFeatureAllGTs

def getFeaturesAsArrayByGT(data):
    return np.asarray([np.asarray(a["entries"]) for a in data])

def printFeatureStatistics():
    path =  "/home/lars/Documents/workspace/c++/SampleOrganiser/json_data/featureStats_2020-08-17.10:51:19.json"
    
    jf = {}
    with open(path, "r") as read_file:
        print("Converting JSON encoded data into Python dictionary")
        jf = json.load(read_file)
    
    # select = ["Haralick-FACM_2"]
    data = []
    featureList = []
    for entry in jf["entries"]:
        for feature in entry["features"]:
            data.append({"gt": entry["gt_label"], 
                         "feature": feature["name"],
                         "childs": entry["child_amount"], 
                         "mean": feature["mean"], 
                         "median": feature["median"], 
                         "std" : feature["std"], 
                         "absDev" : feature["absDeviation"], 
                         "mad": feature["mad"], 
                         "min": feature["min"], 
                         "max": feature["max"]})
            if [feature["name"] not in featureList]:
                featureList.append(feature["name"])
    
    
    featureList = ["Haralick-FACM_11", "ZeroCrossingRate"]
    # truncating strings
    # for d in data:
    #     d["feature"] = getTruncatedFeature(d["feature"])
    #     d["gt"] = getTruncatedFeature(d["gt"])
    
    pd.set_option('display.expand_frame_repr', False)
    pd.set_option('display.width', None)
    pd.set_option('display.max_columns', None)
    pd.set_option('display.max_rows', None)
    df = pd.DataFrame(data)
    df.columns
    printAll = True
    for feature in featureList:
        if printAll:
            sliced = df.loc[df['feature'] == feature]
        else:
            if "Haralick" not in feature:
                sliced = df.loc[df['feature'] == feature]
        print(sliced)
     

def filterDataByFeature(data, featureName = "", featureValue = ""):
    resFeats = []
    resGTs = []
    for d in data:
        if d[featureName] == featureValue:
            resFeats.append(d)
            resGTs.append(d["gt"])
    return resFeats, resGTs

    
def plotSingleFeatures():
    path =  "/home/lars/Documents/workspace/c++/SampleOrganiser/json_data/singleStats_2020-08-17.10:51:19.json"
    
    jf = {}
    with open(path, "r") as read_file:
        print("Converting JSON encoded data into Python dictionary")
        jf = json.load(read_file)
    
    # select = ["Haralick-FACM_2"]
    data = []
    featureList = []
    for entry in jf["entries"]:
        for feature in entry["features"]:
            singleOccures = {}
            singleOccures["gt"] = entry["gt_label"]
            singleOccures["feature"] = feature["name"]
            singleOccures["entries"] = []
            for singleEntry in feature["entries"]:
                singleOccures["entries"].append(singleEntry)
            data.append(singleOccures)
            if [feature["name"] not in featureList]:
                featureList.append(feature["name"])
                    
    featureList = ["equalTemperedDeviation","nonTemperedEnergyRatio","nonTemperedPeaksEnergyRatio",
                   "Haralick-FACM_5", "Haralick-FACM_6"]
    featureList = [featureList[0], featureList[-1]]
    feature1, gts = filterDataByFeature(data, featureName = "feature", featureValue = featureList[0])
    feature1 = getFeaturesAsArrayByGT(feature1)
    feature2, _ = filterDataByFeature(data, featureName = "feature", featureValue = featureList[1])
    feature2 = getFeaturesAsArrayByGT(feature2)
    
    boxplotOneFeatureAllGTs(feature1,featureList[0])
    boxplotOneFeatureAllGTs(feature2,featureList[1])
    
    for i in range(len(gts)):
        print("{} is {} with {} entries".format(i+1, gts[i], len(feature1[i])))
           