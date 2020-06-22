/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: lars
 *
 * Created on May 21, 2020, 10:02 AM
 */
#include "./stdheader.h"

using namespace std;
//TODO: I can input a sample and not library ! Fix this
//TODO: get somehow negated features (or which features to negate)
int main(int argc, char** argv) {
    bool verbose = false;
        // sample library or single file
            //__sample library -> Load Sample Library
                // load gt (maybe) -> Load GT Library and assign to SL
                // select features
                // Calculate Features
                    //with all flags
                // export Features as json file
                // normalize features
                // Cluster in C++ (maybe)
        
            //__single file -> Load Single File
                // load sample library OR precalculated features (or subset of them)!
                // select features
                // calculate features
                // normalize features
                // calculate distance matrix
                // get N samples with smallest distance
                
        //<---finish--->// (1)
            // Copy files to new folder
            // give summary as output
    //<----finish---->// (2)
    
    auto start = std::chrono::high_resolution_clock::now(); //measure runtime
    // declaring here, so we can print some statistics about the analysis.
    std::map<int, int> distinct;  // 
    std::vector<AudioFile> allFiles;
    //loads command line arguments and initialize essentia
    if( auto message = handleCommdLineInput(argc,argv, FLOW_PARMS) == 0 && initEssentia()){
        FLOW_PARMS.printParams();
        
        //inputted features from user (or default, or explicitly force to calculate all features)
        std::vector<string> selFeatures = FLOW_PARMS.features;
//        std::cout << "The following features were given:" << std::endl;
//        printVector<string>(selFeatures);
        std::string appPath = cleanPathFromRelative(argv[0]); // path of application
        std::cout << "\nargv[0]:"<< argv[0] <<"\n\napp path: " << appPath << std::endl;
        if(FLOW_PARMS.libraryPath == ""){
            std::cout << "analyze " << FLOW_PARMS.samplePath << "   with " << FLOW_PARMS.compareLibraryPath << std::endl;
        }else{
             std::cout << "analyze " << FLOW_PARMS.libraryPath << std::endl; 
        }
       
        if(!FLOW_PARMS.libraryPath.empty() || ( !FLOW_PARMS.featurePath.empty() && FLOW_PARMS.samplePath.empty())){
            // get all audio file paths
            if(!FLOW_PARMS.libraryPath.empty()){
                std::vector<string> audioPaths = getAllWavPaths(FLOW_PARMS.libraryPath);

                // only analyze a subset. For debug purposes
                if(FLOW_PARMS.nUl <= audioPaths.size()){
                    audioPaths.resize(FLOW_PARMS.nUl);
                }
                // load all audio files with equal loudness (~RMS loudness)
                std::cout << "start reading audio files" << std::endl;
                allFiles.resize(audioPaths.size());
                 for(unsigned int i = 0; i < audioPaths.size(); ++i){
                    if(verbose){ std::cout << "Load Audio File: "<< audioPaths[i] << std::endl;}
                    allFiles[i] = AudioFile(audioPaths[i]);
                    if( (i+1) % 10 == 0){
                        std::cout << std::to_string(i+1) << "|" << std::to_string(audioPaths.size()) << " audio files are loaded" << std::endl;
                    }
                }
                std::cout << allFiles.size() << " audio files read" << std::endl;

            }else if(!FLOW_PARMS.featurePath.empty()){
                std::cout << "Read audio feauters from feature file " << std::endl;
                allFiles = readAudioFeaturesFromJSON(FLOW_PARMS.featurePath, FLOW_PARMS.features);
                // only analyze a subset. For debug purposes
                if(FLOW_PARMS.nUl <= allFiles.size()){
                    allFiles.resize(FLOW_PARMS.nUl);
                }
                std::cout << "Feature file read and populated" << std::endl;
            }
            
            //assign Ground Truth Label to Audio File
            //in the case of APP_PARMS.withGT == False, each audio file gets a dummy Ground Truth.
            std::cout << "Read GT_labels" << std::endl;
            std::vector<SampleGT> gts = readAndGetGroundTruth(FLOW_PARMS.gtPath, FLOW_PARMS.gtFlagBool, verbose);
            std::cout << gts.size() << " GT-Labels read" << std::endl;
            std::cout << "start assining GT" << std::endl;
            assignGroundTruthToFiles(gts,allFiles, FLOW_PARMS.gtFlagBool, FLOW_PARMS.gtLabel, verbose);
            std::cout << "GT assigned" << std::endl;
            
            
            //if export by ground truth is active, it will only copy the files according to their ground truth
            if(FLOW_PARMS.expByGT){
                std::cout << "start copying all samples to their preannotated 'ground truth'-label" << std::endl;
                //reorganize samples
                std::cout << "All files are copied according to GT-Label_" << FLOW_PARMS.gtLabel << " to path:" << FLOW_PARMS.saveSamplePath << std::endl;
                goto end;

            }
            
            if( FLOW_PARMS.featurePath.empty()){
                //calculate all Features from each file.
                std::cout << "start calculating selected features features" << std::endl;
                populateFeaturesToAllFiles(allFiles, selFeatures, verbose);
                std::cout << "feature calculation over" << std::endl;
            }
            if(FLOW_PARMS.expJSON){
                std::cout << "Start exporting features to JSON-File" << std::endl;
                wirteAudioFeatures(allFiles, findInFileSystemBack(appPath, "json_data", 4), getNameInPath(FLOW_PARMS.libraryPath, 0));
                std::cout << "Feature-JSON-File written" << std::endl;
                //when "only export feature file" is active, no cluster will be build
                if(FLOW_PARMS.onlyFeat){
                    goto summary;
                }
            }

            std::cout << "Start normalizing features" << std::endl;
            normalizeFeatures(allFiles);
            std::cout << "Features normalized" << std::endl;
            
            std::cout << "start calculating distance matrix" << std::endl;
            std::vector<double> distMatVec = calcDistanceMat(allFiles, selFeatures);
            std::cout << "Start clustering samples by features" << std::endl;
            std::vector<int> clLabels = clusterAudioFiles(distMatVec, allFiles.size(), FLOW_PARMS.clusters);
            //count distinct labels and their occurrence
            for(int & cl : clLabels){
                if(distinct[cl] >= 0){
                    ++distinct[cl];
                }else{
                    distinct[cl] = 0;
                }
            }
            addClusterToAudioFiles(allFiles, clLabels);
            std::cout << "finished clustering"<<std::endl;
//            printClusterWithFiles(allFiles);
            std::cout << "Samples are clustered" << std::endl;
            
            //export samples to new directory, grouped by their cluster
            if(FLOW_PARMS.expSampl){
                std::cout << "Start copying all samples to the designated path" << std::endl;
                OrgaStruct modeData;
                modeData.set("Cluster", "Cluster");
                reorganizeAudioFiles(allFiles, fs::path(FLOW_PARMS.libraryPath).string(), modeData, fs::path(FLOW_PARMS.saveSamplePath).string());
                std::cout << "All samples are copied to: " << std::endl;
            }
            
        }else{
            std::cout << "Sample LIBRARY Path is empty. Select 'get n most similar samples to a SINGLE file'" << std::endl;
            std::vector<AudioFile> subFiles; // The vector of subFiles for later access;
            if(!FLOW_PARMS.samplePath.empty()){
                std::cout << "Start reading SINGLE audio file " << FLOW_PARMS.samplePath << std::endl;
                    //TODO:Read single audio file and calculate features
                std::vector<string> singleFilePath = getAllWavPaths(FLOW_PARMS.samplePath);
                std::vector<AudioFile> singleAudio(singleFilePath.size());
                for(auto & f : singleFilePath){
                    std::cout << f << std::endl;
                    singleAudio[0] = AudioFile(f);   
                }
                populateFeaturesToAllFiles(singleAudio, selFeatures, true);
                std::cout << "SINGLE audio file read, loaded and features calculated" << std::endl;
                
                //Decide how the compare features will be loaded (precalcualted features, or calculate features of an entire sample library)
                if( !FLOW_PARMS.featurePath.empty() && FLOW_PARMS.compareLibraryPath.empty() ){
                    std::cout << "Use precalculated features from file:" << FLOW_PARMS.featurePath << std::endl;
                    allFiles = readAudioFeaturesFromJSON(FLOW_PARMS.featurePath, FLOW_PARMS.features);
                    allFiles.insert(allFiles.begin(), singleAudio[0]);
                }else if( !FLOW_PARMS.compareLibraryPath.empty() && FLOW_PARMS.featurePath.empty() ){
                                        
                    std::vector<string> audioPaths = getAllWavPaths(FLOW_PARMS.compareLibraryPath);
                    
                    if(FLOW_PARMS.nUl >= 0) {audioPaths.resize(FLOW_PARMS.nUl);}
                    
                    //the "main" audio sample, which will be compared is at index 0
                    std::cout << "start reading audio files" << std::endl;
                    allFiles.resize(audioPaths.size()+1);
                    allFiles[0] = AudioFile(singleFilePath[0]);
                    
                     for(unsigned int i = 1; i <= audioPaths.size(); ++i){
                        std::cout << "Load Audio File: "<< audioPaths[i-1] << std::endl;
                        allFiles[i] = AudioFile(audioPaths[i-1]);
                    }
                    std::cout << allFiles.size() << " audio files read" << std::endl;
                    
                    //calculate all Features from each file.
                    std::cout << "start calculating features" << std::endl;
                    populateFeaturesToAllFiles(allFiles, selFeatures, false);
                    std::cout << "feature calculation over" << std::endl;

                    std::cout << "Start exporting features to JSON-File" << std::endl;
                        //TODO:export function
                    std::cout << "Feature-JSON-File written" << std::endl;
                }  

                std::cout << "Start normalizing features" << std::endl;
                normalizeFeatures(allFiles);
                std::cout << "Features normalized" << std::endl;
                
                std::cout << "start calculating distance matrix" << std::endl;
                std::vector<double> distVec = calcDistToOneEl(allFiles, 0, selFeatures);
                printVector(distVec);
                std::cout << "Distance Matrix calculated" << std::endl;
                
                std::cout << "Start selecting n="<<FLOW_PARMS.nSimilar << " samples with smallest distance" << std::endl;
                std::map<double, int> nSim = get_N_Minimum(distVec, FLOW_PARMS.nSimilar, true);
                //add the audio file which is compared to the "output" vector (there is no distance of -1 in the real world)
                nSim[-1.0] = 0;
                subFiles = getSubAudioFiles(allFiles, nSim);
                std::cout << FLOW_PARMS.nSimilar << " Samples selected" << std::endl;
            }else{
                std::cout << "SINGLE file Path is empty. I guess you do not want to use this application." << std::endl;
                //TODO:LOAD FEATURE FILE
            }
           
            if(FLOW_PARMS.expSampl){
                std::cout << "Start copying all "<<FLOW_PARMS.nSimilar <<"+1 samples to the designated path" << std::endl;
                OrgaStruct modeData;
                modeData.set(fs::path(FLOW_PARMS.samplePath).string(), "OneFile");
                reorganizeAudioFiles(subFiles, fs::path(FLOW_PARMS.compareLibraryPath).string(), modeData, fs::path(FLOW_PARMS.saveSamplePath).string());
                std::cout << "All samples are copied to: " << std::endl;
            }
        }
        // print summary
        std::cout << "\n\nStart printing summary" << std::endl;
        std::cout << "_____Used the following settings____" << std::endl;
        FLOW_PARMS.printParams();
        if(FLOW_PARMS.expSampl){
            std::map<int,int>::iterator dist_it = distinct.begin();
            int i = 0;
            while( dist_it != distinct.end()){
                if( i == 0){ std::cout << "\n(L)Label:(A)Amount" << std::endl;}
                std::cout <<  "(L)" << dist_it->first << ":(A)"<< dist_it->second;
                i = (i % 7) +1;
                if( i % 7 == 0 ){
                    std::cout << std::endl;
                }else{
                    std::cout << "\t";
                }
                ++dist_it;
                if( dist_it == distinct.end()){
                    std::cout << std::endl;
                }
            }
        }
        summary:
        std::cout << "\nTotal files analyzed: " << allFiles.size() << "  Total samples (Datapoints): " << getTotalSampleLength(allFiles) << " (Samples)"
                << "  Total time: " << convertSecondsToTime(static_cast<double>(getTotalSampleLength(allFiles)) / SAMPLERATE) << " (seconds)" <<std::endl;
            //TODO:print summary function
        std::cout << "Summary printed" << std::endl;
        std::cout << "\n\n___________" << std::endl;
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start); 
        std::cout << "Task finished successfully after " << duration.count() << " seconds." << std::endl;
        
    }
    end:
    shutdownEssentia();
    return 0;
    
}

