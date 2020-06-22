/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   essentiaInterface.h
 * Author: lars
 *
 * Created on May 21, 2020, 12:20 PM
 */
//TODO [optional] := check the necessity of a 5% hop size. This can be increased!
#ifndef ESSENTIAINTERFACE_H
#define ESSENTIAINTERFACE_H

const float DBFS_ADJUST = 100.0;

const int FRAMESIZE  = 2048 * 2;
const int HOPSIZE    = 256;
//const int HOPSIZE    = 2048;
const int SAMPLERATE = 44100;
const string WINDOW_TYPE = "hann";
const int MELBAND = 96;

//self calculated bark bands ->
/*[0,100,200,300,410,520,640,770,910,1070,1250,1450,1680,1960,2290,2700,3200,3810,4540,5400,6400,7600,9150,11400,15410,22030,22050]
*/

bool initEssentia();

bool shutdownEssentia();

void getMonoAudio(string&, std::vector<float>& );

/**
 * Load audio file with normalized loudness. normalized to REPLAYGAIN (-9 dB)
 * @param 
 * @param 
 */
void getMonoAudioEQLoud(string&, std::vector<float>& , float replaygain = -9);



/**
 * Converts an audio signal with range (-1,1) to the dB-Scale
 * @param signal
 * @param dest
 * @param mode := {"power_ratio"/"amplitude_ratio"}. power ratio =  with 10*log10(x/1), amplitude ratio =  with 10*log10(x*x/1*1) = 20*log10(x/1)
 */
void convertLinearSignalToDBScale(vector<float>& signal, std::vector<float>& dest, std::string mode ="power_ratio", bool makePositive = true);



/****************************************
 ********--Spectrum and Spectrogram--****
 *****************************************/
std::vector<float> calcMeanSpectrum(std::vector<float>&);

vector<float> calcMeanSpectrum(std::vector<std::vector<float>>& spectrogram);

std::vector<std::vector<float>> calcSpectrogram(std::vector<float> & monoData);

///** Mel Bands **///


std::vector<std::vector<float>> calcMelBands(std::vector<float> & signal, bool log = true, std::string normalize = "unit_sum", std::string type = "power");

std::vector<float> calcMelBandsSingle(vector<float> & spectrum, bool log = true, std::string normalize = "unit_sum", std::string type = "power");


///** Bark Bands **///

std::vector<float> calcBarkBands(std::vector<float>& spectrum, int numberBands = 28);

std::vector<std::vector<float>> calcBarkBandSpectrogram(std::vector<std::vector<float>>& spectrogram, int numberBands = 28);


///** Log **///

std::vector<float> calcLogSpectrum(vector<float>& spectrum, std::string output="logFreqSpectrum", float binsPerSemitone=3, int frameSize=FRAMESIZE/2+1, float rollOn=0);

std::vector<std::vector<float>> calcLogSpectrogram(vector<vector<float>>& spectrogram, std::string output="logFreqSpectrum", float binsPerSemitone=3, int frameSize=FRAMESIZE/2+1, float rollOn=0);

///** Frequency Bands with Rectangular and Triangular Window **///

std::vector<float> calcFrequencyBands(vector<float> & spectrum, std::vector<float> frequencyBands= {});

std::vector<std::vector<float>> calcFrequencyBandSpectrogram(vector<vector<float>>& spectrogram, std::vector<float> frequencyBands= {});

std::vector<float> calcTriangularBands(std::vector<float>& spectrum, std::vector<float> frequencyBands = {}, 
                int inputSize = 0, bool log = true, std::string normalize = "unit_sum", std::string type = "power", std::string weighting = "linear");

std::vector<std::vector<float>> calcTriangularBandSpectrogram(std::vector<std::vector<float>>& spectrogram, std::vector<float> frequencyBands = {}, 
                int inputSize = 0, bool log = true, std::string normalize = "unit_sum", std::string type = "power", std::string weighting = "linear");

///** Chromatic with Triangular window*///

// mode \in ( "triangular", "rectangular")
std::vector<float> calcChromaticSpectrum(std::vector<float>& spectrum, std::string mode = "triangular");
// mode \in ( "triangular", "rectangular")
std::vector<std::vector<float>> calcChromaticSpectrogram(std::vector<std::vector<float>>& spectrogram, std::string mode = "triangular");


void calcSpectralCentroid(std::vector<float>&, float&);

void calcZeroCrossingRate(std::vector<float>&, float&);


void calcSpectralFlux(vector<float>& spectrum, std::unordered_map<string,AudioFeature> & dest, bool halfRectify = false, std::string norm = "L1", std::string addInfo = "Lin_");

void calcSpectralCentroid(std::vector<float>& array, std::unordered_map<string,AudioFeature> & dest, float range = SAMPLERATE/2, std::string addInfo = "Lin_");

void calcRollOff(std::vector<float>& spectrum, std::unordered_map<string,AudioFeature> & dest, float cutoff = 0.85, std::string addInfo = "Lin_");

void calcSpectralComplexity(std::vector<float>& spectrum, std::unordered_map<string,AudioFeature> & dest, float magnitudeThreshold = 0.005, std::string addInfo = "Lin_");

void calcStrongPeak(std::vector<float>& spectrum, std::unordered_map<string,AudioFeature> & dest, std::string addInfo = "Lin_");

void calcLoopBpmEstimator(std::vector<float>& signal, std::unordered_map<string,AudioFeature> & dest, float confidenceThreshold = 0.95);

void calcRhythmExtractor2013(std::vector<float>& signal, std::unordered_map<string,AudioFeature> & dest, Rhythm2013ExtractorParams & select, int maxTempo = 208, int minTempo = 40, std::string method = "multifeature");

void calcDanceability(std::vector<float>& signal, std::unordered_map<string,AudioFeature> & dest, std::string addInfo = "Lin_");

void calcEssEntropy(std::vector<float>& array, std::unordered_map<string,AudioFeature> & dest, std::string addInfo = "Lin_");

void calcDistributionShape(std::vector<float>& array, std::unordered_map<string,AudioFeature> & dest, SpectComplexExtractorParams & select, std::string mode="pdf", std::string addInfo = "Lin_");

void calcFlatness(std::vector<float>& array, std::unordered_map<string,AudioFeature> & dest, std::string addInfo = "Lin_");

void calcTCToTotal(std::vector<float>& signal, std::unordered_map<string,AudioFeature> & dest, bool applyRectification = true, float attackTime = 10, float releaseTime = 1500);

void calcEffectiveDuration(std::vector<float>& signal, std::unordered_map<string,AudioFeature> & dest, float thresholdRatio =0.2);

void calcZeroCrossingRate(std::vector<float>& signal, std::unordered_map<string,AudioFeature> & dest, float threshold = 0.0);

void calcTonalExtractor(std::vector<float>& signal, std::unordered_map<string,AudioFeature> & dest, TonalExtractorParams & featureSet, int frameSize = 4096, int hopSize =1024, float tuningFrequency = 440);


std::vector<float> calcMFCC(vector<float>& spectrum, float highFrequencyBound = 11000, int inputSize = 1025, int numberBands = 40, int numberCoefficients = 13);

void calcHRFs(std::vector<float> &spectrum, std::vector<AudioFeature>& dest, float magnitudeThreshold = 0.0, 
            float maxFrequency = 12000, int maxPeaks = 250, int size = 144, float windowSize = 1.0/12.0, int maxPeaksHRF = 48);


/**
 * Calculates the chromatic half note scale with formula: refNote* 2^(i/12)   
 * -> 440 * 2(-12/12)= 220 Hz etc. 
 * -> 440 * 2(107/12) = 21263 Hz
 * The resulting vector which contains the center frequency of each note.
 * If adjust_50_cents is true, every note will now represent the lower limit of its note (-50 cents)
 * If add_50Cents_Steps is true, the resulting vector will be populated with all lower limits of each note
 *      -> a lower limit of one note is the upper limit of the lower not
 *      -> This can be used for a high profile triangular bands Spectrogram/Spectrum
 * 
 * @param begin             := lowest exponent in formula: refNote * 2^(i/12)   (default -37 )
 * @param end               := highest exponent in formula: refNote * 2^(i/12)  (loops includes this number) (default 67 ) ~ 21096 Hz
 * @param step              := step size ( i.e.: 1, 0.5, 0.33, 0.75, 2, ...) (default 1 )
 * @param refNote           := the reference note. (default 440 Hz)  
 * @param interpol_50_cents := interpolate 50 cents steps with average between two notes (note[i]+note[i+1])/2   (default true )
 * @param only_50_cents     := only save 50 cents steps (lower bound of note) this can be used for rectangular filter)  (default false)
 * @return 
 */
std::vector<float> getChromaticFreqs(float begin = -37.0, float end = 67.0, float step = 1.0, float refNote = 440, bool interpol_50_cents = true, bool only_50_cents = false);

#endif /* ESSENTIAINTERFACE_H */

