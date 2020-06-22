/*
 * Copyright (C) 2020 lars
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "stdheader.h"

/**
 * Converts the User input into a unordered_map of booleske tables to check which 
 * feature of which representation should be calculated
 * @param userSelection := Contains only legit names of features
 * @return that strange looking "lookup"-table
 */
std::unordered_map<std::string,SubFeatureSelector> fillFeatureSelector(std::vector<string> userSelection){
    
    std::unordered_map<std::string,SubFeatureSelector> subFeat;
    std::vector<std::string> baseSpect = {"Lin_", "TriChr_", "Bark28_", "RecChr_", "Log_", "Mel96_", "time"}; 

//    //first case: all features set:
//    std::vector<string>::iterator it = std::find(userSelection.begin(), userSelection.end(), "ALL_FEATURES");
//    if( it != userSelection.end()){
//        std::cout << "select all features" << std::endl;
//        for( string base : baseSpect){
//            setAllFeatures(subFeat[base]);
//        }
//    }
    
    std::vector<std::string> specFeat = {"AutoCorrelation", "Centroid", "EssEntropy", "RollOff", "SpectralComplex", // should contain this
                                         "SpectralFlux", "StrongPeak", "flatness", "kurtosis", "skewness", "spread"};
    std::vector<std::string> specificSpectrogram = {"FACM_","GLCM_"};

    for( std::string & select : userSelection){
        bool breackFlag = false;
        //check for base spectrum cases (linear, bark, mel, etc)
        for(std::string & base : baseSpect){
            if(select.find(base) != std::string::npos){
                //check for spectrogram case (FACM, GLCM)
                for( std::string & feat : specificSpectrogram){
                    //save position of "FACM"/"GLCM" for converting their following number into an integer
                    int pos = select.find(feat);
                    if( pos != std::string::npos){
//                        std::cout << select << std::endl;
//                        std::cout << select.substr(pos+feat.size(), select.size()) << std::endl;
//                        std::cout << "select.size():"<<select.size()<< " pos+feat.size():"<<pos+feat.size()<< " "<<std::endl;
                        subFeat[base].parseFlags(feat, abs(std::stoi(select.substr(pos+feat.size(), select.size()))));
                        breackFlag = true;
                        break;
                    }
                }
                if(breackFlag){ break;}
                //check for spectrum case (Centroid, Strongpeak,...)
                for( std::string & feat : specFeat){
                    if(select.find(feat) != std::string::npos){
//                        std::cout << select << std::endl;
                        subFeat[base].parseFlags(feat);
                        breackFlag = true;
                        break;
                    }
                }
            }else{
                std::string mfcc = "MFCC-";
                if(select.find(mfcc) != std::string::npos) {
//                    std::cout << select << std::endl;
//                    std::cout << "select.size():"<<select.size()<< " pos+feat.size():"<<select.find(mfcc)<< "+"<<mfcc.size()<< " "<<std::endl;
                    auto ab = abs(std::stoi(select.substr(mfcc.size(),select.size())));
                    subFeat["Mel96_"].parseFlags("MFCC", ab);
                    breackFlag = true;
                    break;
                }
            }
        }
        if(breackFlag){continue;}
        subFeat["time"].parseFlags(select);

    }
    return subFeat;
}

void setAllFeatures(SubFeatureSelector & features){
    features.TCToTotal = true;
    features.autoCorrelation_sig = true;
    features.effectiveDuration = true;
    features.effectiveToSdtDuration = true;
    features.rhytm2013.empty = false;
    features.rhytm2013.rhythmBPM = true;
    features.rhytm2013.rhythmConfidence = true;
    features.sampleLength = true;
    features.spectrogram.empty = false;
    features.spectrogram.facm = std::vector<int>(12);
    std::iota (std::begin(features.spectrogram.facm ), std::end(features.spectrogram.facm ), 1);
    features.spectrogram.glcm = std::vector<int>(12);
    std::iota (std::begin(features.spectrogram.glcm ), std::end(features.spectrogram.glcm ), 1);
    features.spectrum.autocorrelation = true;
    features.spectrum.centroid = true;
    features.spectrum.empty = false;
    features.spectrum.essEntropy = true;
    features.spectrum.flatness = true;
    features.spectrum.mfcc =  std::vector<int>(13);
    std::iota (std::begin(features.spectrum.mfcc ), std::end(features.spectrum.mfcc  ), 1);
    features.spectrum.rolloff = true;
    features.spectrum.specComplex.empty = false;
    features.spectrum.specComplex.kurtosis = true;
    features.spectrum.specComplex.skewness = true;
    features.spectrum.specComplex.spread = true;
    features.spectrum.spectralComplexity = false;
    features.spectrum.spectralFlux = true;
    features.spectrum.strongPeak = true;
    features.zeroCrossingRate = true;
    features.tonalExtractor.empty = false;
    features.tonalExtractor.chords_changes_rate = true;
    features.tonalExtractor.chords_histogram = true;
    features.tonalExtractor.chords_key = true;
    features.tonalExtractor.chords_number_rate = true;
    features.tonalExtractor.chords_progression = true;
    features.tonalExtractor.chords_scale = true;
    features.tonalExtractor.chords_strength = true;
    features.tonalExtractor.key_key = true;
    features.tonalExtractor.key_scale = true;
    features.tonalExtractor.key_strength = true;
}