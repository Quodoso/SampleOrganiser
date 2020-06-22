/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AudioFile.h
 * Author: lars
 *
 * Created on May 21, 2020, 9:15 AM
 */

#ifndef AUDIOFILE_H
#define AUDIOFILE_H

class AudioFile {
    string fileLocation;
    string code;
    int bitrate;
    int samplesize;

    vector<float> meanSpectrum;
    vector<float> meanFreqBands;
    vector<float> meanMelBands;
    vector<float> logSpectrum;
    vector<vector<float>> logSpectrogram;
    vector<vector<float>> freqBands;
    vector<vector<float>> spectrogram;
    vector<vector<float>> melBands;
    vector<vector<double>> facm;
    vector<vector<double>> glcm;
    vector<float> haralicks;
    vector<float> monoData;
    SampleGT groundTruth;
    int clusterID = -1;
    std::unordered_map<string, AudioFeature> features;
    
public:
    AudioFile();
    AudioFile(const AudioFile& orig);
    AudioFile(string fileLocation, bool onlyPath = false);
    virtual ~AudioFile();
    
    std::string fileName(bool replaceSpace=true);
    std::string getParentDirName();
    std::string getFileLocation();
    void assignGT(SampleGT gt);
    
    vector<vector<float>> getSpectrogram();
    
    void populateFeatures(std::unordered_map<std::string, SubFeatureSelector> & tonalExtractor, bool verbose = false);
    
    void printAudioFeatures(bool withNotes = false);
    
    int getClusterID();
    
    void addFeature(std::string name, AudioFeature feature);
    /**
     * calculates the Gray-level-coocurence matrix of a matrix (i.e. spectrogram)
     * 
     * @param spectrogram := 2D-Matrix (nested vector) of any NumericType. Should be a spectrogram
     * @return  GLCM matrix
     */  
    template <class NumericType>
    std::vector<std::vector<double>> getGLCM( std::vector<std::vector<NumericType>> spectrogram){
        if(spectrogram.size() > 0 && spectrogram[0].size() > 0){
            return GLCM<NumericType>(spectrogram, spectrogram[0].size(), spectrogram[0].size()).getGLCM();
        }
        return std::vector<std::vector<double>>{{0}};
    }
    
//    vector<double> getHaralicks(std::vector<std::vector<double>> glcm);
    
    vector<float> getMonoData();
    
    std::vector<double> getSubSetFeatures(std::vector<string> subset);
    
    void setCluster(int clID);
    
    std::unordered_map<string, AudioFeature> & getAudioFeatures();
    SampleGT getGroundTruth();
    /**
     * Returns one GT label.
     * @param labelSelector := (1,2,3) select a Ground truth label
     * @return the corresponding GT-Label
     */
    std::string getGroundTruthLabel(int labelSelector = 1);
    std::string getGroundTruthLabel(std::string labelSelector = "");
private:
    void calculateMonoSignalFeatures(std::unordered_map<std::string, SubFeatureSelector> & whiteL, bool & verbose);
    void calculateSpectrumFeatures(std::unordered_map<std::string, SubFeatureSelector> & whiteL,std::vector<std::vector<float>> & spectrogram, bool verbose);
    std::vector<std::vector<float>> calculateSpectrogramFeatures(std::unordered_map<std::string, SubFeatureSelector> & whiteL, bool verbose);
};



void printSpecificFeature(std::vector<AudioFile> & files, string feat);
void printAllAudioFeatures(std::vector<AudioFile> & files);
/*
 * These are function for vectors of AudioFiles
 */

void printClusterWithFiles(std::vector<AudioFile> & audiofiles);
    
std::unordered_map<std::string, std::unordered_set<std::string>> prepareFeatureWhiteList(std::vector<std::string> selFeat);


//template <class NumericValue>
std::vector<AudioFile> getSubAudioFiles(std::vector<AudioFile> & files, std::map<double,int> subF);


void addClusterToAudioFiles( std::vector<AudioFile>& allFiles, std::vector<int> & clLabels);

void normalizeFeatures(std::vector<AudioFile>& files);

void reorganizeAudioFiles(std::vector<AudioFile> & files, std::string sampleDirPath, OrgaStruct modeData, std::string loc="./reorganized_samples/");

void assignGroundTruthToFiles(std::vector<SampleGT> & gts, std::vector<AudioFile> & files, bool withGT = false, int gtLabel = 1, bool verbose = false);

void populateFeaturesToAllFiles(std::vector<AudioFile> & files, std::vector<string> userSelection, bool verbose = false);

std::map<std::string, std::vector<AudioFile>> groupFilesByGroundTruth(std::vector<AudioFile> samples, int labelSelector = 1);

size_t getTotalSampleLength(std::vector<AudioFile> & files);

#endif /* AUDIOFILE_H */