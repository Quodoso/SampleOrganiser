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

/* 
 * File:   featureSelektor.h
 * Author: lars
 *
 * Created on October 15, 2020, 1:49 PM
 */

#ifndef FEATURESELEKTOR_H
#define FEATURESELEKTOR_H


//This struct serves as data holder for the function calcTonalExtractor() in essentiaInterface
typedef struct TonalExtractorParams_
{
    bool empty = true;
    bool chords_changes_rate = false;
    bool chords_histogram = false;
    bool chords_key = false;
    bool chords_number_rate = false;
    bool chords_progression = false;
    bool chords_scale = false;
    bool chords_strength = false;
    bool key_key = false;
    bool key_scale = false;
    bool key_strength = false;
}TonalExtractorParams;

typedef struct SpectComplexExtractorParams_
{
    bool empty = true;
    bool kurtosis = false;
    bool skewness = false;
    bool spread = false;
}SpectComplexExtractorParams;

typedef struct Rhythm2013ExtractorParams_
{
    bool empty = true;
    bool rhythmConfidence = false;
    bool rhythmBPM = false;
}Rhythm2013ExtractorParams;
typedef struct SpectrogrammExtractParams_
{   
    bool empty = true;
    std::vector<int>  facm = std::vector<int>{};
    std::vector<int>  glcm = std::vector<int>{};
}SpectrogrammExtractParams;
typedef struct SpectrumExtractParams_
{
    bool empty = true;
    bool rolloff = false;
    bool spectralComplexity = false;
    bool strongPeak = false;
    bool essEntropy = false;
    SpectComplexExtractorParams specComplex;
    bool flatness = false;
    bool centroid = false;
    bool spectralFlux = false;
    bool autocorrelation = false;
    std::vector<int> mfcc;
    
}SpectrumExtractParams;
typedef struct SubFeatureSelector_{
    SpectrumExtractParams spectrum = SpectrumExtractParams();             //combined selection better usage
    SpectrogrammExtractParams spectrogram = SpectrogrammExtractParams();    // combined selection better usage
    Rhythm2013ExtractorParams rhytm2013 = Rhythm2013ExtractorParams();        // combined selection for passing to essentia
    TonalExtractorParams tonalExtractor = TonalExtractorParams(); // combined selection for passing to essentia
    bool TCToTotal = false;
    bool effectiveDuration = false;
    bool effectiveToSdtDuration = false;
    bool zeroCrossingRate = false;
    bool sampleLength = false;
    bool autoCorrelation_sig = false;
    
    /**
     * parsing user selected features in this booleske hell. 
     * @param in := description of a feature
     * @param num := if it has numbers, there it goes;
     * @return 
     */
    int parseFlags(std::string in, int num = 0){
//        std::cout << " in: "<< in << "  num: "<<num << std::endl;
        if(in == "RollOff") {this->spectrum.rolloff = true; this->spectrum.empty = false; return 1;}
        if(in == "SpectralComplex"){ this->spectrum.spectralComplexity = true; this->spectrum.empty = false; return 1;};
        if(in == "StrongPeak") {this->spectrum.strongPeak = true; this->spectrum.empty = false; return 1;};
        if(in == "EssEntropy") {this->spectrum.essEntropy = true; this->spectrum.empty = false; return 1;};
        if(in == "flatness") {this->spectrum.flatness = true; this->spectrum.empty = false; return 1;};
        if(in == "Centroid") {this->spectrum.centroid = true; this->spectrum.empty = false; return 1;};
        if(in == "SpectralFlux") {this->spectrum.spectralFlux = true; this->spectrum.empty = false; return 1;};
        if(in == "AutoCorrelation") {this->spectrum.autocorrelation = true; this->spectrum.empty = false; return 1;};
        if(in == "kurtosis") {this->spectrum.specComplex.kurtosis = true; this->spectrum.empty = false; this->spectrum.specComplex.empty = false; return 1;};
        if(in == "skewness") {this->spectrum.specComplex.skewness = true; this->spectrum.empty = false; this->spectrum.specComplex.empty = false; return 1;};
        if(in == "spread") {this->spectrum.specComplex.spread  = true; this->spectrum.empty = false; this->spectrum.specComplex.empty = false;return 1;};
        
        if(in == "Rhythm13-bpm") {this->rhytm2013.rhythmBPM = true; this->rhytm2013.empty = false; return 1; }
        if(in == "Rhythm13-confidence") {this->rhytm2013.rhythmConfidence = true; this->rhytm2013.empty = false; return 1; }
        
        if(in == "chords_key") { this->tonalExtractor.chords_key = true; this->tonalExtractor.empty = false; return 1;}
        if(in == "chords_changes_rate") { this->tonalExtractor.chords_changes_rate  = true;  this->tonalExtractor.empty = false; return 1;}
        if(in == "chords_number_rate") { this->tonalExtractor.chords_number_rate = true;  this->tonalExtractor.empty = false; return 1;}
        if(in == "chords_scale") { this->tonalExtractor.chords_scale = true;  this->tonalExtractor.empty = false; return 1;}
        if(in == "key_key") { this->tonalExtractor.key_key = true;  this->tonalExtractor.empty = false; return 1;}
        if(in == "key_scale") { this->tonalExtractor.key_scale = true;  this->tonalExtractor.empty = false; return 1;}
        if(in == "key_strength") { this->tonalExtractor.key_strength = true;  this->tonalExtractor.empty = false; return 1;}

        if(in == "AutoCorrelation_sig"){this->autoCorrelation_sig = true; return 1;}
        if(in == "ZeroCrossingRate"){this->zeroCrossingRate = true; return 1;}
        if(in == "sample_length"){this->sampleLength = true; return 1;}
        if(in == "EffectiveToStdDuration"){this->effectiveToSdtDuration = true; return 1;}
        if(in == "EffectiveDuration"){this->effectiveDuration = true; return 1;}
        if(in == "TCToTotal"){this->TCToTotal= true; return 1;}
        
        if(in == "FACM_") {this->spectrogram.facm.push_back(num); this->spectrogram.empty = false; return 1;};
        if(in == "GLCM_") {this->spectrogram.glcm.push_back(num); this->spectrogram.empty = false; return 1;};
        if(in == "MFCC") {this->spectrum.mfcc.push_back(num); this->spectrum.empty = false; return 1;};
        
        return 0;
    }
    
}SubFeatureSelector;

std::unordered_map<std::string,SubFeatureSelector> fillFeatureSelector(std::vector<string> userSelection);

//SubFeatureSelector ALLFEATURES;

void setAllFeatures(SubFeatureSelector & features);

#endif /* FEATURESELEKTOR_H */

