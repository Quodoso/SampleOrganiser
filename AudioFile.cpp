/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AudioFile.cpp
 * Author: lars
 * 
 * Created on May 21, 2020, 9:15 AM
 */


#include "stdheader.h"


namespace fs = std::filesystem;

std::string AudioFile::getFileLocation(){
    return this->fileLocation;
}

std::vector<std::vector<float>> AudioFile::getSpectrogram(){
    if( !this->spectrogram.empty() && !this->spectrogram[0].empty()){}
    else{
        this->spectrogram = calcSpectrogram(this->monoData);
    }     
    return this->spectrogram;
    
}

std::string AudioFile::fileName(bool replaceSpace){
    
    return getFileName(fileLocation, replaceSpace, true);
}

std::string AudioFile::getParentDirName(){
    return getNameInPath(fileLocation, 1);
}

vector<float> AudioFile::getMonoData(){
    return this->monoData;
}

void AudioFile::assignGT(SampleGT gt){
    this->groundTruth = gt;
}

AudioFile::AudioFile() {
}

AudioFile::AudioFile(const AudioFile& orig) {
    this->groundTruth = orig.groundTruth;
    this->features = orig.features;
    this->fileLocation = orig.fileLocation;
    this->code = orig.code;
}

AudioFile::AudioFile(string fileLocation, bool onlyPath) {
    this->fileLocation = fileLocation;
    if( !onlyPath){
        getMonoAudioEQLoud(fileLocation, this->monoData);  
    }
}

AudioFile::~AudioFile() {
}

SampleGT AudioFile::getGroundTruth(){
    return this->groundTruth;
}

std::string AudioFile::getGroundTruthLabel(int labelSelector){
    switch (labelSelector){
        case 1: return this->groundTruth.label01;
        case 2: return this->groundTruth.label02;
        case 3: return this->groundTruth.label03;
        default: return this->groundTruth.label01;
    }
}
std::string AudioFile::getGroundTruthLabel(std::string labelSelector){
     return this->getGroundTruthLabel(std::abs(std::stoi(labelSelector)));
}


std::unordered_map<string, AudioFeature> & AudioFile::getAudioFeatures(){
    return this->features;
}

void AudioFile::addFeature(std::string name, AudioFeature feature){
    if(this->features.find(name) == this->features.end()){
        this->features[name] = feature;
    }
}
/**
 * prints all features of one AudioFile
 */
void AudioFile::printAudioFeatures(bool withNotes){
    std::setprecision(6);
    int i = 0;
    
    std::unordered_map<string, AudioFeature>::iterator itF = this->features.begin();
    while( itF != this->features.end()){
        std::cout << i <<" name: " << itF->first << std::endl;
        std::cout << i << "    values: ";
            for(double & val : itF->second.values){
                std::cout<< val << " ";
            }
            for(std::string & sval : itF->second.strValues){
                std::cout << sval << " ";
            }
        
        if(withNotes){
            std::cout << "\n    note: " << itF->second.notes;

        }
        std::cout << std::endl;
    ++i;
    itF++;
    }
}

void printSpecificFeature(std::vector<AudioFile> & files, string feat){
    std::cout <<  "print feature '"<< feat <<"'\n" << std::endl;
    for(AudioFile & f : files){
        std::cout << f.getAudioFeatures()[feat].values[0] << " ";
    }
    std::cout << std::endl;
}

void printAllAudioFeatures(std::vector<AudioFile> & files){
    for(AudioFile & f : files){
        f.printAudioFeatures(false);
    }
}

void AudioFile::calculateMonoSignalFeatures( std::unordered_map<std::string, SubFeatureSelector> & whiteL, bool & verbose){
    
    /****
    ***-----Signal Features
    ****/

    if( !whiteL["time"].rhytm2013.empty) {
        if(verbose) std::cout << "RythmExtractor2013" << std::endl;
        calcRhythmExtractor2013(this->monoData, this->features, whiteL["time"].rhytm2013);
    }
    
    if( whiteL["time"].TCToTotal ){
        if(verbose) std::cout << "calcTCToTotal" << std::endl;
        calcTCToTotal(this->monoData, this->features);
    }
    if( whiteL["time"].effectiveDuration || whiteL["time"].effectiveToSdtDuration){
        double effectiveDur;
        if(verbose) std::cout << "calcEffectiveDuration" << std::endl;
        calcEffectiveDuration(this->monoData, this->features);  
        effectiveDur = this->features.at("EffectiveDuration").values[0];
        
        if( whiteL["time"].effectiveDuration  ){
            this->features.erase("EffectiveDuration");
        }
        if( whiteL["time"].effectiveToSdtDuration ){
            if(verbose) std::cout << "DurationToEffective" << std::endl;
            this->features["EffectiveToStdDuration"] = AudioFeature("EffectiveToStdDuration", effectiveDur/(this->monoData.size()*1.0)/(SAMPLERATE*1.0), "Effective Durationt to total sample length");
        }
    }
    if( whiteL["time"].zeroCrossingRate ){
        if(verbose) std::cout << "calcZeroCrossingRate" << std::endl;
        calcZeroCrossingRate(this->monoData, this->features);
    }
    
    if( !whiteL["time"].tonalExtractor.empty){
        if(verbose) std::cout << "calcTonalExtractor" << std::endl;
        calcTonalExtractor(this->monoData, this->features, whiteL["time"].tonalExtractor);
    }
    if( whiteL["time"].sampleLength ){
        if(verbose) std::cout << "SampleLength" << std::endl;
        this->features["sample_length"] = AudioFeature("sample_length", (this->monoData.size()*1.0)/(SAMPLERATE*1.0), "Sample Length with samplerate:"+std::to_string(SAMPLERATE));
    }
    if( whiteL["time"].autoCorrelation_sig ){
        if(verbose) std::cout << "AutoCorrelation on Signal" << std::endl;
        vector<double> mono(this->monoData.begin(), this->monoData.end());
        this->features["AutoCorrelation_sig"] = AudioFeature("AutoCorrelation_sig", getAutoCorrelation(mono), "AutoCorrelation with GSL");
    }

    
}
void AudioFile::calculateSpectrumFeatures(std::unordered_map<std::string, SubFeatureSelector> & whiteL, 
        std::vector<std::vector<float>>& spectogram, bool verbose){
    
    /****
     ***-----Spectrum Features
     ****/
    /* "Bark27" features rooted from a reading error. The essentia function for
     * Bark bands calls FrequencyBands. The default value for frequency bands are the bark bands...
     * (They could at least write a note for that.)
     */
    std::map<std::string, std::vector<float>> spectrumSpaces;
    
    std::vector<float> linSpec = calcMeanSpectrum(spectogram);
    if( whiteL.find("Lin_") != whiteL.end() ){
        spectrumSpaces["Lin_"] = linSpec;
    }
    if( whiteL.find("TriChr_") != whiteL.end() ){
        std::vector<float> triChromSpectrum = calcChromaticSpectrum(linSpec, "triangular");
        spectrumSpaces["TriChr_"] = triChromSpectrum;
    }
    if( whiteL.find("Bark28_") != whiteL.end() ){
        std::vector<float> bark28Spectrum = calcBarkBands(linSpec);
        spectrumSpaces["Bark28_"] = bark28Spectrum;
    }
    if( whiteL.find("RecChr_") != whiteL.end() ){
        std::vector<float> recChromSpectrum = calcChromaticSpectrum(linSpec, "rectangular");
        spectrumSpaces["RecChr_"] = recChromSpectrum;
    }
    if( whiteL.find("Log_") != whiteL.end() ){
        std::vector<float> logSpectrum= calcLogSpectrum(linSpec);
        spectrumSpaces["Log_"] = logSpectrum;
    }
    if( whiteL.find("Mel96_") != whiteL.end() ){
        std::vector<float> melSpectrum= calcMelBandsSingle(linSpec);
        spectrumSpaces["Mel"+std::to_string(MELBAND)+"_"] = melSpectrum;
    }
    /*The following features have calculation errors within:
     *   xxFlatness, xxRollOff, xxStrongPeak, Harxxx GLCM_6 
     * method: for these features transform the spectrum into a dbFS spectrum
     **/
    
    std::map<std::string, std::vector<float> >::iterator it = spectrumSpaces.begin();
    while(it != spectrumSpaces.end()){
        
        const std::string & specName = it->first;

        if( whiteL[it->first].spectrum.rolloff){
            if(verbose) std::cout << "calcRollOff" << std::endl;
            calcRollOff(it->second, this->features, 0.85, specName);
        }
        if (whiteL[it->first].spectrum.spectralComplexity){
            if(verbose) std::cout << "calcSpectralComplexity" << std::endl;
            calcSpectralComplexity(it->second, this->features, 0.005, specName);
        }
        if( whiteL[it->first].spectrum.strongPeak){
            if(verbose) std::cout << "calcStrongPeak" << std::endl;
            calcStrongPeak(it->second, this->features, specName);
        }
        if( whiteL[it->first].spectrum.essEntropy){
            if(verbose) std::cout << "calcEssEntropy" << std::endl;
            calcEssEntropy(it->second, this->features, specName);
        }
        if( whiteL[it->first].spectrum.specComplex.kurtosis ||  whiteL[it->first].spectrum.specComplex.skewness  
                || whiteL[it->first].spectrum.specComplex.spread){
            
            if(verbose) std::cout << "calcDistributionShape" << std::endl;
            calcDistributionShape(it->second, this->features, whiteL[it->first].spectrum.specComplex, "sample", specName);
        }
        if( whiteL[it->first].spectrum.flatness){
            if(verbose) std::cout << "calcFlatness" << std::endl;
            calcFlatness(it->second, this->features, specName);
        }
        if( whiteL[it->first].spectrum.centroid){
            if(verbose) std::cout << "SpectralCentroid" << std::endl;
            calcSpectralCentroid(it->second, this->features, SAMPLERATE/2.0,  specName);
        }
        if( whiteL[it->first].spectrum.spectralFlux){
            if(verbose) std::cout << "calcFlux"+ specName << std::endl;
            calcSpectralFlux(it->second, this->features, false, "L2",  specName);
        }
        if( whiteL[it->first].spectrum.autocorrelation){
            if(verbose) std::cout << "AutoCorrelation on "+ specName << std::endl;
            std::vector<double> triChroDouble(it->second.begin(), it->second.end());
            this->features[specName+"AutoCorrelation"] = AudioFeature(specName+"AutoCorrelation", getAutoCorrelation(triChroDouble), specName);
        }
        
        //MFCC
        if( it->first == "Mel96_"){
            std::vector<int> indices  = whiteL[it->first].spectrum.mfcc;
            if(indices.size()  > 0){
                std::vector<float> mfcc = calcMFCC(linSpec, 13000, linSpec.size(),  40,  13);
                if(verbose) std::cout << "MFCC" << std::endl;
                std::string defaultNote = "MFCC";
//                int i = 1;
                for( const int & mfcc_i : whiteL[it->first].spectrum.mfcc){
                    this->features["MFCC-"+std::to_string(mfcc_i)] = AudioFeature("MFCC-"+std::to_string(mfcc_i), mfcc[mfcc_i], defaultNote);
                }
            }
        }
        it++;
    }
            
   
}
/**
 * calculates all spectrogram related features
 * @param whiteL := white list of a subset of features (key := spectrogram, value := boolean list if features)
 * @param verbose
 * @return linear spectrogram to use it afterwards.
 */
std::vector<std::vector<float>> AudioFile::calculateSpectrogramFeatures(std::unordered_map<std::string, SubFeatureSelector> & whiteL,
        bool verbose){
    
    std::map<std::string, std::vector<std::vector<float>> > spectrogramSpaces;
    
    std::vector<std::vector<float>> linSpectrogram = calcSpectrogram(this->monoData);
    if(whiteL.find("TriChr_") != whiteL.end() && !whiteL["TriChr_"].spectrogram.empty){
        std::vector<std::vector<float>> triChromSpectrogram  = calcChromaticSpectrogram(linSpectrogram, "triangular");
        spectrogramSpaces["TriChr_"] = triChromSpectrogram;

    }
    if(whiteL.find("Bark28_") != whiteL.end() && !whiteL["Bark28_"].spectrogram.empty){
        std::vector<std::vector<float>> bark28Spectrogram  = calcBarkBandSpectrogram(linSpectrogram);
        spectrogramSpaces["Bark28_"] = bark28Spectrogram;
    }
    if(whiteL.find("RecChr_") != whiteL.end() && !whiteL["RecChr_"].spectrogram.empty){
        std::vector<std::vector<float>> recChromSpectrogram  = calcChromaticSpectrogram(linSpectrogram, "rectangular");
        spectrogramSpaces["RecChr_"] = recChromSpectrogram;

    }
    if(whiteL.find("Log_") != whiteL.end() && !whiteL["Log_"].spectrogram.empty){
        std::vector<std::vector<float>> logSpectrogram   = calcLogSpectrogram(linSpectrogram);
        spectrogramSpaces["Log_"] = logSpectrogram;

    }
    if(whiteL.find("Mel96_") != whiteL.end() && !whiteL["Mel96_"].spectrogram.empty){
        std::vector<std::vector<float>> mel96Spectrogram = calcMelBands(this->monoData);
        spectrogramSpaces["Mel96_"] = mel96Spectrogram;

    }

    /****
     ***-----Spectrogram Features
     ****/
    std::map<std::string, std::vector<std::vector<float>> >::iterator its = spectrogramSpaces.begin();
    while(its != spectrogramSpaces.end()){
        
    // do not calculate FACM and GLCM with linear spectrogram. too time consuming
        if( whiteL[its->first].spectrogram.facm.size() > 0){
            
            if(verbose) std::cout << "FACM" << std::endl;
            std::string defaultNote = "FACM on "+its->first+", Torus, full";
            std::vector<double> haralicks = getHaralicks(getFACM(its->second));
            for(const int & har_i : whiteL[its->first].spectrogram.facm){
                string fName = "Har-"+its->first+"-FACM_" +std::to_string(har_i);
                this->features[fName] = AudioFeature(fName, haralicks[har_i-1], defaultNote);

            }
        }
        if( whiteL[its->first].spectrogram.glcm.size() > 0){
            
            if(verbose) std::cout << "getGLCM " << std::endl;
            string defaultNote = "GLCM based";
            std::vector<double> haralicks = getHaralicks(this->getGLCM(its->second));
            for(const int & har_i : whiteL[its->first].spectrogram.glcm){
                    string fName = "Har-"+its->first+"-GLCM_" +std::to_string(har_i);
                    this->features[fName] = AudioFeature(fName, haralicks[har_i-1], defaultNote);

            }
        }
        its++;
    }
    return linSpectrogram;
}



/**
 * Calculates features for each sample
 * @param selection := is a selection of features, therefore not everything has to be calculated.
 * @param selFeatures := Only calculates a subset of features specified in this vector. IF empty, all features will be calculated
 * @param verbose := print out information
 */
void AudioFile::populateFeatures(std::unordered_map<std::string, SubFeatureSelector> & selection, bool  verbose){

   
    this->calculateMonoSignalFeatures(selection, verbose);
    
    std::vector<std::vector<float>> spectrogram = this->calculateSpectrogramFeatures(selection, verbose);

    this->calculateSpectrumFeatures(selection, spectrogram,verbose);
        
    this->features["file_path"] = AudioFeature("file_path" , this->fileLocation, "");
    
    if(verbose) std::cout << " features calculated" << std::endl;
}

/**
 * Creates a Whitelist of features to calculate. These are ordered by: (spectrum to calculate -> features of spectrum) or just the feature
 * @param selFeat  :=  a list of selected features. if empty, all features will be selected
 * @return the corresponding whitelist
 */
std::unordered_map<std::string, std::unordered_set<std::string>> prepareFeatureWhiteList(std::vector<std::string> selFeat){
    
    std::unordered_map<std::string, std::unordered_set<std::string>> subFeat;
    
    std::vector<std::string> spectra = {"Lin_", "TriChr_", "Bark28_", "RecChr_", "Log_", "Mel96_","Rhythm13",
                                        "chords_", "key_","Effective"}; // should contain this
    
    std::vector<std::string> specFeat = {"AutoCorrelation", "Centroid", "EssEntropy", "RollOff", "SpectralComplex", // should contain this
                                         "SpectralFlux", "StrongPeak", "flatness", "kurtosis", "skewness", "spread", "FACM_",
                                        "GLCM_","-confidence", "-bpm", "changes_rate", "chords_key",
                                        "number_rate","scale", "key", "scale", "strength", "Duration", "ToStdDuration"}; 
    
    std::vector<std::string> uncombFeat  = {"MFCC", "AutoCorrelation_sig", // should not contain this/be this
                                        "TCToTotal", "ZeroCrossingRate", "sample_length"};
    for( std::string select : selFeat){
        bool breakFlag = false;
        //check for spectral features
        for( std::string specPart : spectra){
            if(select.find(specPart) != std::string::npos){
                for( std::string featPart : specFeat){
                    
                    //check if selected feature contains a "subspecific feature" like kurtosis; this will be calculated with multiple spectra
                    int start = select.find(specPart+featPart);
                    if(start != std::string::npos){
                        subFeat[specPart].insert(featPart);
//                        subFeat[specPart].insert(select.substr(start, select.size()));
                        breakFlag = true;
                        break;
                    }
                }
                if( breakFlag ){
                    break;
                }
            }
        }
        //add temporal features (expect MFCC, for which this nested assignment yields something (the others should be empty)
        //"look if feature start with a substring defined above, if yes, copy the substring as first key, and the rest of the string as second value
        for( std::string uncomb : uncombFeat){
            if(select.rfind(uncomb,0) == 0){
                subFeat[uncomb].insert(select.substr(uncomb.size(), select.size()));
                break;
            }
        }
    }
    return subFeat;
}
/**
 * Normalize all Audio-Features, so that each feature has a range from 0 to 1
 * @param audiofiles := vector of audiofiles which contain the audio features
 */
void normalizeFeatures(std::vector<AudioFile> & audiofiles){
    const int FEAT = 0;
    if( !audiofiles.empty() ){
        
        std::unordered_map<string, AudioFeature>::iterator itF = audiofiles[0].getAudioFeatures().begin();

        
        //iterate through all possible features
        while( itF != audiofiles[0].getAudioFeatures().end()){
            const string & fName = itF->first;
            AudioFeature & feat = itF->second;
            //ignore string based features and multi valued features
            if( feat.values.size() == 1){
                double max = feat.values[FEAT];
                double min = max;
                //iterate trough all files
                for( AudioFile & audio : audiofiles){
                    // only normalize the same features
                    max = std::max(audio.getAudioFeatures()[fName].values[FEAT],max);
                    min = std::min(audio.getAudioFeatures()[fName].values[FEAT],min);
                }
                for( AudioFile & audio : audiofiles){
                        audio.getAudioFeatures()[fName].values[FEAT] = scaleValue(audio.getAudioFeatures()[fName].values[FEAT], max, min, 1.0, 0.0);
                }
            }
            itF++;

        }
    }
}
int AudioFile::getClusterID(){
    return this->clusterID;
}
void printClusterWithFiles(std::vector<AudioFile> & audiofiles){
    
    for( AudioFile & af : audiofiles){
        std::cout << "file: " << af.getFileLocation() + "\tcluster: " << af.getClusterID() << std::endl;
    }
}

void AudioFile::setCluster(int clID){
    this->clusterID = clID;
}

/**
 * Extract a subset of AudioFiles from the n smallest distances in a condensed distance matrix
 * 
 * @param files := all Audio files
 * @param subF  := The selection of the condensed distance matrix
 * @return 
 */
std::vector<AudioFile> getSubAudioFiles(std::vector<AudioFile> & files, std::map<double,int> subF){
    
    std::vector<AudioFile> subFiles;
    subFiles.reserve(subF.size());
    for (std::map<double, int>::iterator it = subF.begin(); it != subF.end(); ++it){
        subFiles.push_back(files[it->second]);

    }
    return subFiles;
}

void addClusterToAudioFiles( std::vector<AudioFile>& allFiles, std::vector<int> & clLabels){
    
    for (int i = 0; i < allFiles.size(); ++i){
        allFiles[i].setCluster(clLabels[i]);
    }
}
/**
 * Assigns all ground truth labels to its sample, If APP_PARMS.withGT == False ->assing empty
 * @param gts = list of all ground truth entries
 * @param files = all audio files
 */
void assignGroundTruthToFiles(std::vector<SampleGT> & gts, std::vector<AudioFile> & files, bool withGT, int gtLabel, bool verbose){
    
    SampleGT noGT;
    noGT.dirName = "no Truth";
    noGT.fileName = "no Truth";
    noGT.label01 = "no Truth";
    noGT.label02 = "no Truth";
    noGT.label03 = "no Truth";
    
    // write gt lables in temporary set
    std::unordered_map<std::string, SampleGT> gt_map;
    for(SampleGT gt : gts){
        gt_map[gt.dirName + gt.fileName] = gt;
    }
    if( withGT){
        std::cout << "Try to assign the GT-Label to each audio sample" << std::endl;
        int i =1;
        for(AudioFile & file : files){
            auto gt_temp = gt_map.find( file.getParentDirName() + file.fileName());
            if( gt_temp != gt_map.end()){
                 file.assignGT(gt_temp->second);
            }else{
                file.assignGT(noGT);
            }
            if(verbose){
                std::cout << file.getGroundTruthLabel(gtLabel)<< " file/dir: "<< file.getGroundTruth().fileName << " " << file.getGroundTruth().dirName << std::endl;
            }
            ++i;
            if( (i+1) % 10 == 0){
                std::cout << std::to_string(i+1) << "|" << std::to_string(files.size()) << " audio files are assigned with their GT" << std::endl;
            }
        }
    }else{
        if(verbose){
            std::cout<< "These files have no GT-label assigned" << std::endl;
            for(AudioFile & file : files){
                file.assignGT(noGT);
            }
        }
    }
}

void populateFeaturesToAllFiles(std::vector<AudioFile> & files, std::vector<string> userSelection, bool verbose){
    
    int i = 1;
    std::unordered_map<std::string, SubFeatureSelector> selected = fillFeatureSelector(userSelection); // helper map for selection subset of features. Key := spectrumMode (i.e. linear), value := set of features
    for(AudioFile & file : files){
        if(verbose){
            std::cout << "Sample length: " << file.getMonoData().size();
            std::cout << "  - File: " << file.getFileLocation() << std::endl;
        }
        file.populateFeatures(selected, false);
        ++i;
        if(i % 10 == 0 || i == files.size()){
            std::cout << std::to_string(i) << "|" << std::to_string(files.size()) << " with features populated" << std::endl;
        }
    }
}

void reorganizeAudioFiles(std::vector<AudioFile> & files, std::string sampleDirPath, OrgaStruct modeData, std::string loc){
    
    std::cout << "sampleDirPath:" <<sampleDirPath << std::endl;
    std::cout << "loc:" <<loc << std::endl;
//    std::string subFolder = getFileName(getNameInPath(sampleDirPath, 0, true));
    std::string subFolder;
    if(modeData.name == "OneFile"){
        subFolder = files[0].getParentDirName();
    }else{
        subFolder = getNameInPath(sampleDirPath, 0, true);
    }
    std::cout<< "create folder path " << loc << "   and nested folder " << subFolder << std::endl;
    // create folder for reorganized samples by its ground truth label
    // create parent folder 
    fs::path middleDir = subFolder ;
    std::cout<< "middleDir " << middleDir << std::endl;
    if(modeData.name == "Cluster"){
        middleDir = "Cluster"/ middleDir;
    }else if(modeData.name == "GT"){
        middleDir = "GT" / middleDir;
    }else if(modeData.name == "OneFile"){
        middleDir = "OneFile" / middleDir;
    }
    std::cout<< "middleDir " << middleDir << std::endl;
    fs::path nestedLoc = (fs::path(loc) / (middleDir/ currentDateTime( true)));
    if( ! fs::is_directory(nestedLoc) && ! fs::exists(nestedLoc)){
//        nestedLoc = nestedLoc / std::to_string(0);
        fs::create_directories(nestedLoc);
        std::cout << nestedLoc << " folder created" << std::endl;
    }else{
        int foldCount = 0;
        
        while( fs::is_directory( nestedLoc / std::to_string(foldCount)) && fs::exists( nestedLoc / std::to_string(foldCount))){
            std::cout << nestedLoc / std::to_string(foldCount) << " already exists" << std::endl;
            ++foldCount;
        }
        
        nestedLoc = nestedLoc / std::to_string(foldCount);
        fs::create_directory(nestedLoc);
        std::cout << (nestedLoc) << " folder created" << std::endl;

    }
    
    //create sub folder and copy their files into it
    std::cout<< files.size() << "  files to move" << std::endl;
    for(int j = 0; j<files.size(); j++){
            //create label folder
        fs::path labelName;
        if(modeData.name == "Cluster"){
            labelName = nestedLoc / std::to_string(files[j].getClusterID());
        }else if(modeData.name == "GT"){
            labelName = nestedLoc /files[j].getGroundTruthLabel(modeData.value);
        }else if(modeData.name == "OneFile"){
            labelName = nestedLoc;
        }
        if ( !fs::is_directory(labelName) || !fs::exists(labelName)) {
            fs::create_directory(labelName);
            std::cout << labelName.string() << " folder created" << std::endl;
        }
                //copy file.
        fs::path newFileName =  labelName / (files[j].getParentDirName()+"_-_"+files[j].fileName(true));
        if( !fs::exists(newFileName)){
            fs::copy(files[j].getFileLocation(), newFileName);
            std::cout << "copied file from: " << files[j].getFileLocation() << " to " <<(labelName / (files[j].getParentDirName()+"_-_"+files[j].fileName(false))) << std::endl;
        }else{
            std::cout << newFileName << " already exists" << std::endl;
        }
    }
}


size_t getTotalSampleLength(std::vector<AudioFile> & files){
    size_t total_length = 0;
    for(AudioFile & f : files){
        total_length += f.getMonoData().size();
    }
    return total_length;
}

/**
 * Groups all AudioFille which contain ground truth label (SampleGT) by its label
 * @param samples := Audio files with features and ground truth label
 * @param labelElector := choose the label. There are 3 possibilities : (1,2,3)
 * @return grouped Audio files
 */

std::map<std::string, std::vector<AudioFile>> groupFilesByGroundTruth(std::vector<AudioFile> samples, int labelSelector){
    
    std::map<std::string, std::vector<AudioFile>> grouped;
    //choosing label
    for(AudioFile & file : samples){
        std::string label = file.getGroundTruth().getLabel();
        if(grouped.find(label) == grouped.end()){
            grouped.insert(std::make_pair(label, std::vector<AudioFile>{file}));
        }else{
            grouped.at(label).push_back(file);
        }          
    }
    
    return grouped; 
}