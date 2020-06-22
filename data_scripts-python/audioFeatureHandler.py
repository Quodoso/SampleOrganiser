# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import os
import shutil
from datetime import datetime

import pandas as pd
import numpy as np
from sklearn.cluster import OPTICS
from sklearn.cluster import AgglomerativeClustering
from sklearn.cluster import AffinityPropagation
from sklearn.cluster import SpectralClustering
from sklearn.cluster import KMeans
import time

from scipy.spatial import distance
from scipy.sparse.csgraph import minimum_spanning_tree

import seaborn as sn

from numpy.random import Generator, MT19937

from ControlFlowParams import getStatic
from utilitieFuncs import covnertMusicKeyToInteger, makeDir, writeJsonFile, loadJsonFile
from plotting import plotHistogram, plotHistograms, plotSampleLengthBarChar, boxplotOneFeatureAllGTs, plotDendrogram, scatterPlot, generateGraphviz, generateSimpleDotMap, _view_dot_graph



STAT = getStatic()

def noNanAndInf(mat):
    for i in range(len(mat)):
        if not np.isfinite(mat[i]):
            mat[i] = 0.0
    return mat
            
def sortHistogram(narr):
    res = np.array(narr)
    nso = [(i,len(np.where(np.array(narr) == i)[0])) for i in np.unique(np.array(narr))]
    nso.sort(key=lambda t: t[1])
    for i in range(len(nso)):
        res[np.where(res == nso[i][1])] = len(nso) + i
    print(res)
    return res
    
def compress(nparray, newMax=1, newMin=0):
    """compress an numpy array, default is from range 0 to 1"""
    minimum = min(nparray)
    maximum = max(nparray)
    if(maximum - minimum) == 0:
        return nparray
    return (((nparray - minimum)/ ( max(nparray) - minimum)) * newMax + newMin)


def getTruncatedFeature(d= ""):
    if len(d) < 6:
        return d
    return d[0:3]+d[len(d)-3:]

def sliceDataDic(dataArray, featureList=[]):
    res = []
    for data in dataArray:
        if data["feature"] in featureList:
            res.apppend(data)
    return res


def getFeatureStack(df, bestFeatures = [], calcAllFeatures = False, comp = True, verbose = True):
    # bestCorrls =["spread","Haralick-FACM_11","ZeroCrossingRate"]
    
    bestCorrls = bestFeatures
    X = ""
    if calcAllFeatures:
        tmp = ""
        if comp:
            tmp = noNanAndInf(compress(np.asarray(df[df.columns[0]])))
        else:
            tmp = noNanAndInf(np.asarray(df[df.columns[0]]))
        X = tmp.reshape((len(tmp),1))
        for i in range(1,len(df.columns),1):
            if df.columns[i] != "gt":
                print("populate {} features to stack".format(df.columns[i]))
                if compress:
                    tmp = noNanAndInf(compress(np.asarray(df[df.columns[i]])))
                else:
                    tmp = noNanAndInf(np.asarray(df[df.columns[i]]))
                X = np.hstack((X,tmp.reshape((len(tmp),1))))
    else:
        tmp = ""
        if comp:
            tmp = noNanAndInf(compress(np.asarray(df[bestCorrls[0]])))
        else:
            tmp = noNanAndInf(np.asarray(df[bestCorrls[0]]))
        X = tmp.reshape((len(tmp),1))
        for i in range(1,len(bestCorrls),1):
            if comp:
                tmp = noNanAndInf(compress(np.asarray(df[bestCorrls[i]])))
            else:
                tmp = noNanAndInf(np.asarray(df[bestCorrls[i]]))
            X = np.hstack((X,tmp.reshape((len(tmp),1))))
            
    return X


def getClusteringLabels(X, clusterAlgo = "Hierarch", numClust = 20, verbose = True):
    clustering = ""
    if clusterAlgo == "Hierarch":
        mode = "ward"
        if "hierarchMode" in STAT:
            mode = STAT["hierarchMode"]
        if verbose:
            print("AgglomerativeClustering with mode {}".format(mode))
        clustering = AgglomerativeClustering(n_clusters=numClust, affinity='euclidean', linkage=STAT["hierarchMode"])
        dendoCluster = AgglomerativeClustering(n_clusters=None, distance_threshold = 0,linkage=STAT["hierarchMode"])
        dendoCluster.fit(X)
        clustering.fit(X)
    elif clusterAlgo == "KMeans":
        print("Using KMeans")
        clustering = KMeans(numClust).fit(X)
        dendoCluster = AgglomerativeClustering(n_clusters=None, distance_threshold = 0,linkage=STAT["hierarchMode"]).fit(X)
    elif clusterAlgo == "OPTICS":
        print("Density based clustering with OPTICS")
        clustering = OPTICS(min_samples=3, cluster_method = "xi", eps=0.05).fit(X)
        print("There are {} unique labels through DBSCAN".format(len(np.unique(clustering.labels_))))
    elif clusterAlgo == "AffinityPropagation":
        clustering = AffinityPropagation(damping=0.885).fit(X)
        print("There are {} unique labels through AffinityPropagation".format(len(np.unique(clustering.labels_))))
    elif clusterAlgo == "SpectralClustering":
         clustering = SpectralClustering(n_clusters=40, random_state = 1337, 
                                         assign_labels = STAT["assign_labels"], 
                                         eigen_solver = STAT["eigen_solver"],
                                         affinity = "rbf").fit(X)
         dendoCluster = clustering
         print("There are {} unique labels through SpectralClustering".format(len(np.unique(clustering.labels_))))

    if clustering == "":
        return ""
    return clustering.labels_, dendoCluster

def printLabelAndGT(labels):
    for i in labels.index.values:
        print("{:5d}- gt-label:{:3d}  cluster-label:{:3d}".format(i, labels["gt"][i], labels["newLabel"][i]))

def loadFeaturesFromJSONToPandas(fileName = "pandaOptimized_2020-08-17.15:47:08.json",
                                 dirName = "../json_data/", checkFeatureLength = True):
    
    
    jf = loadJsonFile(filePath = os.path.join(dirName,fileName))
    #checks all features if they have the same length
    if checkFeatureLength:
        print("There are {} features".format(len(jf)-2))
        for a in jf:
             print("{:12s} has size {:5d}".format(a, len(jf[a])))
    
    #delte CPP Flow data entry befor converting to pandas
    if "CPP_FLOW_DATA" in jf:
        del jf["CPP_FLOW_DATA"]         
    df = pd.DataFrame(jf)    
    if "gt" in df:
        df['gt'] = df['gt'].astype('category').cat.codes
    if "chords_scale" in df:
        df['chords_scale'] = df['chords_scale'].astype('category').cat.codes
    if "key_scale" in df:
        df['key_scale'] = df['key_scale'].astype('category').cat.codes
    if "chords_key" in df:
        df['chords_key'] = [covnertMusicKeyToInteger(x) for x in df['chords_key']]
        df['chords_key'] = df['chords_key'].astype(np.int8)
    if "key_key" in df:
        df['key_key'] = [covnertMusicKeyToInteger(x) for x in df['key_key']]
        df['key_key'] = df['key_key'].astype(np.int8)
    
    if "RollOff" in df and 'flatness' in df:
        df = df.drop(columns=["RollOff","flatness"])
    
    return df

def createLabelPands(gts, newLabels):
    ob = {"gt":np.asarray(gts),
          "assigned":newLabels}
    ob = pd.DataFrame(ob)
    return ob
        

def getPandasCorrelationMatrix(df, printHeatmap = True, saveHead = False):
    # df_withoutGT = df.drop(columns =["gt"])
    corrMatrix = df.corr()
    if printHeatmap:
        sn_plot  = sn.heatmap(corrMatrix, annot=True)
        sn_plot.figure.subplots_adjust(left = 0.3, bottom = 0.3)
        if saveHead:
            sn_plot.figure.savefig("./heatmap.png")
    return corrMatrix
    
def getPandasCorrelation(df, corrWith = "gt", invNegative = False, verbose = True):
    corrls = df[df.columns[:]].corr()[corrWith][:]
    if invNegative:
        print("negate all negative correlations")
        for feature in df[df.columns[:]]:
            if corrls[feature] < 0:
                df[feature] = df[feature] * -1
    corrls = df[df.columns[:]].corr()[corrWith][:]
    if verbose:
        print(corrls)
    return corrls, df


def getNBestFeatures(gt_corr, corrMatrix, n = 10, suboptimalSearch = (True,0.2, 0.7), verbose = False):
    """
        Select N Features with a according to their correlation matrix
    """
    gt_corr = np.abs(gt_corr)
    gt_corr = gt_corr.sort_values(ascending=False)
    endList = pd.DataFrame()
    
    
    
    
    if suboptimalSearch[0] is True:
        better = gt_corr
        
        # if "gt" in better.columns:
        better.drop(["gt"], inplace=True)
        abort = False;
        
        while(abort is False):
            abort = True
            
            if len(better) == 0:
                break
            better = better[better > suboptimalSearch[1]] #get only features wich correlate with gt
            
            print("{} are in more correlated list".format(len(better)))
            name = better.index.values[0] # get best feature
            endList[name] = better[name] #add to new list
            better = better.drop(index=[name]) #delete from better list
            # get all features which correlate to much with assigned feature
            # they won't get selected
            entry = corrMatrix[name]
            entry = entry.drop(index=[name])
            entry = np.abs(entry)
            entry = entry.sort_values(ascending=False)
            entry = entry[entry > suboptimalSearch[2]]
            
            print("{} are in to deleting list".format(len(entry)))
            if(len(entry) != 0):
               print("\nremove follwing entries")
               print(entry)
            if(len(entry) == 0):
                print("best features are:")
                print(better)
                for goodIndex in range(len(better)):
                    name = better.index.values[goodIndex] # get best feature
                    endList[name] = better[name] #add to new lis
            # else:
            #     abort = False
            # remove all features which correlate to much with assigned feature
            for index in entry.index.values:
                if index in better:
                    better = better.drop(index=[index])
                if index in corrMatrix:
                    corrMatrix = corrMatrix.drop(columns=[index],index=[index])
                abort = False
        endList = endList[endList.columns[:]].keys()
    else:
        endList = gt_corr.keys()[1:n+1]
    if "nBest" in STAT:
        endList = endList[:STAT["nBest"]]
    if verbose:
        for best in endList:
            print("{} with {}".format(best, gt_corr[best]))
    return endList


def restructureN_NearestSamples(filePaths, focusFilePath, saveFolder = "../estimateSongs/Nearest/", verbose = True):

    dateTimeObj = datetime.now()
    # print(filePaths)
    saveFolder += os.path.basename(os.path.splitext(focusFilePath)[0]) + dateTimeObj.strftime("%Y-%m-%d-%H-%M")
    saveFolder = os.path.abspath(saveFolder)
    
    makeDir(saveFolder)

    for path in filePaths:
        foldername = os.path.basename(os.path.dirname(path))
        filename = os.path.basename(path)
        filename = foldername + "_-_" + filename
        filename = filename.replace(" ","_")
        destPath = os.path.abspath(os.path.join(saveFolder, filename))
        shutil.copy2(path, os.path.abspath(destPath))
        if verbose:
            print("written file: {}".format(os.path.abspath(destPath)))
    foldername = os.path.basename(os.path.dirname(focusFilePath))
    filename = os.path.basename(focusFilePath)
    filename = foldername + "_-_" + filename
    filename = filename.replace(" ","_")
    destPath = os.path.abspath(os.path.join(saveFolder, filename))
    shutil.copy2(path, os.path.abspath(destPath))
    

def restructureFiles(pandaLabels, saveData = False, saveFolder = "../estimateSongs/", verbose = False):
    
    dateTimeObj = datetime.now()
    folderName = dateTimeObj.strftime("%Y-%m-%d-%H-%M")
    folderName = STAT["hierarchMode"] + "_" + folderName
    # create folderstructer -> ./estimateSongs/timestamp/label/bluberblaba.wav
    papaDir = os.path.abspath(saveFolder)
    papaDir = os.path.join(papaDir, folderName)
    makeDir(papaDir)
    
    
    pandaLabels = pandaLabels.sort_values(by=["newLabel"])
    newLabels = np.unique(pandaLabels["newLabel"])
    
    # copy each wav file in its new labeled folder
    for label in newLabels:
        kidoDir = os.path.join(papaDir, str(label))
        makeDir(kidoDir)
        for path in pandaLabels[pandaLabels["newLabel"] == label]["paths"]:
            foldername = os.path.basename(os.path.dirname(path))
            filename = os.path.basename(path)
            filename = foldername + "_-_" + filename
            filename = filename.replace(" ","_")
            destPath = os.path.abspath(os.path.join(kidoDir, filename))
            shutil.copy2(path, os.path.abspath(destPath))
            if verbose:
                print("written file: {}".format(os.path.abspath(destPath)))
            
    processData = STAT.copy()
    processData.update(STAT.copy())
    
    writeJsonFile(os.path.join(papaDir, "_programmData.json"), processData )
    
    if "gt" in pandaLabels.columns:
        plotHistograms(pandaLabels["gt"], pandaLabels["newLabel"], papaDir)
    else:
        plotHistogram(pandaLabels["newLabel"], papaDir)

def test_N_RandomFeatures(fileName = "pandaOptimized_2020-08-17.15:47:08.json"):
      #fileName can be an actual filName to an feature-file or a pandas data frame
    # load json-file into panda DataFrame
    df = ""
    if isinstance(fileName, str):
        df = loadFeaturesFromJSONToPandas(fileName= STAT["fileName"])
    else:
        df = fileName
    
    # create param data for later saving
    saveData = {"fileLoaded":fileName}
    
    # Here aussondern von loops und longs
    df, df_loopy, df_toLong ,df_toShort =  extractLoopsShortsLongs(df)
    
    # extract file paths
    # filePaths = df["file_path"]
    df = df.drop(columns=["file_path"])
    
    print("\n\n"+"calculcate correlation with pandas")
    pd.set_option('display.max_rows', None)
    corrls, df = getPandasCorrelation(df, corrWith = "gt",
                                      invNegative = STAT["invNegative"],verbose = False)
    
    # pd.set_option('display.max_rows', 25)
    # corrMatrix = getPandasCorrelationMatrix(df, printHeatmap = False)
    featureListOld = []
    maxIt = 2
    if "maxHill" in STAT:
        maxIt = STAT["maxHill"]
    
    for j in range(maxIt):
        optimizerLength = 1
        rg_integers = ""
        bestList = {"eva": 0,
                    "fList" : []}
        rg = Generator(MT19937(int(time.time())))
        rg.random()
        optimizerLength = 50
        try:
            rg_integers = rg.integers
        except AttributeError:
            rg_integers = rg.randint
        for i in range (optimizerLength):
            featureList = []
            if "optimizer" in STAT and STAT["optimizer"] == "Random":
                featureAmount = rg_integers(j+1) + 2
                while len(featureList) < featureAmount:
                    poss = rg_integers(len(df.columns[:].values))
                    if df.columns[:].values[poss] not in featureList and df.columns[:].values[poss] != "gt":
                        featureList.append(df.columns[:].values[poss])
                    else:
                        featureAmount -=1 
                featureList = np.asarray(featureList)
            if len(featureListOld) == 0:
                featureListOld = np.asarray(featureList)

            # print("\n\n"+"create feature stack")
            X = getFeatureStack(df = df, bestFeatures = featureList, comp = STAT["compress"],
                        calcAllFeatures = STAT["calcAllFeatures"], verbose= False)
            
            # print("\n\n"+"start clustering")
            labels, cluster = getClusteringLabels(X = X, clusterAlgo = STAT["clusterAlgo"], 
                                         numClust = STAT["nCluster"],  verbose = False)
            
            l_df = createLabelPands(df["gt"], labels)
            
            # print("\n\n"+"Correlation between new clusters and GTs")
            l_corrls, l_df = getPandasCorrelation(df = l_df, corrWith = "gt", verbose = False)
            print("it:{} elems changed:{} lengthNew:{}, oldEva: {}, newEva:{}".format(j, len(featureListOld[np.isin(featureListOld,featureList)]), len(featureList),l_corrls["assigned"],bestList["eva"]))           
            
            featureListOld = featureList
            if np.abs(l_corrls["assigned"]) > np.abs(bestList["eva"]):
                bestList["fList"] = featureList
                bestList["eva"] = l_corrls["assigned"]
                print("{}  from features".format(bestList["eva"]))
                print(featureList)
        saveData["labelCorrelation"] =  bestList["eva"]
       
        print("\n\n!!!!!!!!-Best value:{} is with {}-features:\n".format(bestList["eva"], len(bestList["fList"])))
        print(featureList)
        print("\n\n")
        # save best featurelist as json, together with all params
        saveData["featureList"] = []
        for feat in bestList["fList"]:
            saveData["featureList"].append({ feat : corrls[feat]})
        
        fileName = str(np.abs(bestList["eva"])).replace(".", "-") +".json"
        writeJsonFile(os.path.join(os.path.abspath("../json_data/bestFeatureCorrl/"), fileName) , data = {**saveData, **STAT})
        
        return bestList["fList"]

def hillMutate(insertRemoveList, allIndiviuals, mutProb = -1.0, mode = "small"):
    """
    Toy mutation for hillclimber 
    insertRemoveList -> feature is in or out (insert/remove problem)
    allIndiviuals    -> The set of all possible indivuduals    
    mode -> ("small","big") "small"-Mutation := addes or removes one feature per iteration
                                                    "big"- each feature as a probability to be inserted/removed
    """
    
    allIndiviuals_copy = np.array(allIndiviuals, copy=True)
    insertRemoveList_copy = np.array(insertRemoveList, copy=True)
    result = ""
    if mutProb <= 0.0:
            mutProb = 1/(len(insertRemoveList_copy))
    if mode == "medium":
        norm = int(np.min([np.abs(np.random.normal(8,8)),1]))
        if norm < 1:
            norm = 1
        if (len(insertRemoveList) <=1):
            size = np.ceil(norm)
        else:
            probFifty = np.random.sample((1,))
            if (probFifty >= 0.75) and (len(insertRemoveList_copy) >= 2):
                size = np.ceil(len(insertRemoveList_copy)/norm)
            else:
                size = np.ceil(len(insertRemoveList_copy) * norm)
        result = np.random.choice(allIndiviuals_copy, int(size))
    if mode == "big":
        inOrOut = np.random.random_sample((len(allIndiviuals_copy),))
        result =  np.asarray(allIndiviuals_copy)[np.where(inOrOut <= 0.1)]
    else:
        repeat = int(np.min([np.abs(np.random.normal(5,2)),1]))
        while(repeat >= 0):
            repeat-=1
            # 50-50 decision for removing one feature or adding one
            # except for the feature lists with only one element
            probFifty = np.random.sample((1,))
            if (probFifty >= 0.5) and (len(insertRemoveList_copy) >= 2):
                ind = np.random.choice(insertRemoveList_copy, 1, False)
                result=  insertRemoveList_copy[np.isin(insertRemoveList_copy, ind, invert=True)]
            else:
                allIndiviuals_copy = allIndiviuals_copy[np.isin(allIndiviuals_copy, insertRemoveList_copy[:1], invert=True)]
                ind = np.random.choice(allIndiviuals_copy, 1, False)
                result = np.append(insertRemoveList_copy, ind)
        return result
    return result

def extractLoopsShortsLongs(df):
    """ routine for extracting Loops, Short and Long Samples, if appropriately flagged
    
        @param df := panda DataFrame
        
        return df := DataFrame without following entries
        df_loopy  := contains all "possible" Loops
        df_toLong := contains all too long samples
        df_toShort := contains all too short samples
    """
    df_loopy = ""
    df_toLong = ""
    df_toShort = ""
    if ("extractLoops" in STAT) and (STAT["extractLoops"]):
        print("Extract Samples which are probably a loop")
    
        df_loopy = df[df["Rhythm13-confidence"] >= STAT["bpmConfidence"]]
        df = df[df["Rhythm13-confidence"] < STAT["bpmConfidence"]]
        
        print("{} samples are probably loops".format(len(df_loopy)))


    if ("extractLongs" in STAT) and (STAT["extractLongs"]):
        print("Extract Samples which are too long")
        df_toLong = df[df["sample_length"] > STAT["delteSampleAbove[s]"]]
        df = df[df["sample_length"] <= STAT["delteSampleAbove[s]"]]
        
        print("{} samples are above {} seconds".format(len(df_toLong),STAT["delteSampleAbove[s]"]))
    
    if ("extractShort" in STAT) and (STAT["extractShort"]):
        print("Extract Samples which are too short")
        df_toShort = df[df["sample_length"] < STAT["toShort[s]"]]
        df = df[df["sample_length"] >= STAT["toShort[s]"]]
        print("{} samples are short than {} seconds".format(len(df_toLong),STAT["delteSampleAbove[s]"]))
        
    return df, df_loopy, df_toLong, df_toShort

def test_N_HillClimberFeatures(fileName = "pandaOptimized_2020-08-17.15:47:08.json", verbose = True):
      #fileName can be an actual filName to an feature-file or a pandas data frame
    # load json-file into panda DataFrame
    df = ""
    if isinstance(fileName, str):
        df = loadFeaturesFromJSONToPandas(fileName= STAT["fileName"], checkFeatureLength=(False))
    else:
        df = fileName
    
    # create param data for later storage
    saveData = {"fileLoaded":fileName}
    
    # extract possible loops  and too long/short samples
    df, df_loopy, df_toLong ,df_toShort =  extractLoopsShortsLongs(df)

        
    # extract file paths
    # filePaths = df["file_path"]
    if "file_path" in df:
        df = df.drop(columns=["file_path"])
    
    # print("\n\n"+"calculcate correlation with pandas")
    pd.set_option('display.max_rows', None)
    corrls, df = getPandasCorrelation(df, corrWith = "gt",
                                      invNegative = STAT["invNegative"],verbose = False)
    
# delete all correlated above 0.9
    # corrMatrix=df.corr().abs()
    # upperMatrix = corrMatrix.where(np.triu(np.ones(corrMatrix.shape), k=1).astype(np.bool))
    # corrFutures = [column for column in upperMatrix.columns if any(upperMatrix[column] > 0.90)]
    # df = df.drop(columns=corrFutures)
    print("There are {} Features left after deleting correlated features".format(len(df.keys().to_numpy())))
    
    bestList = {"eva": 0.0,
            "fList" : []}
    
    saveData["featureList"] = []
    maxIt = 50
    if "maxHill" in STAT:
        maxIt = STAT["maxHill"]
    
    allFeatureList = df.drop(columns=["gt"])
    allFeatureList = allFeatureList.columns[:].values
    
    
    featureListOld = hillMutate([], allFeatureList, 1/(len(allFeatureList)**10), "bigChoice")
    modeHill = "small"
    probHill = -1
    if "modeHill" in STAT:
        modeHill = STAT["modeHill"]
    if "probHill" in STAT:
        probHill = STAT["probHill"]
    for j in range(maxIt):

        featureList = hillMutate(featureListOld, allFeatureList, probHill, modeHill)
        
        # print("\n\n"+"create feature stack")
        X = getFeatureStack(df = df, bestFeatures = featureList, comp = STAT["compress"],
                    calcAllFeatures = STAT["calcAllFeatures"], verbose= False)
        
        # print("\n\n"+"start clustering")
        labels, cluster = getClusteringLabels(X = X, clusterAlgo = STAT["clusterAlgo"], 
                                     numClust = STAT["nCluster"],  verbose = False)
        
        l_df = createLabelPands(df["gt"], labels)
        
        # print("\n\n"+"Correlation between new clusters and GTs")
        l_corrls, l_df = getPandasCorrelation(df = l_df, corrWith = "gt", verbose = False)
        
        if verbose:
            print("it:{} elems changed:{} lengthNew:{}, oldEva: {}, newEva:{}".format(j, len(featureListOld[np.isin(featureListOld,featureList, invert=True)]), len(featureList),l_corrls["assigned"],bestList["eva"]))           
        if np.abs(l_corrls["assigned"]) >= np.abs(bestList["eva"]):
        # if  l_corrls["assigned"] >= bestList["eva"]:
            bestList["fList"] = featureList
            bestList["eva"] = l_corrls["assigned"]
            # print("{}  from features".format(bestList["eva"]))
            # print(featureList)
            print("update old feature list")

            featureListOld = featureList
    print("\n\n!!!!!!!!-Best value:{} is with {}-features:\n".format(bestList["eva"], len(bestList["fList"])))
    print(featureList)
    print("\n\n")
    # save best featurelist as json, together with all params
    saveData["featureList"] = []
    for feat in bestList["fList"]:
        saveData["featureList"].append({ feat : corrls[feat]})
    
    fileName = str(np.abs(bestList["eva"])).replace(".", "-") +".json"
    writeJsonFile(os.path.join(os.path.abspath("../json_data/bestFeatureCorrl/"), fileName) , data = {**saveData, **STAT})
    return bestList["fList"]
            
            
# =============================================================================
#     End of heuristic approach        
# =============================================================================
    for feat in bestList["fList"]:
        saveData["featureList"].append({ feat : corrls[feat]})
    print("\n\n!!!!!!!!-Best value:{} is with {} features:\n".format(bestList["eva"], len(bestList["fList"])))
    print(bestList["fList"])
    print("\n\n")
    # save best featurelist as json, together with all params
    saveData["featureList"] = []
    for feat in bestList["fList"]:
        saveData["featureList"].append({ feat : corrls[feat]})
    fileName = str(np.abs(bestList["eva"])).replace(".", "-") +".json"
    
    writeJsonFile(os.path.join(os.path.abspath("../json_data/bestFeatureCorrl/"), fileName) , data = {**saveData, **STAT})



def dataAnalysisPipeline(fileName = "pandaOptimized_2020-08-17.15:47:08.json" ):
    """
    Parameters
    ----------
    fileName : string, path of 
        DESCRIPTION. The default is "pandaOptimized_2020-08-17.15:47:08.json".

    Returns
    -------
    endLabels : TYPE
        DESCRIPTION.
    saveData : TYPE
        DESCRIPTION.
    df : TYPE
        DESCRIPTION.
    l_df : TYPE
        DESCRIPTION.
    X : TYPE
        DESCRIPTION.
    cluster : TYPE
        DESCRIPTION.

    """
    # load json-file into panda DataFrame
    df = loadFeaturesFromJSONToPandas(fileName= STAT["fileName"], checkFeatureLength=False)
    
    # create param data for later saving
    saveData = {"fileLoaded":fileName}
    
    # Here aussondern von loops und longs
    df, df_loopy, df_toLong ,df_toShort =  extractLoopsShortsLongs(df)
        
    # extract file paths
    filePaths = df["file_path"]
    df = df.drop(columns=["file_path"])
    
    print("\n\n"+"calculcate correlation with pandas")
    pd.set_option('display.max_rows', None)
    
    corrls, df = getPandasCorrelation(df, corrWith = "gt",
                                      invNegative = STAT["invNegative"],verbose = True)
    
    pd.set_option('display.max_rows', 25)
    # xf = df[['AutoCorrelation_sig', 'Bark28_Centroid', 'Mel96_SpectralFlux', 'ZeroCrossingRate']]

    corrMatrix = getPandasCorrelationMatrix(df, printHeatmap = False)
    
    print("\n\n"+"populate features")
    featureList = []

    if(STAT["predefinedFeatures"] == False):
        featureList = getNBestFeatures(corrls, corrMatrix, n=STAT["nBest"], 
                                       suboptimalSearch = STAT["suboptimalSearch"], verbose = True)
        print(featureList)
        feats = featureList.values.tolist()
    else:
        # predefined 
        # -0.494
        # featureList = ['Har-Log_-FACM_10', 'MFCC-4', 'MFCC-7', 'Har-RecChr_-FACM_12','TriChr_Centroid', 'ZeroCrossingRate', 'MFCC-8']
        # -0.484
        # featureList=  ["Har-TriChr_-FACM_12","MFCC-10"]
        featureList = STAT["defineYoureOwnFeatureSet"]
    # Saving chosen features with its correlation-value
    saveData["featureList"] = []
    feats = featureList
    print("use the following features with their correlation to GT:")
    for feat in feats:
        saveData["featureList"].append({ feat : corrls[feat]})
        print("{} : {}".format(feat, corrls[feat]))
    
    
    print("\n\n"+"create feature stack")
    X = getFeatureStack(df = df, bestFeatures = featureList, comp = STAT["compress"],
                calcAllFeatures = STAT["calcAllFeatures"], verbose= False)
    
    print("\n\n"+"start clustering")
    labels, cluster = getClusteringLabels(X = X, clusterAlgo = STAT["clusterAlgo"], 
                                 numClust = STAT["nCluster"],  verbose = True)
    
    l_df = createLabelPands(df["gt"], labels)
    
    print("\n\n"+"Correlation between new clusters and GTs")
    l_corrls, l_df = getPandasCorrelation(df = l_df, corrWith = "gt", verbose = True)
    saveData["labelCorrelation"] =  l_corrls["assigned"]
   

    #ugly converts from different pandas, numpys and lists....
    onlyLabels = df["gt"].values
    indices = df["gt"].index.to_numpy()
    onlyLabels.reshape(len(onlyLabels),1)
    indices.reshape(len(indices),1)
    labels.reshape(len(labels),1)
    
    indices = np.vstack((indices,onlyLabels))
    indices = np.vstack((indices,labels))
    
    data = indices[1].reshape(len(indices[1]),1)
    data  = np.hstack((data,indices[2].reshape(len(indices[2]),1)))
    endLabels = pd.DataFrame(data, columns=["gt","newLabel"], index = indices[0])
    endLabels.insert(len(endLabels.columns),"paths",filePaths)

    for i in range(len(featureList)):
        endLabels.insert(len(endLabels.columns),featureList[i],X[:,i:i+1])

    
    # only add the extracted samples, if they are extracted
    if ("extractLoops" in STAT) and (STAT["extractLoops"]):
        df_loopy = df_loopy[["gt","file_path"]].rename(columns={"file_path":"paths"})
        df_loopy["newLabel"] = len(np.unique(endLabels["newLabel"]))
        endLabels = endLabels.append(df_loopy)
    if ("extractLongs" in STAT) and (STAT["extractLongs"]):
        df_toLong = df_toLong[["gt","file_path"]].rename(columns={"file_path":"paths"})
        df_toLong["newLabel"] = len(np.unique(endLabels["newLabel"]))
        endLabels = endLabels.append(df_toLong)
    if ("extractShort" in STAT) and (STAT["extractShort"]):
        df_toShort = df_toShort[["gt","file_path"]].rename(columns={"file_path":"paths"})
        df_toShort["newLabel"] = len(np.unique(endLabels["newLabel"]))
        endLabels = endLabels.append(df_toShort)
      
    #sort at the end
    endLabels = endLabels.sort_values(by=["gt"])
    
    return endLabels, saveData, df, l_df, X, cluster
    
    
# =============================================================================
#  TO-DOs:   
#   -Loops can be detected with Rhythm13-confidence if value is
#       -(1.5, 3.5] good confidence, accuracy around 80%
#       -(3.5, 5.32] excellent confidence
#  -This may find Loops :)  -> #spoiler: it doesn't
# 
#   -ridiculous short sounds detection and also remove from main group
#        -> for example bird3_/002_11.wav  0.025 secs long (1119 samples)
#                                        at 0.011 (487 samples) is just silence
# 
# Methodical error! -> you dipshit inserted the sound paths AFTER sorting...
#                       Realy?
#       
# =============================================================================


# df = loadFeaturesFromJSONToPandas(fileName= STAT["fileName"])
# plotSampleLengthBarChar(np.array(df["sample_length"]))


def regroupSamplesByClustering(fileName = ""):
# =============================================================================
#     Load files
#     convert pandas
#     get features by CONTROLFLOWPARAMS specification
#     calc Distance from that file to every other
#     select n
#     print results (optional graphviz)
#     optional copying of sample files in new folder
# =============================================================================

    saveData = {}
    # load json-file into panda DataFrame
    df = loadFeaturesFromJSONToPandas(fileName= fileName)
    
    # Here aussondern von loops und longs
    df, df_loopy, df_toLong ,df_toShort =  extractLoopsShortsLongs(df)
        
    # extract file paths
    filePaths = df["file_path"]
    df = df.drop(columns=["file_path"])
    
    print("\n\n"+"calculcate correlation with pandas")
    pd.set_option('display.max_rows', None)
    # corrls, df = getPandasCorrelation(df, corrWith = "gt",
    #                                   invNegative = STAT["invNegative"],verbose = True)
    
    pd.set_option('display.max_rows', 25)
    # corrMatrix = getPandasCorrelationMatrix(df, printHeatmap = False)
    
    print("\n\n"+"populate features")
    featureList = []

    #Get Features t
    # they all require GTs
    # if(STAT["optimizer"] == "HillClimber"):
    #     featureList = test_N_HillClimberFeatures(df)
    # elif(STAT["optimizer"] == "Random"):
    #     featureList = test_N_RandomFeatures(df)
    # elif(STAT["predefinedFeatures"] == False):
    #     featureList = getNBestFeatures(corrls, corrMatrix, n=STAT["nBest"], 
    #                                    suboptimalSearch = STAT["suboptimalSearch"], verbose = False)
    if(STAT["predefinedFeatures"] == False or STAT["predefinedFeatures"]):
        featureList = STAT["defineYoureOwnFeatureSet"]
    print("use these features: ", featureList)
    print("\n\n"+"create feature stack: ", featureList)
    X = getFeatureStack(df = df, bestFeatures = featureList, comp = STAT["compress"],
                calcAllFeatures = STAT["calcAllFeatures"], verbose= False)
    
    print("\n\n"+"start clustering")
    labels, cluster = getClusteringLabels(X = X, clusterAlgo = STAT["clusterAlgo"], numClust = STAT["nCluster"],  verbose = True)
    # asd

    endLabels = pd.DataFrame(labels.reshape(len(labels),1), columns = ["newLabel"], index = [i for i in range(2017)])
    endLabels.insert(len(endLabels.columns),"paths",filePaths)

    for i in range(len(featureList)):
        endLabels.insert(len(endLabels.columns),featureList[i],X[:,i:i+1])

    endLabels = endLabels.sort_values(by=["newLabel"])
    
    saveData["featureList"] = featureList
            
    if ("graphviz" in STAT) and (STAT["graphviz"]):
        generateGraphviz(X, endLabels, STAT["graphvizMode"])    
    if ("copyFilesToFolder" in STAT) and (STAT["copyFilesToFolder"] is True):
        restructureFiles(endLabels,saveData = saveData, saveFolder = os.path.join(STAT["saveFolder"], "withoutGT"))
   

def getNSimilarSamples(singleFile = "", library = ""):
# =============================================================================
#     Load files
#     convert pandas
#     get features by CONTROLFLOWPARAMS specification
#     calc Distance from that file to every other
#     select n
#     print results (optional graphviz)
#     optional copying of sample files in new folder
# =============================================================================
# load json-file into panda DataFrame

    saveData = {}
    dfAll = loadFeaturesFromJSONToPandas(library)
    
    dfSingle = loadFeaturesFromJSONToPandas(singleFile)
    
    df = pd.concat([dfAll,dfSingle])
    
    # Here aussondern von loops und longs
    df, df_loopy, df_toLong ,df_toShort =  extractLoopsShortsLongs(df)
        
    # extract file paths
    filePaths = df["file_path"]
    df = df.drop(columns=["file_path"])
    
    print("\n\n"+"calculcate correlation with pandas")
    pd.set_option('display.max_rows', None)
    
    # nBestSelect = False
    # if "gt" in df:
    #     corrls, df = getPandasCorrelation(df, corrWith = "gt",
    #                                   invNegative = STAT["invNegative"],verbose = False)
    # else:
    #     nBestSelect = True
        
    
    pd.set_option('display.max_rows', 25)
    # corrMatrix = getPandasCorrelationMatrix(df, printHeatmap = False)
    
    print("\n\n"+"get n features")
    featureList = []

    #Get Features t
    # if(STAT["optimizer"] == "HillClimber"):
    #     featureList = test_N_HillClimberFeatures(df)
    # elif(STAT["optimizer"] == "Random"):
    #     featureList = test_N_RandomFeatures(df)
    # elif(STAT["predefinedFeatures"] == False and nBestSelect):
    #     featureList = getNBestFeatures(corrls, corrMatrix, n=STAT["nBest"], 
    #                                    suboptimalSearch = STAT["suboptimalSearch"], verbose = False)
    # else:
    if(STAT["predefinedFeatures"] == False or STAT["predefinedFeatures"]):
        featureList = STAT["defineYoureOwnFeatureSet"]
    print("use these features: ", featureList)
    print("\n\n"+"create feature stack")
    X = getFeatureStack(df = df, bestFeatures = featureList, comp = STAT["compress"],
                calcAllFeatures = STAT["calcAllFeatures"], verbose= False)
    
    # seperation is needed AFTER all feature get normalized
    Xsingle = np.asarray([X[-1]])
    X = X[:-2]

    #calculate distance matrix and extract corresponding file paths
    distanceMatrix = distance.cdist(Xsingle, X, 'euclidean')[0]
    
    focusFilePath = np.asarray(dfSingle["file_path"])[0]
    filePaths = np.asarray(filePaths[np.argsort(distanceMatrix)[:int(STAT["n_mostSimilar"])]])
    
    
    print("the folowing files are similar to: {}".format(focusFilePath))
    for p in filePaths:
        print(p)
        
    if ("graphviz" in STAT) and (STAT["graphviz"]):
        generateSimpleDotMap(distanceMatrix, filePaths, focusFilePath)
    
    if ("copyFilesToFolder" in STAT) and (STAT["copyFilesToFolder"] is True):
        saveData["featureList"] = featureList
        restructureN_NearestSamples(filePaths, focusFilePath)



def main():
        
    graMode  = "clusterBased"
    
    "Hierarchie is as follows:"
    "First -> optimize features to extract a good subset (if the corresponding value is set)"
    "Second -> Group Samples with a feature-vector-json-file and without comparing to GT-Label"
    "Third  -> get the N most similar features to one specified sample (That sample has to have a corresponding json file"
    "Fourth -> Analyse a given Sample set with their GT-Label"
    
    
    if  "scriptMode" in STAT and STAT["scriptMode"] == "optimizer":
        
        if "optimizer" in STAT and STAT["optimizer"] == "Random":
            
            test_N_RandomFeatures(fileName = STAT["fileName"])
        else:
            
            test_N_HillClimberFeatures(fileName = STAT["fileName"])
            
    elif "scriptMode" in STAT and STAT["scriptMode"] == "clustering":
        
        regroupSamplesByClustering(fileName = STAT["fileName"])
        return
        
    elif "scriptMode" in STAT and STAT["scriptMode"] == "get_N_Closest":
        
        getNSimilarSamples(singleFile = STAT["compareFilePath"], library = STAT["fileName"])
        return 
    
    elif "scriptMode" in STAT and STAT["scriptMode"] == "analyseWithGT":
        
        labelPairs, saveData, df, l_df, X, cluster = dataAnalysisPipeline(fileName = STAT["fileName"])
      
        
        if ("graphviz" in STAT) and (STAT["graphviz"]):
            
            
            
            if "graphvizMode" in STAT :
                graMode = STAT["graphvizMode"]
                
            generateGraphviz(X, labelPairs, graMode)
            
        if ("copyFilesToFolder" in STAT) and (STAT["copyFilesToFolder"] is True):
            print("Start copying files to its labels folder")
            # plotHistogram(labelPairs["gt"], labelPairs["newLabel"], savePath = "../estimateSongs")
            restructureFiles(labelPairs,saveData = saveData, saveFolder = STAT["saveFolder"])
            plotDendrogram(X, '../estimateSongs/', False)
        else:
            # labelPairs["gt"] = sortHistogram(labelPairs["gt"])
            # labelPairs["newLabel"] = sortHistogram(labelPairs["newLabel"])
            plotHistograms(labelPairs["gt"], labelPairs["newLabel"], savePath = "")
    
    
    
if __name__ == "__main__":
 	main()