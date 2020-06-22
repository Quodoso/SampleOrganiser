/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CLIHandler.h
 * Author: lars
 *
 * Created on October 13, 2020, 7:19 AM
 */

#ifndef CLIHANDLER_H
#define CLIHANDLER_H


namespace fs = std::filesystem;

extern const std::set<string> possibleFeaturesSet;

typedef struct APP_PARAMS_{
    std::string libraryPath;   //path of sample library
    int clusters{40}; //amount of clusters to regroup sample library
    int nUl{2047}; // Some abritary upper limit for the amount of samples
    std::string samplePath = ""; // path of ONE specific sample
    std::string compareLibraryPath = "Dirt-Samples"; // path of sample library
    std::string featurePath = fs::path("./json_data/Dirt-Samples2020-09-14.20:53:18.json").string(); // path of precalculated features of sample library
    int nSimilar{25};  // get N most similar samples (from samplePath, search in sample library)
    
    std::string gtPath = fs::path("./groundTruth/DIRT_LABELS.JSON").string();   // path to "Ground truth labels"
    int gtLabel = 1;  // specify GT-Label (Cochlear has 3 labels with descending meaningfulness)

    std::string saveSamplePath = fs::path("./estimateSongs").string();  // path to an existing folder where the regrouped samples will be copied.
    
    bool gtFlagBool = false;  // flag for using ground truth label or not
    bool expByGT = false;  // flag for copying all samples according to their preannotated ground truth label
    bool expJSON = false;    // flag for exporting features as JSON-file
    bool onlyFeat = false;  //  flag for ONLY exporting features as JSON-file
    bool useAllFeat = false; // flag for using all features (not recommended for actual usage)
    bool expSampl = false;  //  flag for copying samples according to clusters/n-similar.
    
    std::vector<std::string> features{"Har-Log_-FACM_10", "MFCC-4", "MFCC-7", "Har-RecChr_-FACM_12","TriChr_Centroid", "ZeroCrossingRate", "MFCC-8"}; // the feature names which will be calculated
    
    void printParams(){      
        std::cout << "Sample Library Path: " <<libraryPath << " \ncluster amount: " << clusters << " maxSamples:"<<nUl<<"\n\n";
        std::cout << "Single Sample Path: " <<samplePath << " \nCompare Sample Library: " << compareLibraryPath << " \nCompare With Precalculated Feature File: " << featurePath << " \nnSimilar: " << nSimilar << "\n\n";
        std::cout << "Ground Truth Path: " << gtPath << " \nUse Ground Truth Label: " << gtLabel << " \nCopy Samples to: " << saveSamplePath << std::endl;
        std::cout << "\nFlags:\n";
        std::cout << "Use Ground Truth?: " << printBool(gtFlagBool) << "\nExport Samples by Ground Truth?: " << printBool(expByGT);
        std::cout << "\nExport Features as JSON?: " << printBool(expJSON) << "\nOnly Export JSON file of Features?: " << printBool(onlyFeat);
        std::cout << "\nCopy Samples?: " << printBool(expSampl) << "\nUse all Features?: " << printBool(useAllFeat) << std::endl;
        std::cout << "\n\nUse the following features:" << std::endl;
        //print the selecting features;
        int i = 0;
        for(size_t j  = 0; j < features.size(); ++j){
            ++i;
            std::cout << features[j] << " \t ";
            if( i% 7 == 0){
                std::cout << std::endl;
            }
        }
    }
}APP_PARAMS;

extern APP_PARAMS FLOW_PARMS;

int handleCommdLineInput(int argc, char** argv, APP_PARAMS & params);

#endif /* CLIHANDLER_H */
