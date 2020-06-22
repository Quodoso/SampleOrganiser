#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Sep 10 00:03:16 2020

@author: lars
"""

import os
import argparse

def getListOfFiles(papaDir, fileConstrains = []):

    listOfFiles = os.listdir(os.path.abspath(papaDir))
    allFiles = list()
    # Iterate over all the entries
    for entry in listOfFiles:
        
        fullPath = os.path.join(papaDir, entry)
        # If entry is a directory then get the list of files in this directory 
        if os.path.isdir(fullPath):
            allFiles = allFiles + getListOfFiles(fullPath,fileConstrains)
        else:
            if any([ constrain in fullPath for constrain in fileConstrains]):
                allFiles.append(fullPath)
                
    return allFiles


def main(startPath, oldString, replaceWith):
    
    
    fileConstrains = [".json", "ControlFlowParams.py"]
    
    allFiles = getListOfFiles(papaDir, fileConstrains)
    print("{} files collected".format(len(allFiles)))
    for f in allFiles:
        print(f)
    i = 1
    for filePath in allFiles:
        # Read in the file
        with open(filePath, 'r') as file :
          filedata = file.read()
        
        # Replace the target string
        filedata = filedata.replace(oldString, replaceWith)
        
        # Write the file out again
        with open(filePath, 'w') as file:
          file.write(filedata)
        print("{}/{} files modified".format(i,len(allFiles)))
        i += 1
        

if __name__ == "__main__":
    
    parser = argparse.ArgumentParser(description='Replace Super Dirt Sample Library Path in all data files')
    parser.add_argument('replaceWith',type=str,
                    help='The string which represent the directory of the Super-Dirt-Samples')
    parser.add_argument('--oldText', type=str,
                    help='The old file path which should be replaced',
                    default="/home/lars/Documents/Samples/Dirt-Samples/")
    parser.add_argument('--startFolder', type=str,
                    help='Folder path to start iterating through all sub folders and collecting files',
                    default="PATH_OF_PYTHON_FILE")
    args = parser.parse_args()
    
    papaDir = ""
    if args.startFolder == "PATH_OF_PYTHON_FILE":
        papaDir = os.path.dirname(os.path.abspath(__file__))
    else:
        papaDir = args.startFolder
    papaDir = os.path.abspath(papaDir)
    
    if args.replaceWith != None:
        print("parendFolder: {}, oldText:{}, replaceBy:{}".format(papaDir, args.oldText, args.replaceWith))
        main(papaDir, args.oldText, args.replaceWith)
    