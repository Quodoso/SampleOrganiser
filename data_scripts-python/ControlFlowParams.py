#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Aug 18 11:27:19 2020

@author: lars
"""
import os
import json
import numpy as np

def getStatic():
    """
    These are "static" params for a smoother application flow and fine tuning of some params
    Not all functions are implemented yet
    Returns the necessary Params to run this application
    """
    VISU_PAR = {
# =============================================================================
#  More general Params
# =============================================================================
        # does not consider samples which are longer than this value in [s]
        "delteSampleAbove[s]": 5, 
        # flag for extractring/considering long Samples
        "extractLongs" : False,
         # does not consider samples which are longer than this value in [s]
        "toShort[s]": 0.003,
        # flag for extractring/considering too short Samples
        "extractShort" : False,
        # this might indicate a loop !! 
        "bpmConfidence" : 1, 
        # flag for extractring/considering potential Loops
        "extractLoops" : False,
        
         #compress all features to a range from (0,..,1)  ->getFeatureStack()
        "compress": True,  
        # invert all negative feature values with a total negative correlation ->getPandasCorrelation()
        "invNegative" : True,  
# =============================================================================
#   Application Modes     
# =============================================================================
        # scriptMode := ("clustering", "get_N_Closest", "analyseWithGT", "optimizer") 
        #   "clustering" := group samples into 'n Cluster' not regarding their GT
        #   "get_N_Closest" := select N most similar samples to a reference sample not regarding their GT
        #       requires path of a JSON file which contains the features of one sample (compareFilePath)
        #       requires a number (N) (n_mostSimilar)
        #   "analyseWithGT" := analyse a set of features and evaluate with GT-Labels
        #       it is still possible to cluster within this option and save a landmap and restructure files
        #   "optimizer" := trys a new subset of features and save the new subset, Needs GTs
        #
        # the hiearchy of the application mode is: analyseWithGT (when true, most params below are usefull)
        #                                           clustering (There will be no option  to select features compared to GT)
        #                                           get_N_Closest There will be no option  to select features compared to GT)
        #                                                        -> The best Features calculated and saved will be used ->(getBestFile,getBestFeatureSelektion)
        "scriptMode" : "get_N_Closest",
        #for get_N_Closest  -> This should only contain one file and only the features for one Sample,
        "compareFilePath" : "../json_data/singleFile/Dirt-SamplesSingle2020-10-06.17:26:55.json",
                    
        "n_mostSimilar": 25,

        # path to json files
        "dirName" : "../json_data/", 
        # saved Features of a sample-library
        "fileName": "Dirt-Samples2020-09-14.20:53:18.json",
        
        
# =============================================================================
#   Feature selection and Feature subset creation modes      
# =============================================================================  
        # A fixed set of Features to select by (the names my vary from old JSON-Files to new ones)
        "predefinedFeatures" : False, 
        # You can select Features by yourself if you want. It will refers to the predefined featrues
        # the default set can be generated from the Dirst-samples with suboptimalSearchs default values.
        "defineYoureOwnFeatureSet" : ['Har-Log_-FACM_10', 'MFCC-4', 'MFCC-7', 'Har-RecChr_-FACM_12','TriChr_Centroid', 'ZeroCrossingRate', 'MFCC-8'], 
        # "defineYoureOwnFeatureSet" : ["Har-TriChr_-FACM_12", "MFCC-10"],
        # Select all features with  correlation > suboptimalSearch.second to GT-Labels
        # And discard all features with cross correlation > suboptimalSearch.third
        "suboptimalSearch" : (True,0.3, 0.8), 
        # Only take the nBest Features from suboptimaSearch  (-1 := all)
        "nBest" : 7,
        # Consider all Features or take an approach of above.
        "calcAllFeatures": False, 
        
        #("HillClimber", "Random") optimize features with a) hillclimber b) totaly random
        # maxxHill is the maximum iterationof the hillclimber/ max repeat for Random
        # probHill is the probability for each individual feature to get selected
        # modeHill := ("small", "big", "medium") affects HillClimber
        #        small -> small steps (1-2 changes at a time)
        #        big -> every permutation has equal probability
        #        bigChoice -> bigger steps than "small" but not everything possibe like "big"
        "optimizer" : "HillClimber",   
        "maxHill" : 500,
        "probHill": 0.0000001,
        "modeHill" : "medium",

        
        # amount of cluster to consider with Hierarch
        "nCluster" : 40, 
        # (Hierarch/OPTICS/AffinityPropagation/SpectralClustering)  1st is hierarchial clustering, 2nd is Density based->getClusteringLabels()
        "clusterAlgo" : "Hierarch", 
         # The mode for hierarchichal clustering. ward = minimum variance, average = minimum of average, complete = maximum of each cluster, single = minimum of each cluster
        "hierarchMode" : "average",   

# =============================================================================
# Output Params (save files to folder | draw landmap)
# =============================================================================
         # save folder for copying all audio files
        "saveFolder" : '../estimateSongs/',
         # restructure all files within their new assigned cluster Group/
         # if mode is n_mostSimilar, it is an folder which contains the n_mostSimilar samples
        "copyFilesToFolder" : True,  

        
        # draw a distance landmap with graphviz.
        "graphviz": False,                  
         # graphvizMode := ("clusterBased", "oneFilePerCluster", "minimalSpan") :
            # "minimalSpan" = draw one big landmap without clusters as minimal span tree (not recommended for all Files) 
            # "clusterBased" = draw seperate clusters in one big landmap | 
            # "oneFilePerCluster" = generate one landmap file per cluster) 
        "graphvizMode" : "minimalSpan"
        }
    
    # Same Params for Spectral Clustering. This approach  be will not be taken further
    SpectralClusterParam = {"assign_labels":"kmeans",    #{‘kmeans’, ‘discretize’} default kmeans,
                            "eigen_solver":  "amg",
                           }
    VISU_PAR = {**VISU_PAR, **SpectralClusterParam}
    return VISU_PAR

"""
Iterates through all saved log files and returns best features
"""
def getBestFeatureSelektion(fileName=""):
    
    if os.path.exists(fileName):
        with open(fileName, "r") as read_file:
            jf = json.load(read_file)
            res = {"fList":[*jf["featureList"]]}
            del jf["featureList"]
            jf["featureList"] = False
            jf["predefinedFeatures"] = False
            jf["calcAllFeatures"] = False
            res["STAT"] = jf
        return res
    else:
        # Dirt-Samples2020-09-13.18:30:15.json has -0.4826
        #has a correlation of -0.4745 for dataset with hopsize 256 and windowsize = 4096 -> DirtSamples_2020-09-09.03:13:15.json
        return {"fList":['RecChr_Centroid','Har-Bark27_-FACM_1','Har-TriChr_-FACM_1','Har-Bark28_-FACM_12','Har-Bark27_-GLCM_7','Har-Log_-GLCM_7','Har-Bark27_-FACM_9'],
                "hierarchMode": "average",
                }

def getBestFile(filePath = "../json_data/bestFeatureCorrl/"):
    jsons = [os.path.join(filePath, f) for f in os.listdir(filePath) if os.path.isfile(os.path.join(filePath, f))]
    best = 0
    bestFile = ""
    for jsonF in jsons:
        with open(jsonF, "r") as read_file:
            jf = json.load(read_file)
            if np.abs(jf["labelCorrelation"]) > np.abs(best):
                bestFile = jsonF
                best = jf["labelCorrelation"]
    return bestFile

# print(getBestFeatureSelektion(getBestFile()))