# SampleOrganiser
SampleOrganiser aims to group and find similar Samples in a given Sample Library. The Library which is used is the [Super-Dirt-Samples](https://github.com/musikinformatik/SuperDirt) Library.

It originated from a research project and is sadly slightly inconvenient to use.

## Overview
This project is separated into two parts: 

1. Audio Feature Extraction, which is implemented in C++, extract right know ~190 single Features and save the results with their file path in a JSON-file.
Most of this ridiculous amount of features originated from the same idea. In General, the relevant Features out of this project can be divided into 3 groups:
- Temporal Features
- Spectral Features (without a time component)
- Spectral/Temporal Features (i.e. spectrogram)
Especially the spectral features are calculated with six different scaled and/or filtered frequency bands.


2. Data Analysis (optional), which is implemented in python with the usual Data-Tools (pandas, sklearn, etc.).

## How can you use anything here?
Group all Dirt-Samples according to the "best fitting features" into 40 groups

    ./sampleorganiser --lp=/home/lars/Documents/Samples/Dirt-Samples --oF

For faster clustering save all features

    ./sampleorganiser --lp=/home/lars/Documents/Samples/Dirt-Samples --gtF --gt=/home/lars/Documents/workspace/c++/quickSample/SampleOrganiser/groundTruth/DIRT_LABELS.json --gtL=1 --eF --allF --onlyF

Now it takes significantly less time to cluster:

    ./sampleorganiser --fp=/home/lars/Documents/workspace/Sampleorganiser/jsonData/Dirt-Samples2021-04-12-10,14,49.json --oF

Get 25 most similar samples to one reference sample and group/copy them accordingly

    ./sampleorganiser --fp=/home/lars/Documents/workspace/Sampleorganiser/jsonData/Dirt-Samples2021-04-12-10,14,49.json --sp=/home/lars/Documents/Samples/Dirt-Samples/kurt/002_kurt03.wav --oF

Generating a landmap is not implemented in c++ yet. You have to use /data_scripts-python/audioFeatureHandler.py and control its behavior with several parameters/options within /data_scripts-python/ControlFlowParams.py

## Requierements
For c++ (c++17) are the following libraries necessary to compile (gcc 8) the program:
- [Essentia](https://essentia.upf.edu/) is a Music Information Retrieval Library under Affero GPLv3 Licence.
- [nlohmann](https://github.com/nlohmann/json) is a C++ Library for convenient usage of and with json files (MIT Licence).
- [GSL-Statistic](https://www.gnu.org/software/gsl/) is a numerical Library for C/C++ under GNU General Public License
- [experimental/filesystem](https://en.cppreference.com/w/cpp/experimental/fs/path) That's already in a more modern standard. Don't forget the corresponding linker

For python (3.8.5) there are obviously a couple of Requirements involved:
- [pandas]()
- [numpy]()
- [matplotlib]()
- [scipy]()
- [sklearn]()

-> If you want to keep it simple, I recommend the use of [Anaconda](https://anaconda.org/anaconda/python) where all the important libraries are default

- [pygraphviz](https://pygraphviz.github.io/) is the full-blown python wrapper for [GraphViz](https://graphviz.org/) a quiet handy visualization tool
- [graphviz](https://graphviz.readthedocs.io/en/stable/) a very light-weighted version. I could not rewrite the part where it is used right now. This will be tackled in the future.

And a general tool to install is:
- [GraphViz](https://graphviz.org/), a powerfull application for visualition.

(optional)
If you want to generate Audio-Labels for whatever purpose you can use [Cochlear.ai](https://github.com/finejuly/dcase2018_task2_cochlearai) which will classify Samples into predefined 41 groups. The pre-trained NN can be downloaded [here](https://storage.googleapis.com/cochlearai_public/dcase2018_task2_cochlearai.zip).
Since the Super-Dirt-Samples are far from labeled data, it was important to have something to compare and evaluate.

## IMPORTANT
If you want to use the precalculated feature vectors, it is necessary to run the python script 'renameSamplePaths.py'. It will rename all sample paths in all files to the SUPER-DIRT-SAMPLE libary of your machine. Without that update, you won't be able to regroup samples (copy and paste them).
Just run:

    python ./renameSamplePaths.py "/YOUR/PATH/TO/Dirt-Samples/"


For example:
    
    python ./renameSamplePaths.py "/home/Documents/blalalala/Dirt-Samples/"
    
from any terminal.
Keep in mind, that it needs the path to the folder of the "/Dirt-Samples/". The next depth are already the 
- './clubkick/'
- './blip/'
- './amencutup/'

to name a few.


## Step-by-step instructions

1.  Install requierements for c++ and python as mentioned in "Requierements"
2.  Download [Super-Dirt-Samples](https://github.com/musikinformatik/SuperDirt) to any folder you want.
3.  Open Terminal and navigate to parent folder of this project
4.  run "make"
5.  cp ./dist/Release/GNU-Linux/sampleorganiser ./sampleorganiser
6.  run "python ./renameSamplePaths.py "YOUR/SUPER_DIRT_SAMPLE_PATH/Dirt-Samples/" with your absolute path to the Dirt-Samples (The last "/" have to be included)

Now there are 2 possible ways to continue: 
1.  work with
2.  Work with the generated features with python

### Generate features and cluster/group files with C++ 
use

    ./sampleorganiser -h to display all grouping and calculation options

### Work with the generated features with python
You either have calculated a new feature set or are using the precalculated one ("./json_data/Dirt-Samples2020-09-14.20:53:18.json"). To run with the default settings:
    cd ./data_scripts-python
    python audioFeatureHandler.py
This will load the calculated feature set, take the 7 features, cluster the feature set by the 7 features, calculate the correlation between the compare annotation and the new groups (which are the clusters) and display a histrogram of the group distribution of the compare annotation and the new groups.
To change the behaviour you have to edit the file "./data_scripts-python/ControlFlowParams.py". There is again, no command line control yet. There are too many option as well to name everything here, but each option is descripted in "./data_scripts-python/ControlFlowParams.py". But to name a few different behaviours:

1. "fileName" : file name of sample library feature set in "./json-data/". Default is "Dirt-Samples2020-09-14.20:53:18.json"
2. "scriptMode" : ("clustering", "get_N_Closest", "analyseWithGT", "optimizer"). The four modi of this application. "analyseWithGT", "optimizer" need compare annotations within the precalculated feature set.
3. "graphviz": True/False to generate a landmap of a given feature set (the default landmap is a minimal span tree saved to "./landmaps/Minimal/"
4. "copyFilesToFolder" : True/False to regroup all samples by their new cluster group, or copy the N most similar samples of a given sample to a folder.


As an example make the following edit of "./data_scripts-python/ControlFlowParams.py":
    "scriptMode" : "clustering",
    "graphviz" : True,
    "copyFilesToFolder" : True,
Now
    cd ./data_scripts-python
    python audioFeatureHandler.py
You will now generate a minimal span tree landmap of the default feature set of the dirt-samples and save the regroup all samples into their assigned cluster number in "./estimateSongs/withoutGT/average$CURRENT_DATE$/.

An example to get the 25 most similar samples:
    "scriptMode" : "get_N_Closest",
    "graphviz" : False,
    "copyFilesToFolder" : True,
Now
    cd ./data_scripts-python
    python audioFeatureHandler.py
You will now get the 25 most similar samples copied to "./estimateSongs/Nearest/$SampleName$CURRENT_DATE$/.


## What are some "Features" of these programs?
Right now, the actual use is kind of limited:
- regroup the Super-Dirt-Samples however you want.
- have some Data Exploration
- generate an absolute messy land map where a minimal span tree is drawn according to the Feature Distance. There are Several Options to chose (Samples grouped by Clusters, each Cluster one file, and one huge map)
- Try different options which may lead to different/better results.
- get Histrogram of the new assigned labels, compare it with ground truth, plot Dendrogramm
- calculate features of any sample (with c++) and get n most similar samples from an existing precalculated sample library
- cluster and regroup any Sample library by a set of featurs
- get n most similar samples according to a given sample


Disclaimer: 
This project is still under construction and will be continued in the near future.
All necessary generated research data is contained
