#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Sep  9 00:42:16 2020

@author: lars
"""

import os
import json
import argparse
import sys

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

def doesFileFullfillConstraints(fullPath, constraints = []):
    """
    @param constrains := list auf conditions, which a file shoul have in its name or path
    """
    for const in constraints:
        if const in fullPath:
            return True
        
    

def getListOfFiles(dirName, constraints = []):
    """
    https://thispointer.com/python-how-to-get-list-of-files-in-directory-and-sub-directories/
    """

    listOfFile = os.listdir(dirName)
    allFiles = list()
    # Iterate over all the entries
    for entry in listOfFile:
        # Create full path
        fullPath = os.path.join(dirName, entry)
        # If entry is a directory then get the list of files in this directory 
        if os.path.isdir(fullPath):
            allFiles = allFiles + getListOfFiles(fullPath)
        else:
            if doesFileFullfillConstraints(fullPath,constraints):
                allFiles.append(fullPath)
                
    return allFiles        



def recursiveJSONgrazing(d, findByValue = "", replaceWith = "", keyContains  = ["path", "Path"]):
    if(isinstance(d, dict)):
        for (k,v) in d:
            if isinstance(v, list):
                if len(v) >0 and isinstance(v, str):
                    for i in range(len(v)):
                        v[i].replace(findByValue, replaceWith)
            if isinstance(v, dict):
                return recursiveJSONgrazing(v)
                                              


def replaceAllPathsInJSON(jsonList):
    for j in jsonList:
        jf = loadJsonFile(j)
        
        
        for (k, v) in jf.items():
           print("Key: " + k)
           print("Value: " + str(v))
           if isinstance(v, list) or isinstance(v, dict):
               "walla"
               

def main():
    
    parser = argparse.ArgumentParser(description='Updates all .json/Data files which contain a non valid path to the SUPER-DIRT-Library')
    parser.add_argument('--folderPath',type=str,
                    help='The head folder of the Super-Dirt-Library. There should only be a list of 217 folders.')
    parser.add_argument('--whatToErase',type=str,
                    help='If you know what to erase, you can do that as well')
    args = parser.parse_args()
    if os.path.exists(sys.argv[0]):
        print(args.folderPath)
        
        contraints = [".json"]
        contraints.append(args.whatToErase)
        
        fileList = getListOfFiles(args.folderPath,contraints)
        
        replaceAllPathsInJSON(fileList)
        





if __name__ == "__main__":
 	main()




# replaceStringSUPERDIRT = "Hanna/Montana"

# currDir = os.path.abspath(os.getcwd())


# appFlowCPP = "OrganizerFlowParams.json"

# appFlowPython = "ControlFlowParams.py"

# mostData = "SampleOrganizer/json_data"

# #find all json files within SampleOrganizer