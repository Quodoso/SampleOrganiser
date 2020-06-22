/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/** To implement
 https://essentia.upf.edu/reference/std_DerivativeSFX.html
 * https://essentia.upf.edu/reference/std_Flux.html
 *  * 
 * 
 * https://essentia.upf.edu/reference/std_AfterMaxToBeforeMaxEnergyRatio.html
 * 
 * since everything should be RMS normalized this might be useless
 * https://essentia.upf.edu/reference/std_TruePeakDetector.html
 *  https://en.wikipedia.org/wiki/Energy_(signal_processing)
 */
#include "stdheader.h"

using namespace essentia;
using namespace essentia::standard;



bool initEssentia(){
    
    if(essentia::isInitialized()){
        return true;
    }else{
        essentia::init();
        return true;
    }
    return false;
}

bool shutdownEssentia(){
    
    
    if(essentia::isInitialized()){
        essentia::shutdown();
        return true;
    }else{
        return true;
    }
    return false;
}

/**
 * Converts an audio signal with range (-1,1) to the dB-Scale
 * @param signal
 * @param dest
 * @param mode := {"power_ratio"/"amplitude_ratio"}. power ratio =  with 10*log(x/1), amplitude ratio =  with 10*log(x*x/1*1)
 * @param makePositive := db FS is usually from ~-98 db FS to 0. some calculatons need positive values; so make maximum between 0 and dbFS+100
 */
void convertLinearSignalToDBScale(vector<float>& signal, vector<float>& dest, std::string mode, bool makePositive){
    
    dest.resize(signal.size());
    if(mode == "power_ratio"){
        for(int i =0; i<signal.size(); ++i){
            dest[i] = 10* std::log10(std::abs(signal[i]));
            if(makePositive){
                dest[i] = std::max(0.01f, dest[i] + DBFS_ADJUST);
            }
        }
    }else{
        if(mode == "amplitude_ratio"){
            for(int i =0; i<signal.size(); ++i){
                dest[i] = 20* std::log10(std::abs(signal[i]));
                if(makePositive){
                    dest[i] = std::max(0.01f, dest[i] + DBFS_ADJUST);
                }
            }
        }
    }
}


/**
 * Source: https://essentia.upf.edu/reference/std_DynamicComplexity.html
 * 
 * This algorithm computes the dynamic complexity defined as the average absolute deviation from the global loudness level estimate on the dB scale. 
 * It is related to the dynamic range and to the amount of fluctuation in loudness present in a recording. 
 * Silence at the beginning and at the end of a track are ignored in the computation in order not to deteriorate the results.
 * 
 * @param signal := the input audio signal
 * @param dest := pointer to AudioFeature vector to add features (dynamicComplexity, loudness)
 * @param frameSize := the frame size [s] (real ∈ (0, ∞), default = 0.05)
 */
void calcDynamicComplexity(vector<float>& signal, std::unordered_map<string, AudioFeature> & dest, float frameSize){
    
    initEssentia();

    std::string defaultNote = "Params\n frameSize: "+ std::to_string(frameSize);

    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* DynamicComplexity     = factory.create("DynamicComplexity",
                                             "sampleRate", SAMPLERATE,
                                            "frameSize", frameSize);
    
    float dynamicComplexity;
    float loudness;
    DynamicComplexity->input("spectrum").set(signal );
    DynamicComplexity->output("dynamicComplexity").set(dynamicComplexity);
    DynamicComplexity->output("loudness").set(loudness);
    
    dest["DynamicComplexity"] = AudioFeature("DynamicComplexity", dynamicComplexity, defaultNote);
    dest["Loudness"] = AudioFeature("Loudness", loudness, defaultNote);
    
    DynamicComplexity->compute();
    delete DynamicComplexity;
//    essentia::shutdown();
}
/**
 * Source:https://essentia.upf.edu/reference/std_SpectralComplexity.html
This algorithm computes the spectral complexity of a spectrum. The spectral complexity is based on the number of peaks in the input spectrum.
It is recommended that the input "spectrum" be computed by the Spectrum algorithm. The input "spectrum" is passed to the SpectralPeaks algorithm and thus inherits its input requirements and exceptions.
 * @param spectrum := vector<float := mean spectrogram
 * @param dest := pointer to AudioFeature vector to add features
 * @param addInfo := additional Information which like (log, mel, bark, etc) it will be placed infront of the file name
 */
void calcSpectralComplexity(vector<float>& spectrum, std::unordered_map<string, AudioFeature> & dest, float magnitudeThreshold, std::string addInfo){
    
    initEssentia();
    std::string defaultNote = "Params\n magnitudeThreshold: "+ std::to_string(magnitudeThreshold);
    float tmp;


    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* spectralComplexity     = factory.create("SpectralComplexity",
                                             "sampleRate", SAMPLERATE,
                                            "magnitudeThreshold", magnitudeThreshold);
    
    spectralComplexity->input("spectrum").set(spectrum);
    spectralComplexity->output("spectralComplexity").set(tmp);
    
    spectralComplexity->compute();
    
    dest[addInfo+"SpectralComplex"] = AudioFeature(addInfo+"SpectralComplex", tmp, defaultNote);

                
    delete spectralComplexity;
//    essentia::shutdown();
}

/**
 * Source:https://essentia.upf.edu/reference/std_RollOff.html
 * This algorithm computes the roll-off frequency of a spectrum. The roll-off frequency is defined as the frequency under which some percentage (cutoff) of the total energy of the spectrum is contained. The roll-off frequency can be used to distinguish between harmonic (below roll-off) and noisy sounds (above roll-off).
 * An exception is thrown if the input audio spectrum is smaller than 2.
 * @param spectrum := vector<float := mean spectrogram
 * @param dest := pointer to AudioFeature vector to add features
 */
void calcRollOff(vector<float>& spectrum, std::unordered_map<string, AudioFeature> & dest, float cutoff, std::string addInfo){
    
    initEssentia();
    
    std::string defaultNote = "Params\n cuttoff: "+ std::to_string(cutoff);

    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* rolloff     = factory.create("RollOff",
                                             "sampleRate", SAMPLERATE,
                                            "cutoff", cutoff);
    
    float tmp;
    rolloff->input("spectrum").set(spectrum);
    rolloff->output("rollOff").set(tmp);
    
    dest[addInfo+"RollOff"] = AudioFeature(addInfo+"RollOff", tmp, defaultNote);
    
    
    rolloff->compute();
    delete rolloff;
//    essentia::shutdown();
}


/**
 * Source:https://essentia.upf.edu/reference/std_MFCC.html
This algorithm computes the mel-frequency cepstrum coefficients of a spectrum. As there is no standard implementation, the MFCC-FB40 is used by default:
 * 
This algorithm computes the spectral complexity of a spectrum. The spectral complexity is based on the number of peaks in the input spectrum.
It is recommended that the input "spectrum" be computed by the Spectrum algorithm. The input "spectrum" is passed to the SpectralPeaks algorithm and thus inherits its input requirements and exceptions.
 * @param spectrum := vector<float := mean spectrogram
 * @param highFrequencyBound :=the upper bound of the frequency range [Hz] (real ∈ (0, ∞), default = 11000)
 * @param inputSize := the size of input spectrum  (integer ∈ (1, ∞), default = 1025)
 * @param numberBands :=     the number of mel-bands in the filter  (integer ∈ [1, ∞), default = 40)
 * @param numberCoefficients := the number of output mel coefficients  (integer ∈ [1, ∞), default = 13)
 */
std::vector<float> calcMFCC(vector<float>& spectrum, float highFrequencyBound, int inputSize, int numberBands, int numberCoefficients){
    
    initEssentia();
    std::string defaultNote = "Params\n magnitudeThreshold: "+ std::to_string(highFrequencyBound) +
            " numberBands:"+std::to_string(numberBands)+
            " numberCoefficients"+std::to_string(numberCoefficients);
    std::vector<float> tmp;
    std::vector<float> discard;

    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* mfcc     = factory.create("MFCC",
                                             "highFrequencyBound", highFrequencyBound,
                                            "inputSize", inputSize,
                                            "numberBands", numberBands,
                                            "numberCoefficients",numberCoefficients);
    
    mfcc->input("spectrum").set(spectrum);
    mfcc->output("mfcc").set(tmp);
    mfcc->output("bands").set(discard);
    
    mfcc->compute();
   
                
    delete mfcc;
//    essentia::shutdown();
    return tmp;
}

/**
 * Source:https://essentia.upf.edu/reference/std_Flux.html
This algorithm computes the spectral flux of a spectrum. 
Flux is defined as the L2-norm [1] or L1-norm [2] of the difference between two consecutive frames of the magnitude spectrum.
The frames have to be of the same size in order to yield a meaningful result. The default L2-norm is used more commonly.

An exception is thrown if the size of the input spectrum does not equal the previous input spectrum's size.
 *  * @param spectrum := vector<float := mean spectrogram
 * @param dest := pointer to AudioFeature vector to add features
 * @param addInfo := additional Information which like (log, mel, bark, etc) it will be placed infront of the file name
 * @parm halfRectify := half-rectify the differences in each spectrum bin  (bool ∈ {true, false}, default = false)
 * @parm norm :=     the norm to use for difference computation (string ∈ {L1, L2}, default = L2)
 */
void calcSpectralFlux(vector<float>& spectrum, std::unordered_map<string, AudioFeature> & dest, bool halfRectify, std::string norm, std::string addInfo){
    
    initEssentia();
    std::string defaultNote = "Params\n halfRectify: "+ std::to_string(halfRectify) + " norm: "+norm;
    float tmp;


    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* flux     = factory.create("Flux",
                                             "halfRectify", halfRectify,
                                            "norm", norm);
    
    flux->input("spectrum").set(spectrum);
    flux->output("flux").set(tmp);
    
    flux->compute();
    
    dest[addInfo+"SpectralFlux"] = AudioFeature(addInfo+"SpectralFlux", tmp, defaultNote);

                
    delete flux;
//    essentia::shutdown();
}
/**
 * Source: https://essentia.upf.edu/reference/std_StrongPeak.html
This algorithm computes the Strong Peak of a spectrum. The Strong Peak is defined as the ratio between the spectrum's maximum peak's magnitude and the "bandwidth" of the peak above a threshold (half its amplitude). This ratio reveals whether the spectrum presents a very "pronounced" maximum peak (i.e. the thinner and the higher the maximum of the spectrum is, the higher the ratio value).
Note that "bandwidth" is defined as the width of the peak in the log10-frequency domain. This is different than as implemented in [1]. Using the log10-frequency domain allows this algorithm to compare strong peaks at lower frequencies with those from higher frequencies.
An exception is thrown if the input spectrum contains less than two elements.
 * @param spectrum := vector<float := mean spectrogram
 * @param dest := pointer to AudioFeature vector to add features
 */
void calcStrongPeak(vector<float>& spectrum, std::unordered_map<string, AudioFeature> & dest, std::string addInfo){
    
    initEssentia();
    std::string defaultNote = "Params\n";
    float tmp;

    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* strongPeak     = factory.create("StrongPeak");
    
    strongPeak->input("spectrum").set(spectrum);
    strongPeak->output("strongPeak").set(tmp);
    
    strongPeak->compute();
    
    dest[addInfo+"StrongPeak"] = AudioFeature(addInfo+"StrongPeak", tmp, defaultNote);

    delete strongPeak;
//    essentia::shutdown();
}
/**
 * Source: https://essentia.upf.edu/reference/std_RhythmExtractor2013.html
This algorithm extracts the beat positions and estimates their confidence as well as tempo in bpm for an audio signal. 
The beat locations can be computed using:
 -'multifeature', the BeatTrackerMultiFeature algorithm
 -'degara', the BeatTrackerDegara algorithm (note that there is no confidence estimation for this method, the output confidence value is always 0)

 * Source: https://essentia.upf.edu/reference/std_BeatTrackerMultiFeature.html
This algorithm estimates the beat positions given an input signal. 
It computes a number of onset detection functions and estimates beat location candidates from them using TempoTapDegara algorithm. 
Thereafter the best candidates are selected using TempoTapMaxAgreement. 
The employed detection functions, and the optimal frame/hop sizes used for their computation are:
 -complex spectral difference (see 'complex' method in OnsetDetection algorithm, 2048/1024 with posterior x2 upsample or the detection function)
 -energy flux (see 'rms' method in OnsetDetection algorithm, the same settings)
 -spectral flux in Mel-frequency bands (see 'melflux' method in OnsetDetection algorithm, the same settings)
 -beat emphasis function (see 'beat_emphasis' method in OnsetDetectionGlobal algorithm, 2048/512)
 -spectral flux between histogrammed spectrum frames, measured by the modified information gain (see 'infogain' method in OnsetDetectionGlobal algorithm, 2048/512)

 * You can follow these guidelines [2] to assess the quality of beats estimation based on the computed confidence value:
    -[0, 1) very low confidence, the input signal is hard for the employed candidate beat trackers
    -[1, 1.5] low confidence
    -(1.5, 3.5] good confidence, accuracy around 80% in AMLt measure
    -(3.5, 5.32] excellent confidence
 
 * @param signal := the audio input signal
 * @param dest := pointer to AudioFeature vector to add features
 * @param maxTempo := the fastest tempo to detect [bpm] : (integer ∈ [60, 250], default = 208)
 * @param minTempo := the slowest tempo to detect [bpm] : (integer ∈ [40, 180], default = 40)
 * @param method := the method used for beat tracking  : (string ∈ {multifeature, degara}, default = multifeature)
 */
void calcRhythmExtractor2013(vector<float>& signal, std::unordered_map<string, AudioFeature> & dest, Rhythm2013ExtractorParams & select, int maxTempo, int minTempo, std::string method){
    
    //if no feature should be extracted -> return
    if(select.empty) return;
    
    initEssentia();
    std::string defaultNote = "Params\n maxTempo: " + std::to_string(maxTempo)
                            + " maxTempo: " + std::to_string(maxTempo)
                            + " minTempo: " + std::to_string(minTempo)
                            + " method: " + method;
    std::vector<float> ticks;
    std::vector<float> estimates;
    std::vector<float> bpmIntervals;
    float bpm;
    float confidence;
    
    AlgorithmFactory& factory = AlgorithmFactory::instance();
    
    
    Algorithm* rhythm2013     = factory.create("RhythmExtractor2013",
                                                    "maxTempo", maxTempo,
                                                     "minTempo", minTempo,
                                                      "method", method);
    
    rhythm2013->input("signal").set(signal);
    rhythm2013->output("ticks").set(ticks);
    rhythm2013->output("estimates").set(estimates);
    rhythm2013->output("bpmIntervals").set(bpmIntervals);
    rhythm2013->output("confidence").set(confidence);
    rhythm2013->output("bpm").set(bpm);
    
    rhythm2013->compute();
    
    
    //Attention! This is only needed for successfully handling the essentia algorithm
    //I am only considering 1D (one Value)-Features
    //Sadly, sometimes they only trigger one Feature! (when the file is to short)
    //Maybe in later use this can be erased
    bpmIntervals.push_back(0.0);
    bpmIntervals.push_back(0.0);
    estimates.push_back(0.0);
    estimates.push_back(0.0);
    
//    if(bpmIntervals.empty()){
//        bpmIntervals = std::vector<float>{0};
//    }
//    if(estimates.empty()){
//        estimates = std::vector<float>{0};
//    }
    
    
//    dest[AudioFeature("Rhythm13-ticks", ticks, defaultNote));
//    dest[AudioFeature("Rhythm13-estimates", estimates, defaultNote));
//    dest[AudioFeature("Rhythm13-bpmIntervals", bpmIntervals, defaultNote));
    if(select.rhythmConfidence) dest["Rhythm13-confidence"] = AudioFeature("Rhythm13-confidence", confidence, defaultNote);
    if(select.rhythmBPM) dest["Rhythm13-bpm"] = AudioFeature("Rhythm13-bpm", bpm, defaultNote);
    
    delete rhythm2013;
//    essentia::shutdown();
}


/**
 * Source:https://essentia.upf.edu/reference/std_LoopBpmEstimator.html
This algorithm estimates the BPM of audio loops. It internally uses PercivalBpmEstimator algorithm to produce a BPM estimate and LoopBpmConfidence to asses the reliability of the estimate. If the provided estimate is below the given confidenceThreshold, the algorithm outputs a BPM 0.0, otherwise it outputs the estimated BPM. For more details on the BPM estimation method and the confidence measure please check the used algorithms.
 * @param spectrum := vector<float := mean spectrogram
 * @param dest := pointer to AudioFeature vector to add features
 */
void calcLoopBpmEstimator(vector<float>& signal, std::unordered_map<string, AudioFeature> & dest, float confidenceThreshold){
    
    initEssentia();
    std::string defaultNote = "Params\n confidenceThreshold: " + std::to_string(confidenceThreshold);
    float tmp;

    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* loopBpmEstimator     = factory.create("LoopBpmEstimator",
                                            "confidenceThreshold", confidenceThreshold);
    
    loopBpmEstimator->input("signal").set(signal);
    loopBpmEstimator->output("bpm").set(tmp);
    
    loopBpmEstimator->compute();
    
    dest["LoopBpmEstimator"] = AudioFeature("LoopBpmEstimator", tmp, defaultNote);
    
    delete loopBpmEstimator;
//    essentia::shutdown();
}


/**
 * Source:https://essentia.upf.edu/reference/std_Danceability.html
This algorithm estimates the BPM of audio loops. It internally uses PercivalBpmEstimator algorithm to produce a BPM estimate and LoopBpmConfidence to asses the reliability of the estimate. If the provided estimate is below the given confidenceThreshold, the algorithm outputs a BPM 0.0, otherwise it outputs the estimated BPM. For more details on the BPM estimation method and the confidence measure please check the used algorithms.
 * @param spectrum := vector<float := mean spectrogram
 * @param dest := pointer to AudioFeature vector to add features
 */
void calcDanceability(vector<float>& signal, std::unordered_map<string, AudioFeature> & dest, std::string addInfo){
    
    initEssentia();
    std::string defaultNote = "Params\n";
    float tmp;
    std::vector<float> tmpV;
    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* danceability     = factory.create("Danceability",
                                             "sampleRate", SAMPLERATE);
    
    danceability->input("signal").set(signal);
    danceability->output("danceability").set(tmp);
    danceability->output("dfa").set(tmpV);
    
    danceability->compute();
    
    dest[addInfo+"Danceability"] = AudioFeature(addInfo+"Danceability", tmp, defaultNote);
    dest[addInfo+"dfa"] = AudioFeature(addInfo+"dfa", tmpV, defaultNote);
    
    delete danceability;
//    essentia::shutdown();
}


/**
 * Source:https://essentia.upf.edu/reference/std_Entropy.html
This algorithm computes the Shannon entropy of an array. Entropy can be used to quantify the peakiness of a distribution. This has been used for voiced/unvoiced decision in automatic speech recognition.
Entropy cannot be computed neither on empty arrays nor arrays which contain negative values. In such cases, exceptions will be thrown.
 * @param array := input sample array (is allowed to have negative values)
 * @param dest := pointer to AudioFeature vector to add features
 * @param addInfo := additional Information which like (log, mel, bark, etc) it will be placed infront of the file name
 */
void calcEssEntropy(vector<float>& array, std::unordered_map<string, AudioFeature> & dest, std::string addInfo){
    
    initEssentia();
    std::string defaultNote = "Params\n confidenceThreshold";
    float tmp;
    
    for(int i = 0; i< array.size(); ++i){
        if(array[i]<0){
            array[i]*=-1;
        }
    }
    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* entropy     = factory.create("Entropy");
    
    entropy->input("array").set(array);
    entropy->output("entropy").set(tmp);
    
    entropy->compute();
    
    dest[addInfo+"EssEntropy"] = AudioFeature(addInfo+"EssEntropy", tmp, defaultNote);
            
    delete entropy;
//    essentia::shutdown();
}

/**
 * Source:https://essentia.upf.edu/reference/std_DistributionShape.html
 *       :https://essentia.upf.edu/reference/std_CentralMoments.html
This algorithm computes the spread (variance), skewness and kurtosis of an array given its central moments. The extracted features are good indicators of the shape of the distribution. For the required input see CentralMoments algorithm. The size of the input array must be at least 5. An exception will be thrown otherwise.
 * @param array := sample array or histogramm (mean spectrum)
 * @param dest := pointer to AudioFeature vector to add features
 * @param mode := mode of Distributionshape (pdf,sample) (pdf := probability density function)
 */
void calcDistributionShape(vector<float>& array, std::unordered_map<string, AudioFeature> & dest,SpectComplexExtractorParams & select, std::string mode, std::string addInfo){
    
    //if no feature should be extracted -> return
    if(select.empty) return;
    
    initEssentia();
    std::string defaultNote = "Params\n mode: " + mode;
//    std::cout << "am here " << std::to_string(array.size()) << std::endl;
    //compute central moments with probability density function (of histogram, mean spectrum)
    //https://www.themathdoctors.org/from-histograms-to-probability-distribution-functions/
    //there is only the case of a linear mean spectrum. therefor there is no need to divide each bin by its area
    //if mode is nether 'pdf' nor 'sample' interpret input array as sample array
    if(mode == "pdf"){
        float total = 0;
        for(int i = 0; i <= array.size(); ++i){
            total+= array[i];
        }
        for(int i = 0; i <= array.size(); ++i){
            array[i] /= (total + EPS);
        }
    }else {
        if(mode != "sample"){
            mode = "sample";
    }}
    
    AlgorithmFactory& factory = AlgorithmFactory::instance();

    Algorithm* centralMoments     = factory.create("CentralMoments",
                                                    "mode", mode,
                                                    "range", 1);
    
    Algorithm* distributionShape     = factory.create("DistributionShape");
    
    centralMoments->input("array").set(array);
    
    std::vector<float> tmpV;
    centralMoments->output("centralMoments").set(tmpV);
    
    float destSpread;
    float destSkewness;
    float destKurtosis;

    distributionShape->input("centralMoments").set(tmpV);
    distributionShape->output("spread").set(destSpread);
    distributionShape->output("skewness").set(destSkewness);
    distributionShape->output("kurtosis").set(destKurtosis);
    
    centralMoments->compute();
    distributionShape->compute();
    
    if(select.spread) dest[addInfo+"spread"] = AudioFeature(addInfo+"spread", destSpread, defaultNote);
    if(select.skewness) dest[addInfo+"skewness"] = AudioFeature(addInfo+"skewness", destSkewness, defaultNote);
    if(select.kurtosis) dest[addInfo+"kurtosis"] = AudioFeature(addInfo+"kurtosis", destKurtosis, defaultNote);
    
    
    delete distributionShape;
    delete centralMoments;
    
//    essentia::shutdown();
}


/**
 * Source: https://essentia.upf.edu/reference/std_Flatness.html
This algorithm computes the flatness of an array, which is defined as the ratio between the geometric mean and the arithmetic mean.

Flatness is undefined for empty input and negative values, therefore an exception is thrown in any both cases.
 * @param array := input sample array (is allowed to have negative values)
 * @param dest  := pointer to AudioFeature vector to add feature
 */
void calcFlatness(vector<float>& array, std::unordered_map<string, AudioFeature> & dest, std::string addInfo){
    
    initEssentia();
    std::string defaultNote = "Params\n ";
    float tmp;
    
    for(int i = 0; i< array.size(); ++i){
        if(array[i]<0){
            array[i]*=-1;
        }
    }
    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* flatness     = factory.create("Flatness");
    
    flatness->input("array").set(array);
    flatness->output("flatness").set(tmp);
    
    flatness->compute();
    dest[addInfo+"flatness"] = AudioFeature(addInfo+"flatness", tmp, defaultNote);
    
    delete flatness;
//    essentia::shutdown();
}

/**
 * Source:https://essentia.upf.edu/reference/std_Envelope.html
 This algorithm computes the envelope of a signal by applying a non-symmetric lowpass filter on a signal. 
 By default it rectifies the signal, but that is optional.
 * 
 * Source:https://essentia.upf.edu/reference/std_TCToTotal.html
This algorithm calculates the ratio of the temporal centroid to the total length of a signal envelope. 
This ratio shows how the sound is 'balanced'. Its value is close to 0 if most of the energy lies at the beginning of the sound 
(e.g. decrescendo or impulsive sounds), close to 0.5 if the sound is symetric (e.g. 'delta unvarying' sounds), 
and close to 1 if most of the energy lies at the end of the sound (e.g. crescendo sounds).

Please note that the TCToTotal ratio is not defined for a zero signal (a signal consisting of only zeros), 
nor it is defined for a signal of less than 2 elements.An exception is thrown if the given envelope's size is not larger than 1. 
And also if the integral of the input envelope is 0 (i.e. envelope is only zeros or if its sum is 0).
This algorithm is intended to be plugged after the Envelope algorithm
 * 
 * 
 * @param signal := signal of sample
 * @param dest := pointer to AudioFeature vector to add features
 * @param applyRectification := whether to apply rectification (envelope based on the absolute value of signal) : (bool ∈ {true, false}, default = true)
 * @param attackTime := the attack time of the first order lowpass in the attack phase [ms] :  (real ∈ [0, ∞), default = 10) 
 * @param releaseTime := the release time of the first order lowpass in the release phase [ms] :  (real ∈ [0, ∞), default = 1500)
 */
void calcTCToTotal(vector<float>& signal, std::unordered_map<string, AudioFeature> & dest, bool applyRectification, float attackTime, float releaseTime){
    initEssentia();
    std::string defaultNote = "Params\n attackTime: " + std::to_string(attackTime)
                            + " releaseTime: " + std::to_string(releaseTime)
                            + " applyRectification: "+std::to_string(applyRectification);
    std::vector<float> tmpEnv;
    float tmp;
    
    AlgorithmFactory& factory = AlgorithmFactory::instance();
    
    Algorithm* Envelope     = factory.create("Envelope",
                                            "applyRectification", applyRectification,
                                            "attackTime", attackTime,
                                            "releaseTime",releaseTime,
                                            "sampleRate", SAMPLERATE);

    Algorithm* TCToTotal     = factory.create("TCToTotal");
    
    Envelope->input("signal").set(signal);
    Envelope->output("signal").set(tmpEnv);
    
    TCToTotal->input("envelope").set(tmpEnv);
    TCToTotal->output("TCToTotal").set(tmp);
    
    Envelope->compute();
    TCToTotal->compute();
    
    dest["TCToTotal"] = AudioFeature("TCToTotal", tmp, defaultNote);

    
    delete Envelope;
    delete TCToTotal;
    freeVector(tmpEnv);
//    essentia::shutdown();
}

/**
 * Source:https://essentia.upf.edu/reference/std_EffectiveDuration.html
 * This algorithm computes the effective duration of an envelope signal. 
 * The effective duration is a measure of the time the signal is perceptually meaningful. 
 * This is approximated by the time the envelope is above or equal to a given threshold and is above the -90db noise floor. 
 * This measure allows to distinguish percussive sounds from sustained sounds but depends on the signal length. 
 * By default, this algorithm uses 40% of the envelope maximum as the threshold which is suited for short sounds. 
 * Note, that the 0% thresold corresponds to the duration of signal above -90db noise floor, 
 * while the 100% thresold corresponds to the number of times the envelope takes its maximum value.
 * 
 * @param signal := signal of sample
 * @param dest := pointer to AudioFeature vector to add features
 * @param thresholdRatio := the ratio of the envelope maximum to be used as the threshold :(real ∈ [0, 1], default = 0.4)
 */
void calcEffectiveDuration(vector<float>& signal, std::unordered_map<string, AudioFeature> & dest, float thresholdRatio){
    
    initEssentia();
    std::string defaultNote = "Params\n thresholdRatio: " + std::to_string(thresholdRatio);
    float tmp;
    
    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* effectiveDuration     = factory.create("EffectiveDuration",
                                                      "sampleRate", SAMPLERATE,
                                                      "thresholdRatio", thresholdRatio);
    
    effectiveDuration->input("signal").set(signal);
    effectiveDuration->output("effectiveDuration").set(tmp);
    
    effectiveDuration->compute();
    
    dest["EffectiveDuration"] = AudioFeature("EffectiveDuration", tmp, defaultNote);

    
    delete effectiveDuration;
//    essentia::shutdown();
}
/**
 * Source:https://essentia.upf.edu/reference/std_Centroid.html
This algorithm computes the centroid of an array. The centroid is normalized to a specified range. 
This algorithm can be used to compute spectral centroid or temporal centroid.
The spectral centroid is a measure that indicates where the "center of mass" of the spectrum is. 
Perceptually, it has a robust connection with the impression of "brightness" of a sound, and therefore is used to characterise musical timbre. 
It is calculated as the weighted mean of the frequencies present in the signal, with their magnitudes as the weights.
The temporal centroid is the point in time in a signal that is a temporal balancing point of the sound event energy. 
It can be computed from the envelope of the signal across audio samples [3]
(see Envelope algorithm) or over the RMS level of signal across frames [4] (see RMS algorithm).
 * 
Note: - For a spectral centroid [hz], frequency range should be equal to samplerate/2 
- For a temporal envelope centroid [s], range should be equal to (audio_size_in_samples-1) / samplerate 
- Exceptions are thrown when input array contains less than 2 elements.
 * 
 * @param array := either a spectrum, signal or temporal envelope
 * @param dest := pointer to a vector of AudioFeatures to add features
 * @param range := the range of the input array, used for normalizing the results : (real ∈ (0, ∞), default = 1)
 */
void calcSpectralCentroid(vector<float>& array, std::unordered_map<string, AudioFeature> & dest, float range, std::string addInfo){
    initEssentia();
    std::string defaultNote = "Params\n range: " + std::to_string(range)
                            + " mode: " +addInfo;
    
    float tmp;
    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* centroid     = factory.create("Centroid",
                                             "range", range);
    
    centroid->input("array").set(array);
    centroid->output("centroid").set(tmp);
    
    centroid->compute();
    dest[addInfo+"Centroid"] = AudioFeature(addInfo+"Centroid", tmp, defaultNote);
    
    delete centroid;
//    essentia::shutdown();

}

/**
 * Source: https://essentia.upf.edu/reference/std_TonalExtractor.html
This algorithm computes tonal features for an audio signal

_Selfnote: this algorithms computes multiple tonal features (or every tonal feature from essentia)
 There might not all features be implemented in this gatherer function (i.e. firstToSecondRelativeStrength of regular KEY extractor)
 * 
 * @param signal := signal of sample
 * @param dest := pointer to a map of AudioFeatures to add features
 * @param featureSet := Setting which features to extract of this huge collection
 * @param frameSize := the framesize for computing tonal features : (integer ∈ (0, ∞), default = 4096) 
 * @param hopSize := the hopSize for computing tonal features : (integer ∈ (0, ∞), default = 2048) 
 * @param tuningFrequency := the tuningFrequency  of the input signal : (real ∈ (0, ∞), default = 440)
 */
void calcTonalExtractor(vector<float>& signal, std::unordered_map<string, AudioFeature> & dest, TonalExtractorParams & featureSet, 
        int frameSize, int hopSize, float tuningFrequency){
    
    //if no feature should be extracted -> return
    if(featureSet.empty) return;
    
    initEssentia();
    
    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* tonalExtractor     = factory.create("TonalExtractor",
                                                      "frameSize", frameSize,
                                                      "hopSize", hopSize,
                                                      "tuningFrequency", tuningFrequency);
    
    tonalExtractor->input("signal").set(signal);
    
    std::string defaulNotes = "Calculation from tonalExtractor\n frameSize: " + std::to_string(frameSize)
                              + " hopSize: " + std::to_string(frameSize) +" tuningFrequency: " + std::to_string(tuningFrequency);
    
    
    float tmpCCR;
    tonalExtractor->output("chords_changes_rate").set(tmpCCR);

   
    std::vector<float> tmpCH;
    tonalExtractor->output("chords_histogram").set(tmpCH);

    
    std::string tmpK = "";
    tonalExtractor->output("chords_key").set(tmpK);

    float tmpCNR = 0;
    tonalExtractor->output("chords_number_rate").set(tmpCNR);

    std::vector<std::string> tmpP;
    tonalExtractor->output("chords_progression").set(tmpP);
    
    std::string tmpS= "";
    tonalExtractor->output("chords_scale").set(tmpS);

    std::vector<float> tmpSt;
    tonalExtractor->output("chords_strength").set(tmpSt);

    std::string tmpkk= "";
    tonalExtractor->output("key_key").set(tmpkk);

    std::string tmpks= "";
    tonalExtractor->output("key_scale").set(tmpks);
    
    float tmpkstr = 0;
    tonalExtractor->output("key_strength").set(tmpkstr);

    std::vector<std::vector<float>> tmpHPCP;
    std::vector<std::vector<float>> tmpHRF;

    tonalExtractor->output("hpcp").set(tmpHPCP);
    tonalExtractor->output("hpcp_highres").set(tmpHRF);

    tonalExtractor->compute();
    
    if(featureSet.key_strength){
        dest["key_strength"] = AudioFeature("key_strength", tmpkstr, defaulNotes);
    }
    if(featureSet.key_scale){
        dest["key_scale"] =  AudioFeature("key_scale", tmpks, defaulNotes);
    }
    if(featureSet.key_key){
        dest["key_key"] =  AudioFeature("key_key", tmpkk, defaulNotes);
    }
    if(featureSet.chords_strength){
        dest["chords_strength"] =  AudioFeature("chords_strength", tmpSt, defaulNotes);
    }
    if(featureSet.chords_scale){
        dest["chords_scale"] =  AudioFeature("chords_scale", tmpS, defaulNotes);
    }
    if(featureSet.chords_number_rate){
        dest["chords_number_rate"] =  AudioFeature("chords_number_rate", tmpCNR, defaulNotes);
    }
    if(featureSet.chords_key){
        dest["chords_key"] =  AudioFeature("chords_key", tmpK, defaulNotes);
    }
    if(featureSet.chords_progression){

        dest["chords_progression"] =  AudioFeature("chords_progression", tmpP, defaulNotes);
    }
    if(featureSet.chords_changes_rate){
        dest["chords_changes_rate"] = AudioFeature("chords_changes_rate", tmpCCR, defaulNotes);
    }
    if(featureSet.chords_histogram){
        dest["chords_histogram"] =  AudioFeature("chords_histogram", tmpCH, defaulNotes);
    }
    delete tonalExtractor;
//    essentia::shutdown();
}

/**
 * Source: https://essentia.upf.edu/reference/std_SpectralPeaks.html
 * 
This algorithm extracts peaks from a spectrum. It is important to note that the peak algorithm is independent of an input that is linear or in dB, 
so one has to adapt the threshold to fit with the type of data fed to it. 
The algorithm relies on PeakDetection algorithm which is run with parabolic interpolation [1]. 
The exactness of the peak-searching depends heavily on the windowing type. 
It gives best results with dB input, a blackman-harris 92dB window and interpolation set to true. According to [1], 
spectral peak frequencies tend to be about twice as accurate when dB magnitude is used rather than just linear magnitude. 
For further information about the peak detection, see the description of the PeakDetection algorithm.
 
It is recommended that the input "spectrum" be computed by the Spectrum algorithm. This algorithm uses PeakDetection. 
See documentation for possible exceptions and input requirements on input "spectrum".
 * 
 * 
 * https://essentia.upf.edu/reference/std_HPCP.html
 * 
Computes a Harmonic Pitch Class Profile (HPCP) from the spectral peaks of a signal. 
HPCP is a k*12 dimensional vector which represents the intensities of the twelve (k==1) semitone pitch classes (corresponsing to notes from A to G#),
or subdivisions of these (k>1).
Exceptions are thrown if "minFrequency", "bandSplitFrequency" and "maxFrequency" are not separated by at least 200Hz from each other, 
requiring that "maxFrequency" be greater than "bandSplitFrequency" and "bandSplitFrequency" be greater than "minFrequency". 
Other exceptions are thrown if input vectors have different size, if parameter "size" is not a positive non-zero multiple of 12 
or if "windowSize" is less than one hpcp bin (12/size).
 * 
 * 
 * https://essentia.upf.edu/reference/std_HighResolutionFeatures.html
 * 
 * This algorithm computes high-resolution chroma features from an HPCP vector.
 * The vector's size must be a multiple of 12 and it is recommended that it be larger than 120. 
 * In otherwords, the HPCP's resolution should be 10 Cents or more. The high-resolution features being computed are:
    -Equal-temperament deviation: a measure of the deviation of HPCP local maxima with respect to equal-tempered bins. This is done by: 
            a) Computing local maxima of HPCP vector b) Computing the deviations from equal-tempered (abs) bins and their average
    -Non-tempered energy ratio: the ratio betwen the energy on non-tempered bins and the total energy, computed from the HPCP average
    -Non-tempered peak energy ratio: the ratio betwen the energy on non tempered peaks and the total energy, computed from the HPCP average
 * 
 * 
 * @param filePath
 * @param dest  := vector<AudioFeature> destination of extracted (HighResolutionFeatures) 
 * @param magnitudeThreshold := (SpectralPeaks) peaks below this given threshold are not outputted (real ∈ (-∞, ∞), default = 0) 
 * @param maxFrequency := (SpectralPeaks) the maximum frequency of the range to evaluate [Hz]  (real ∈ (0, ∞), default = 5000)
 * @param maxPeaks := (SpectralPeaks) the maximum number of returned peaks (integer ∈ [1, ∞), default = 100)
 * @param size := the size of the output HPCP (must be a positive nonzero multiple of 12)  (integer ∈ [12, ∞), default = 12) -> Use 144 default for HRF
 * @param windowSize := the size, in semitones, of the window used for the weighting (real ∈ (0, 12], default = 1) :
 * @param maxPeaksHRF :=  (HighResolutionFeatures) maximum number of HPCP peaks to consider when calculating outputs (integer ∈ [1, ∞), default = 24)
 */
void calcHRFs(std::vector<float> &spectrum, std::unordered_map<string, AudioFeature>& dest, float magnitudeThreshold, float maxFrequency, int maxPeaks, int size, float windowSize,
            int maxPeaksHRF){
    
    if(spectrum.size() < 2){
        dest["equalTemperedDeviation"] = AudioFeature("equalTemperedDeviation", 0 ,"size to small");
        dest["nonTemperedEnergyRatio"] = AudioFeature("nonTemperedEnergyRatio", 0 ,"size to small");
        dest["nonTemperedPeaksEnergyRatio"] = AudioFeature("nonTemperedPeaksEnergyRatio", 0 ,"size to small");
        return;
    }
    initEssentia();
    
    AlgorithmFactory& factory = AlgorithmFactory::instance();
    
    Algorithm* spectralPeaks = factory.create("SpectralPeaks",
                                      "magnitudeThreshold", magnitudeThreshold,
                                      "maxFrequency",maxFrequency,
                                      "maxPeaks",maxPeaks);
    
    Algorithm* HPCP = factory.create("HPCP",
                                      "size", size,
                                      "windowSize",windowSize,
                                      "sampleRate",SAMPLERATE);
    
    Algorithm* HRF = factory.create("HighResolutionFeatures",
                                    "maxPeaks",maxPeaksHRF);
    
    spectralPeaks->input("spectrum").set(spectrum);
    
    std::vector<float> tmpFreq;
    std::vector<float> tmpMag;
    
    spectralPeaks->output("frequencies").set(tmpFreq);
    spectralPeaks->output("magnitudes").set(tmpMag);
    
    HPCP->input("frequencies").set(tmpFreq);
    HPCP->input("magnitudes").set(tmpMag);
    
    std::vector<float> tmpHPCP;
    HPCP->output("hpcp").set(tmpHPCP);
    

    HRF->input("hpcp").set(tmpHPCP);
    
    float equalTemperedDeviation;
    float nonTemperedEnergyRatio;
    float nonTemperedPeaksEnergyRatio;
    HRF->output("equalTemperedDeviation").set(equalTemperedDeviation);
    HRF->output("nonTemperedEnergyRatio").set(nonTemperedEnergyRatio);
    HRF->output("nonTemperedPeaksEnergyRatio").set(nonTemperedPeaksEnergyRatio);
    
    std::string defaultNote = "Params: \n magnitudeThreshold: "+ std::to_string(magnitudeThreshold)
                             + " maxFrequency: " + std::to_string(maxFrequency)
                            + " maxPeaks: " + std::to_string(maxPeaks)
                            + " size: " + std::to_string(size)
                            + " windowSize: " + std::to_string(windowSize)
                            + " maxPeaksHRF: " + std::to_string(maxPeaksHRF);
    
    spectralPeaks->compute();
    HPCP->compute();
    HRF->compute();
    
    dest["equalTemperedDeviation"] =  AudioFeature("equalTemperedDeviation", equalTemperedDeviation ,defaultNote);
    dest["nonTemperedEnergyRatio"] =  AudioFeature("nonTemperedEnergyRatio", nonTemperedEnergyRatio ,defaultNote);
    dest["nonTemperedPeaksEnergyRatio"] =  AudioFeature("nonTemperedPeaksEnergyRatio", nonTemperedPeaksEnergyRatio ,defaultNote);
    
    delete spectralPeaks;
    delete HPCP;
    delete HRF;
    freeVector(tmpFreq);
    freeVector(tmpMag);
    freeVector(tmpHPCP);
    
//    essentia::shutdown();
}

/**
 * Soruce https://essentia.upf.edu/reference/std_ZeroCrossingRate.html
 * 
    This algorithm computes the zero-crossing rate of an audio signal. 
    It is the number of sign changes between consecutive signal values divided by the total number of values. 
    Noisy signals tend to have higher zero-crossing rate. In order to avoid small variations around zero caused by noise, 
    a threshold around zero is given to consider a valid zerocrosing whenever the boundary is crossed.
    Empty input signals will raise an exception.
 * 
 * @param signal := signal of sample
 * @param dest := destination for a vector of AudioFeatures
 * @param threshold := the threshold which will be taken as the zero axis in both positive and negative sign (real ∈ [0, ∞], default = 0)
 */
void calcZeroCrossingRate(vector<float>& signal, std::unordered_map<string, AudioFeature>& dest, float threshold){
    initEssentia();

    float znr = 0.0;
    std::string defaulNote = "Calculation from ZeroCrossingRate\n threshold: " + std::to_string(threshold);
    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* zrc     = factory.create("ZeroCrossingRate",
                                             "threshold", threshold);
    
    zrc->input("signal").set(signal);
    zrc->output("zeroCrossingRate").set(znr);
    
    zrc->compute();
    dest["ZeroCrossingRate"] =  AudioFeature("ZeroCrossingRate", znr ,defaulNote);
    delete zrc;
//    essentia::shutdown();

}

void getMonoAudioEQLoud(string& filePath, vector<float>& dest, float replaygain){
    initEssentia();
    
    AlgorithmFactory& factory = AlgorithmFactory::instance();
    
    Algorithm* audio = factory.create("EqloudLoader",
                                      "filename", filePath,
                                      "sampleRate", SAMPLERATE,
                                      "replayGain", replaygain);
    audio->output("audio").set(dest);
    audio->compute();
    
    delete audio;
//    essentia::shutdown();
}


void getMonoAudio(string& filePath, vector<float>& dest){
    initEssentia();
    
    AlgorithmFactory& factory = AlgorithmFactory::instance();
    
    Algorithm* audio = factory.create("MonoLoader",
                                      "filename", filePath,
                                      "sampleRate", SAMPLERATE);
//      Algorithm* audio = factory.create("EqloudLoader",
//                                      "filename", filePath,
//                                      "sampleRate", SAMPLERATE);
    audio->output("audio").set(dest);
    audio->compute();
    
    delete audio;
//    essentia::shutdown();
}

/**
 * Source:https://essentia.upf.edu/reference/std_MelBands.html
This algorithm computes energy in mel bands of a spectrum. It applies a frequency-domain filterbank (MFCC FB-40, [1]), 
which consists of equal area triangular filters spaced according to the mel scale. 
The filterbank is normalized in such a way that the sum of coefficients for every filter equals one. 
It is recommended that the input "spectrum" be calculated by the Spectrum algorithm.
It is required that parameter "highMelFrequencyBound" not be larger than the Nyquist frequency, 
but must be larger than the parameter,"lowMelFrequencyBound". 
Also, The input spectrum must contain at least two elements. If any of these requirements are violated, an exception is thrown.
Note: an exception will be thrown in the case when the number of spectrum bins (FFT size) is insufficient to compute the specified number of mel bands: 
in such cases the start and end bin of a band can be the same bin or adjacent bins, which will result in zero energy when summing bins for that band.
Use zero padding to increase the number of spectrum bins in these cases.
 * 
 * Calculates the Meldband-Spectrogram. Therefore a signal will first be transformed into a spectrogram with
 * fixed window type, frame size, and hop size, the new default mel band amount is 96
 * @param signal := signal of a sample
 * @param log   := compute log-energies (log10 (1 + energy)) :  (bool ∈ {true, false}, default = true)
 * @param normalize := spectrum bin weights to use for each mel band: (string ∈ {unit_sum, unit_tri, unit_max}, default = unit_sum)
 *                      'unit_max' to make each mel band vertex equal to 1, 
 *                      'unit_sum' to make each mel band area equal to 1 summing the actual weights of spectrum bins, 
 *                      'unit_area' to make each triangle mel band area equal to 1 normalizing the weights of each triangle by its bandwidth
 * @param type      := 'power' to output squared units, 'magnitude' to keep it as the input :  (string ∈ {magnitude, power}, default = power
 * @return 
 */
vector<vector<float>> calcMelBands(vector<float> & signal, bool log, std::string normalize, std::string type){
    initEssentia();
    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* window     = factory.create("Windowing",
                                       "type", WINDOW_TYPE,
                                       "size", FRAMESIZE);

    Algorithm* fc = factory.create("FrameCutter",
                               "frameSize", FRAMESIZE,
                               "hopSize", HOPSIZE);

    Algorithm* fft        = factory.create("Spectrum",
                                       "size", FRAMESIZE);
    
    Algorithm* melBand    = factory.create("MelBands",
                                       "inputSize", FRAMESIZE/2 +1,
                                        "log", log,
                                        "numberBands",MELBAND,
                                        "normalize",normalize,
                                        "type",type);  
    fc->input("signal").set(signal);

    vector<vector<Real>> melFrames;
    
    vector<Real> frame;
    vector<Real> singSpec;
    vector<Real> singleMel;
    fc->output("frame").set(frame);
    //cutts signal into frames, applies windowing on frame,
    //calculates spectrum and than calculate mel bands
    //when there is no frame left: abort
    while (true) {
        fc->compute();
        if (frame.empty()){
            break;
        }else{
          window->input("frame").set(frame);
          window->output("frame").set(frame);
          window->compute();
          
          fft->input("frame").set(frame);
          fft->output("spectrum").set(singSpec);
          fft->compute();
          
          melBand->input("spectrum").set(singSpec);
          melBand->output("bands").set(singleMel);
          melBand->compute();
          melFrames.push_back(singleMel);
        }
    }
    
    delete fc;
    delete window;
    delete fft;
    delete melBand;
    freeVector(frame);
    freeVector(singSpec);
    freeVector(singleMel);
//    essentia::shutdown();
    
    return melFrames;
}


/**
 * Same as calcMelBands, but calculates from a spectrum
 * @param spectrum
 * @param log
 * @param normalize
 * @param type
 * @return 
 */
vector<float> calcMelBandsSingle(vector<float> & spectrum, bool log , std::string normalize, std::string type){
    
    
    initEssentia();
    vector<Real> mels;
    
    AlgorithmFactory& factory = AlgorithmFactory::instance();
    
    Algorithm* melBand    = factory.create("MelBands",
                                       "inputSize", FRAMESIZE/2 +1,
                                        "log", log,
                                        "numberBands",MELBAND,
                                        "normalize",normalize,
                                        "type",type);
    
    melBand->input("spectrum").set(spectrum);
    melBand->output("bands").set(mels);
    melBand->compute();
    
    delete melBand;
//    essentia::shutdown();
    
    return mels;
}


/**
 * Calculates a Spectrogram from a given signal
 * 
 * @param monoData := mono signal
 * @return         := 2D-Spectrogram
 */
vector<vector<float>> calcSpectrogram(vector<float>& monoData){
    initEssentia();


    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* window     = factory.create("Windowing",
                                       "type", WINDOW_TYPE,
                                       "size", FRAMESIZE+1);

    Algorithm* fc = factory.create("FrameCutter",
                               "frameSize", FRAMESIZE,
                               "hopSize", HOPSIZE);

    Algorithm* fft        = factory.create("Spectrum",
                                       "size", FRAMESIZE);                                                          
    fc->input("signal").set(monoData);

    vector<vector<Real>> frameS;

    vector<Real> frame;
    vector<Real> singSpec;
    fc->output("frame").set(frame);
    //cutts signal into frames, applies windowing on frame and calculates spectrum
    //when there is no frame left: abort
    while (true) {
        fc->compute();
        if (frame.empty()){
            break;
        }else{
          window->input("frame").set(frame);
          window->output("frame").set(frame);
          window->compute();
          fft->input("frame").set(frame);
          fft->output("spectrum").set(singSpec);
          fft->compute();
          frameS.push_back(singSpec);
        }
    }
    delete fc;
    delete window;
    delete fft;
    freeVector(frame);
    freeVector(singSpec);
//    essentia::shutdown();
    
    return frameS;
    
}
/**
 * Source:https://essentia.upf.edu/reference/std_LogSpectrum.html
This algorithm computes spectrum with logarithmically distributed frequency bins. This code is ported from NNLS Chroma [1, 2].
This algorithm also returns a local tuning that is retrieved for input frame and a global tuning that is updated with a moving average.dantly, 
otherwise an exception will be thrown. FrequencyBands is only defined for spectra, which size is greater than 1.
Note: As the algorithm uses moving averages that are updated every frame it should be reset before processing a new audio file. To do this call reset() (or configure())
 * 
 * This implementation returns the Log-Spectrogram 
 * @param spectrogram :=  the input spectrogram
 * @param output := defines the output, either logFreqSpectrum, meanTuning, or localTuning
 * @param binsPerSemitone :=     bins per semitone :(real ∈ (0, ∞), default = 3)
 * @param frameSize := the input frame size of the spectrum vector (integer ∈ (1, ∞), default = 1025)
 * @param rollOn := this removes low-frequency noise - useful in quiet recordingsrollOn (real ∈ [0, 5], default = 0)
 * @return  log frequency spectrogram or normalized mean tuning frequency
 */
vector<vector<float>> calcLogSpectrogram(vector<vector<float>>& spectrogram, std::string output, float binsPerSemitone, int frameSize, float rollOn){
    
    if(output != "logFreqSpectrum" && output != "meanTuning" && output != "localTuning"){
        std::cout << "Wrong input in calcLogSpecrogram" << std::endl;
        return vector<vector<float>>{{}};
    }
    
    initEssentia();

    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* logBand        = factory.create("LogSpectrum",
                                                "binsPerSemitone", binsPerSemitone,
                                                "frameSize", FRAMESIZE/2+1,
                                                "rollOn", rollOn,
                                                "sampleRate", SAMPLERATE);      
    
    

    vector<vector<Real>> frameS;
    vector<Real> bands;
    vector<Real> meanTuning;
    Real tuning;

    for(vector<float> & spectrum : spectrogram){
        
        logBand->input("spectrum").set(spectrum);
        logBand->output("logFreqSpectrum").set(bands);
        logBand->output("meanTuning").set(meanTuning);
        logBand->output("localTuning").set(tuning);
        logBand->compute();
        
        if(output == "logFreqSpectrum"){
            frameS.push_back(bands);
        }else if(output == "meanTuning"){
            frameS.push_back(meanTuning);
        }else if(output == "localTuning"){
            frameS.push_back(std::vector<Real>{tuning});
        }
    }
    
    delete logBand;
    freeVector(bands);
//    essentia::shutdown();
    
    return frameS;
    
}
/**
 * Source:https://essentia.upf.edu/reference/std_LogSpectrum.html
This algorithm computes spectrum with logarithmically distributed frequency bins. This code is ported from NNLS Chroma [1, 2].
This algorithm also returns a local tuning that is retrieved for input frame and a global tuning that is updated with a moving average.dantly, 
otherwise an exception will be thrown. FrequencyBands is only defined for spectra, which size is greater than 1.
Note: As the algorithm uses moving averages that are updated every frame it should be reset before processing a new audio file. To do this call reset() (or configure())
 * 
 * This implementation returns the Log-Spectrum 
 * @param spectrum :=  the input spectrum
 * @param output := defines the output, either logFreqSpectrum, meanTuning, or localTuning (default = logFreqSpectrum)
 * @param binsPerSemitone :=     bins per semitone :(real ∈ (0, ∞), default = 3)
 * @param frameSize := the input frame size of the spectrum vector (integer ∈ (1, ∞), default = 1025)
 * @param rollOn := this removes low-frequency noise - useful in quiet recordingsrollOn (real ∈ [0, 5], default = 0)
 * @return  log frequency spectrogram or normalized mean tuning frequency
 */
vector<float> calcLogSpectrum(vector<float>& spectrum, std::string output, float binsPerSemitone, int frameSize, float rollOn){
    
    if(output != "logFreqSpectrum" && output != "meanTuning" && output != "localTuning"){
        std::cout << "Wrong input in calcLogSpecrogram" << std::endl;
        return vector<float>();
    }
    
    initEssentia();

    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* logBand        = factory.create("LogSpectrum",
                                                "binsPerSemitone", binsPerSemitone,
                                                "frameSize", FRAMESIZE/2+1,
                                                "rollOn", rollOn,
                                                "sampleRate", SAMPLERATE);         
    
    vector<Real> bands;
    vector<Real> meanTuning;
    vector<Real> localTuning;
    Real tun;
    
    logBand->input("spectrum").set(spectrum);
    
    logBand->output("logFreqSpectrum").set(bands);
    logBand->output("meanTuning").set(meanTuning);
    logBand->output("localTuning").set(tun);
    logBand->compute();
    localTuning.push_back(tun);
  
    delete logBand;
//    essentia::shutdown();
    
    if(output != "meanTuning"){
        return meanTuning;
    }
    if(output != "localTuning"){
        return localTuning;
    }
    return bands;
}


/**
 * Source: https://essentia.upf.edu/reference/std_BarkBands.html
 * This algorithm computes energy in Bark bands of a spectrum. The band frequencies are: 
 * [0.0, 50.0, 100.0, 150.0, 200.0, 300.0, 400.0, 510.0, 630.0, 770.0, 920.0, 1080.0, 
 * 1270.0, 1480.0, 1720.0, 2000.0, 2320.0, 2700.0, 3150.0, 3700.0, 4400.0, 5300.0, 6400.0, 
 * 7700.0, 9500.0, 12000.0, 15500.0, 20500.0, 27000.0]. 
 * The first two Bark bands [0,100] and [100,200] have been split in half for better resolution 
 * (because of an observed better performance in beat detection). 
 * For each bark band the power-spectrum (mag-squared) is summed.
 * 
 * Information to Bark-Scale: https://ccrma.stanford.edu/~jos/bbt/Bark_Frequency_Scale.html
 * 
 * The default implementation of Frequency Bands are the default (27) Bark bands.

 * 
 * @param spectrum :=  the input spectrum
 * @param numberBands := the number of desired barkbands :  (integer ∈ [1, 28], default = 28)
 * @return := the energy of the bark bands
 */
std::vector<float> calcBarkBands(std::vector<float>& spectrum, int numberBands){
    
    initEssentia();
    vector<Real> bands;
    
    AlgorithmFactory& factory = AlgorithmFactory::instance();
    
    Algorithm* freqBands    = factory.create("BarkBands",
                                             "numberBands",numberBands,
                                             "sampleRate", SAMPLERATE);
    
    freqBands->input("spectrum").set(spectrum);
    freqBands->output("bands").set(bands);
    freqBands->compute();
    
    delete freqBands;
//    essentia::shutdown();
    
    return bands;
}
/**
 * Source: https://essentia.upf.edu/reference/std_BarkBands.html
 * This algorithm computes energy in Bark bands of a spectrum. The band frequencies are: 
 * [0.0, 50.0, 100.0, 150.0, 200.0, 300.0, 400.0, 510.0, 630.0, 770.0, 920.0, 1080.0, 
 * 1270.0, 1480.0, 1720.0, 2000.0, 2320.0, 2700.0, 3150.0, 3700.0, 4400.0, 5300.0, 6400.0, 
 * 7700.0, 9500.0, 12000.0, 15500.0, 20500.0, 27000.0]. 
 * The first two Bark bands [0,100] and [100,200] have been split in half for better resolution 
 * (because of an observed better performance in beat detection). 
 * For each bark band the power-spectrum (mag-squared) is summed.
 * 
 * Information to Bark-Scale: https://ccrma.stanford.edu/~jos/bbt/Bark_Frequency_Scale.html
 * 
 * The default implementation of Frequency Bands are the default (27) Bark bands.
 * 
 * This implementation returns the BarkBand-Spectrogram 
 * 
 * @param spectrum :=  the input spectrum
 * @param numberBands := the number of desired barkbands :  (integer ∈ [1, 28], default = 28)
 * @return := the energy of the bark bands
 */
std::vector<std::vector<float>> calcBarkBandSpectrogram(std::vector<std::vector<float>>& spectrogram, int numberBands){
    
    initEssentia();
    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* barkbands    = factory.create("BarkBands",
                                             "numberBands",numberBands,
                                             "sampleRate", SAMPLERATE);
    

    vector<vector<Real>> frameS;
    vector<Real> bands;
    for(vector<float> & spectrum : spectrogram){
        barkbands->input("spectrum").set(spectrum);
        barkbands->output("bands").set(bands);
        barkbands->compute();
        frameS.push_back(bands);
        
    }
    delete barkbands;
    freeVector(bands);
//    essentia::shutdown();
    
    return frameS;
}


/**
 * Source: //https://essentia.upf.edu/reference/std_TriangularBands.html
 * This algorithm computes energy in triangular frequency bands of a spectrum. 
 * The arbitrary number of overlapping bands can be specified. 
 * For each band the power-spectrum (mag-squared) is summed.

 * Parameter "frequencyBands" must contain at least two frequencies, they all must be positive and must be ordered ascentdantly, 
 * otherwise an exception will be thrown. TriangularBands is only defined for spectrum, which size is greater than 1.
 * 
 *  Interesting usage could be with chromatic scale -> getChromaticFreqs()
 * 
 * @param spectrum :=  the input spectrum
 * @param frequecnyBands := list of frequency ranges into which the spectrum is divided (these must be in ascending order and connot contain duplicates),
 *                          each triangle is build as x(i-1)=0, x(i)=1, x(i+1)=0 over i, the resulting number of bands is size of input array - 2
 * @param inputSize := the size of the spectrum  (default 1025)
 * @param log       := compute log-energies (log10 (1 + energy)  (default true)
 * @param normalize := spectrum bin weights to use for each triangular band:  (string ∈ {unit_sum, unit_tri, unit_max}, default = unit_sum)
 *                      'unit_max' to make each triangle vertex equal to 1, 
 *                      'unit_sum' to make each triangle area equal to 1 summing the actual weights of spectrum bins, 
 *                      'unit_area' to make each triangle area equal to 1 normalizing the weights of each triangle by its bandwidth
 * @param type      := use magnitude or power spectrum  (string ∈ {magnitude, power}, default = power)
 * @param weighting := type of weighting function for determining triangle area (string ∈ {linear, slaneyMel, htkMel}, default = linear)
 * @return := the energy of the bark bands
 */
std::vector<float> calcTriangularBands(     std::vector<float>& spectrum, std::vector<float> frequencyBands, int inputSize, 
                                            bool log, std::string normalize, std::string type, std::string weighting){
    
    if(inputSize == 0){
        inputSize = spectrum.size();
    }
    
    
    initEssentia();
    vector<Real> bands;
    
    AlgorithmFactory& factory = AlgorithmFactory::instance();
    
    Algorithm* freqBands    = factory.create("TriangularBands",
                                             "frequencyBands",frequencyBands,
                                             "sampleRate", SAMPLERATE,
                                             "inputSize", inputSize,
                                             "log", log,
                                             "normalize", normalize,
                                             "type", type, 
                                            "weighting", weighting);
    
    freqBands->input("spectrum").set(spectrum);
    freqBands->output("bands").set(bands);
    freqBands->compute();
    
    delete freqBands;
//    essentia::shutdown();
    
    return bands;
}
std::vector<std::vector<float>> calcTriangularBandSpectrogram(std::vector<std::vector<float>>& spectrogram, std::vector<float> frequencyBands, int inputSize, 
                                                    bool log, std::string normalize, std::string type, std::string weighting){
    
    if(inputSize == 0){
        inputSize = spectrogram[0].size();
    }
    
    initEssentia();


    AlgorithmFactory& factory = AlgorithmFactory::instance();
    
    Algorithm* freqBands    = factory.create("TriangularBands",
                                             "frequencyBands",frequencyBands,
                                             "sampleRate", SAMPLERATE,
                                             "inputSize", inputSize,
                                             "log", log,
                                             "normalize", normalize,
                                             "type", type, 
                                            "weighting", weighting);
    
    

    vector<vector<Real>> frameS;
    vector<Real> bands;
    for(vector<float> & spectrum : spectrogram){
        freqBands->input("spectrum").set(spectrum);
        freqBands->output("bands").set(bands);
        freqBands->compute();
        frameS.push_back(bands);
        
    }
    delete freqBands;
    freeVector(bands);
//    essentia::shutdown();
    
    return frameS;
    
}

/**
 * Calculates chromatic spectrum with triangular filter bank.
 * The frequencies for the filter bank is generated with getChromaticFreqs()..
 * @param spectrum := spectrum which should be converted
 * @param mode     := ("triangular", "rectangular"). Changes the filter type.
 * @return         := Chromatic Spectrum
 */
std::vector<float> calcChromaticSpectrum(std::vector<float>& spectrum, std::string mode){
    
    std::vector<float> chromoFilterFreqs;
    std::vector<float> chromoSpectrum;
    
//    std::vector<float> paddedSpectrum = spectrum;
    //zero padding of spectrogram
    //otherwise TriangularBandFilter throws exceptions -.-
//    paddedSpectrum.insert(paddedSpectrum.end(), {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0});
    
    if( mode == "triangular"){
        
        chromoFilterFreqs = getChromaticFreqs(-32,67,1,440,false, false);
        chromoSpectrum = calcTriangularBands(spectrum,chromoFilterFreqs, spectrum.size());
        
    }else if(mode == "rectangular") {
        
        chromoFilterFreqs  = getChromaticFreqs(-32,67,1,440,true, true);
        chromoSpectrum = calcFrequencyBands(spectrum,chromoFilterFreqs);
        
    }
    
    return chromoSpectrum;
    
}

std::vector<std::vector<float>> calcChromaticSpectrogram(std::vector<std::vector<float>>& spectrogram, std::string mode){
    
    std::vector<float> chromoFilterFreqs;
    
    std::vector<std::vector<float>> chromoSpectogram;
    if( mode == "triangular"){
        
        chromoFilterFreqs = getChromaticFreqs(-32,67,1,440,false, false);
        chromoSpectogram = calcTriangularBandSpectrogram( spectrogram, chromoFilterFreqs, spectrogram[0].size());
    }else if(mode == "rectangular"){
        
        chromoFilterFreqs  = getChromaticFreqs(-32,67,1,440,true, true);
        chromoSpectogram = calcFrequencyBandSpectrogram(spectrogram, chromoFilterFreqs);
    }
    
   return chromoSpectogram;
}


/**
 * Source:https://essentia.upf.edu/reference/std_FrequencyBands.html
This algorithm computes energy in rectangular frequency bands of a spectrum. The bands are non-overlapping. 
For each band the power-spectrum (mag-squared) is summed.
Parameter "frequencyBands" must contain at least 2 frequencies, they all must be positive and must be ordered ascentdantly, 
otherwise an exception will be thrown. FrequencyBands is only defined for spectra, which size is greater than 1.
 * 
 * This implementation returns the FrequencyBand-Spectrogram 
 * @param spectrogram :=  the input spectrogram
 * @param frequencyBands := list of frequency ranges in to which the spectrum is divided (these must be in ascending order and connot contain duplicates)
 * @return  the energy in each band
 */
vector<vector<float>> calcFrequencyBandSpectrogram(vector<vector<float>>& spectrogram, std::vector<float> frequencyBands){
    
    
    initEssentia();
    if(frequencyBands.empty()){
        frequencyBands = std::vector<float>{0, 50, 100, 150, 200, 300, 400, 510, 630, 770, 920, 1080, 1270, 1480, 
                                            1720, 2000, 2320, 2700, 3150, 3700, 4400, 5300, 6400, 7700, 9500, 
                                            12000, 15500, 20500, 27000};
    }
    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* freqBand        = factory.create("FrequencyBands",
                                                "frequencyBands", frequencyBands,
                                                "sampleRate", SAMPLERATE);      
    
    

    vector<vector<Real>> frameS;
    vector<Real> bands;
    for(vector<float> & spectrum : spectrogram){
        freqBand->input("spectrum").set(spectrum);
        freqBand->output("bands").set(bands);
        freqBand->compute();
        frameS.push_back(bands);
        
    }
    delete freqBand;
    freeVector(bands);
//    essentia::shutdown();
    
    return frameS;
    
}
/**
 * Source:https://essentia.upf.edu/reference/std_FrequencyBands.html
This algorithm computes energy in rectangular frequency bands of a spectrum. The bands are non-overlapping. 
For each band the power-spectrum (mag-squared) is summed.
Parameter "frequencyBands" must contain at least 2 frequencies, they all must be positive and must be ordered ascentdantly, 
otherwise an exception will be thrown. FrequencyBands is only defined for spectra, which size is greater than 1.
 * 
 * @param spectrum :=  the input spectrum (must be greater than size one)
 * @param frequencyBands := list of frequency ranges in to which the spectrum is divided (these must be in ascending order and connot contain duplicates)
 * @return  the energy in each band
 */
vector<float> calcFrequencyBands(vector<float> & spectrum, std::vector<float> frequencyBands){
    
    if(frequencyBands.empty()){
        frequencyBands = std::vector<float>{0, 50, 100, 150, 200, 300, 400, 510, 630, 770, 920, 1080, 1270, 1480, 
                                            1720, 2000, 2320, 2700, 3150, 3700, 4400, 5300, 6400, 7700, 9500, 
                                            12000, 15500, 20500, 27000};
    }
    
    initEssentia();
    vector<Real> bands;
    
    AlgorithmFactory& factory = AlgorithmFactory::instance();
    
    Algorithm* freqBands    = factory.create("FrequencyBands",
                                             "frequencyBands",frequencyBands,
                                             "sampleRate", SAMPLERATE);
    
    freqBands->input("spectrum").set(spectrum);
    freqBands->output("bands").set(bands);
    freqBands->compute();
    
    delete freqBands;
//    essentia::shutdown();
    
    return bands;
}

/**
 * Smaller Version of a spectrum. Essentia recommends, that a spectrum should be
 * a averaged version of a spectrogram. Therefor is no need to calculate it twice
 * @param spectrogram
 * @return 
 */
vector<float> calcMeanSpectrum(std::vector<std::vector<float>>& spectrogram){
    
    vector<float> dest(spectrogram[0].size(),0.0);
    for(int i=0; i<spectrogram.size(); ++i){
        for(int ii=0; ii<HOPSIZE; ++ii){
            dest[ii] += spectrogram[i][ii]/spectrogram.size();
        }
    }
    
    return dest;
}

/**
 * Stupid there is a single function for that!
 * @param monoData
 * @return 
 */
vector<float> calcMeanSpectrum(vector<float>& monoData){
    
    initEssentia();

    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* window     = factory.create("Windowing",
                                       "type", WINDOW_TYPE,
                                       "size", FRAMESIZE);

    Algorithm* fc = factory.create("FrameCutter",
                               "frameSize", FRAMESIZE,
                               "hopSize", HOPSIZE);

    Algorithm* fft        = factory.create("Spectrum",
                                       "size", FRAMESIZE);                                                          

    fc->input("signal").set(monoData);

    vector<vector<Real>> frameS;

    vector<Real> frame;
    vector<Real> singSpec;
    fc->output("frame").set(frame);
    //cutts signal into frames, applies windowing on frame and calculates spectrum
    //when there is no frame left: abort
    while (true) {
        fc->compute();
        if (frame.empty()){
            break;
        }else{
          window->input("frame").set(frame);
          window->output("frame").set(frame);
          window->compute();
          fft->input("frame").set(frame);
          fft->output("spectrum").set(singSpec);
          fft->compute();
          frameS.push_back(singSpec);
        }
    }
    vector<float> dest(frameS[0].size(),0.0);
    for(int i=0; i<frameS.size(); ++i){
        for(int ii=0; ii<HOPSIZE; ++ii){
            dest[ii] += frameS[i][ii]/frameS.size();
        }
    }
    delete fc;
    delete window;
    delete fft;
    freeVector(frame);
    freeVector(frameS);
    freeVector(singSpec);
    
//    essentia::shutdown();
    
    return dest;
}

void calcSpectralCentroid(vector<float>& spectrum, float& dest){
    initEssentia();


    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* centroid     = factory.create("Centroid",
                                             "range", SAMPLERATE/2);
    
    centroid->input("array").set(spectrum);
    centroid->output("centroid").set(dest);
    
    centroid->compute();
    delete centroid;
//    essentia::shutdown();

}





void calcZeroCrossingRate(vector<float>& monoData, float& dest){
    
    initEssentia();

    AlgorithmFactory& factory = AlgorithmFactory::instance();


    Algorithm* zrc     = factory.create("ZeroCrossingRate",
                                             "threshold", 0);
    
    zrc->input("signal").set(monoData);
    zrc->output("zeroCrossingRate").set(dest);
    
    zrc->compute();
    dest *= SAMPLERATE/1024;
    delete zrc;
//    essentia::shutdown();

}


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
 * @param end               := highest exponent in formula: refNote * 2^(i/12)  (loops includes this number) (default 107 )
 * @param step              := step size ( i.e.: 1, 0.5, 0.33, 0.75, 2, ...) (default 1 )
 * @param refNote           := the reference note. (default 440 Hz)  
 * @param interpol_50_cents := interpolate 50 cents steps with average between two notes (note[i]+note[i+1])/2   (default true )
 * @param only_50_cents     := only save 50 cents steps (upper bound of note) this can be used for rectangular filter)  (default false)
 * @return 
 */
std::vector<float> getChromaticFreqs(float begin, float end, float step, float refNote, bool interpol_50_cents, bool only_50_cents){
    
    std::vector<float> chromFreqs;

    //
    if(interpol_50_cents){
        
        for(float i = begin; i < end; i += step){
            float note1 = refNote * std::pow(2.0,(i/12.0));
            float note2 = refNote * std::pow(2.0,((i+step)/12.0));
            if(only_50_cents == false){
                chromFreqs.push_back(note1);
            }
            chromFreqs.push_back(note1+ ((note2-note1)/2.0));
            
        }
     }else{
        for(float i = begin; i <= end; i+=step){
            chromFreqs.push_back(refNote * std::pow(2,(i/12.0)));  
        }
    }
    return chromFreqs;
}