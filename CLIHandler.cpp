/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "stdheader.h"

namespace fs = std::filesystem;

//static PARAMS for this application
APP_PARAMS FLOW_PARMS;



const std::set<string> possibleFeaturesSet{"AutoCorrelation_sig","Bark28_AutoCorrelation","Bark28_Centroid","Bark28_EssEntropy","Bark28_RollOff","Bark28_SpectralComplex","Bark28_SpectralFlux","Bark28_StrongPeak","Bark28_flatness","Bark28_kurtosis","Bark28_skewness","Bark28_spread","EffectiveDuration","EffectiveToStdDuration","Har-Bark28_-FACM_1","Har-Bark28_-FACM_10","Har-Bark28_-FACM_11","Har-Bark28_-FACM_12","Har-Bark28_-FACM_2","Har-Bark28_-FACM_3","Har-Bark28_-FACM_4","Har-Bark28_-FACM_5","Har-Bark28_-FACM_6","Har-Bark28_-FACM_7","Har-Bark28_-FACM_8","Har-Bark28_-FACM_9","Har-Bark28_-GLCM_1","Har-Bark28_-GLCM_10","Har-Bark28_-GLCM_11","Har-Bark28_-GLCM_12","Har-Bark28_-GLCM_2","Har-Bark28_-GLCM_3","Har-Bark28_-GLCM_4","Har-Bark28_-GLCM_5","Har-Bark28_-GLCM_6","Har-Bark28_-GLCM_7","Har-Bark28_-GLCM_8","Har-Bark28_-GLCM_9","Har-Log_-FACM_1","Har-Log_-FACM_10","Har-Log_-FACM_11","Har-Log_-FACM_12","Har-Log_-FACM_2","Har-Log_-FACM_3","Har-Log_-FACM_4","Har-Log_-FACM_5","Har-Log_-FACM_6","Har-Log_-FACM_7","Har-Log_-FACM_8","Har-Log_-FACM_9","Har-Log_-GLCM_1","Har-Log_-GLCM_10","Har-Log_-GLCM_11","Har-Log_-GLCM_12","Har-Log_-GLCM_2","Har-Log_-GLCM_3","Har-Log_-GLCM_4","Har-Log_-GLCM_5","Har-Log_-GLCM_6","Har-Log_-GLCM_7","Har-Log_-GLCM_8","Har-Log_-GLCM_9","Har-Mel96_-FACM_1","Har-Mel96_-FACM_10","Har-Mel96_-FACM_11","Har-Mel96_-FACM_12","Har-Mel96_-FACM_2","Har-Mel96_-FACM_3","Har-Mel96_-FACM_4","Har-Mel96_-FACM_5","Har-Mel96_-FACM_6","Har-Mel96_-FACM_7","Har-Mel96_-FACM_8","Har-Mel96_-FACM_9","Har-Mel96_-GLCM_1","Har-Mel96_-GLCM_10","Har-Mel96_-GLCM_11","Har-Mel96_-GLCM_12","Har-Mel96_-GLCM_2","Har-Mel96_-GLCM_3","Har-Mel96_-GLCM_4","Har-Mel96_-GLCM_5","Har-Mel96_-GLCM_6","Har-Mel96_-GLCM_7","Har-Mel96_-GLCM_8","Har-Mel96_-GLCM_9","Har-RecChr_-FACM_1","Har-RecChr_-FACM_10","Har-RecChr_-FACM_11","Har-RecChr_-FACM_12","Har-RecChr_-FACM_2","Har-RecChr_-FACM_3","Har-RecChr_-FACM_4","Har-RecChr_-FACM_5","Har-RecChr_-FACM_6","Har-RecChr_-FACM_7","Har-RecChr_-FACM_8","Har-RecChr_-FACM_9","Har-RecChr_-GLCM_1","Har-RecChr_-GLCM_10","Har-RecChr_-GLCM_11","Har-RecChr_-GLCM_12","Har-RecChr_-GLCM_2","Har-RecChr_-GLCM_3","Har-RecChr_-GLCM_4","Har-RecChr_-GLCM_5","Har-RecChr_-GLCM_6","Har-RecChr_-GLCM_7","Har-RecChr_-GLCM_8","Har-RecChr_-GLCM_9","Har-TriChr_-FACM_1","Har-TriChr_-FACM_10","Har-TriChr_-FACM_11","Har-TriChr_-FACM_12","Har-TriChr_-FACM_2","Har-TriChr_-FACM_3","Har-TriChr_-FACM_4","Har-TriChr_-FACM_5","Har-TriChr_-FACM_6","Har-TriChr_-FACM_7","Har-TriChr_-FACM_8","Har-TriChr_-FACM_9","Har-TriChr_-GLCM_1","Har-TriChr_-GLCM_10","Har-TriChr_-GLCM_11","Har-TriChr_-GLCM_12","Har-TriChr_-GLCM_2","Har-TriChr_-GLCM_3","Har-TriChr_-GLCM_4","Har-TriChr_-GLCM_5","Har-TriChr_-GLCM_6","Har-TriChr_-GLCM_7","Har-TriChr_-GLCM_8","Har-TriChr_-GLCM_9","Lin_AutoCorrelation","Lin_Centroid","Lin_EssEntropy","Lin_RollOff","Lin_SpectralComplex","Lin_SpectralFlux","Lin_StrongPeak","Lin_flatness","Lin_kurtosis","Lin_skewness","Lin_spread","Log_AutoCorrelation","Log_Centroid","Log_EssEntropy","Log_RollOff","Log_SpectralComplex","Log_SpectralFlux","Log_StrongPeak","Log_flatness","Log_kurtosis","Log_skewness","Log_spread","MFCC-1","MFCC-10","MFCC-11","MFCC-12","MFCC-13","MFCC-2","MFCC-3","MFCC-4","MFCC-5","MFCC-6","MFCC-7","MFCC-8","MFCC-9","Mel96_AutoCorrelation","Mel96_Centroid","Mel96_EssEntropy","Mel96_RollOff","Mel96_SpectralComplex","Mel96_SpectralFlux","Mel96_StrongPeak","Mel96_flatness","Mel96_kurtosis","Mel96_skewness","Mel96_spread","RecChr_AutoCorrelation","RecChr_Centroid","RecChr_EssEntropy","RecChr_RollOff","RecChr_SpectralComplex","RecChr_SpectralFlux","RecChr_StrongPeak","RecChr_flatness","RecChr_kurtosis","RecChr_skewness","RecChr_spread","Rhythm13-bpm","Rhythm13-confidence","TCToTotal","TriChr_AutoCorrelation","TriChr_Centroid","TriChr_EssEntropy","TriChr_RollOff","TriChr_SpectralComplex","TriChr_SpectralFlux","TriChr_StrongPeak","TriChr_flatness","TriChr_kurtosis","TriChr_skewness","TriChr_spread","ZeroCrossingRate","chords_changes_rate","chords_key","chords_number_rate","chords_scale","key_key","key_scale","key_strength","sample_length"};
const std::vector<string> possibleFeaturesVector{"AutoCorrelation_sig","Bark28_AutoCorrelation","Bark28_Centroid","Bark28_EssEntropy","Bark28_RollOff","Bark28_SpectralComplex","Bark28_SpectralFlux","Bark28_StrongPeak","Bark28_flatness","Bark28_kurtosis","Bark28_skewness","Bark28_spread","EffectiveDuration","EffectiveToStdDuration","Har-Bark28_-FACM_1","Har-Bark28_-FACM_10","Har-Bark28_-FACM_11","Har-Bark28_-FACM_12","Har-Bark28_-FACM_2","Har-Bark28_-FACM_3","Har-Bark28_-FACM_4","Har-Bark28_-FACM_5","Har-Bark28_-FACM_6","Har-Bark28_-FACM_7","Har-Bark28_-FACM_8","Har-Bark28_-FACM_9","Har-Bark28_-GLCM_1","Har-Bark28_-GLCM_10","Har-Bark28_-GLCM_11","Har-Bark28_-GLCM_12","Har-Bark28_-GLCM_2","Har-Bark28_-GLCM_3","Har-Bark28_-GLCM_4","Har-Bark28_-GLCM_5","Har-Bark28_-GLCM_6","Har-Bark28_-GLCM_7","Har-Bark28_-GLCM_8","Har-Bark28_-GLCM_9","Har-Log_-FACM_1","Har-Log_-FACM_10","Har-Log_-FACM_11","Har-Log_-FACM_12","Har-Log_-FACM_2","Har-Log_-FACM_3","Har-Log_-FACM_4","Har-Log_-FACM_5","Har-Log_-FACM_6","Har-Log_-FACM_7","Har-Log_-FACM_8","Har-Log_-FACM_9","Har-Log_-GLCM_1","Har-Log_-GLCM_10","Har-Log_-GLCM_11","Har-Log_-GLCM_12","Har-Log_-GLCM_2","Har-Log_-GLCM_3","Har-Log_-GLCM_4","Har-Log_-GLCM_5","Har-Log_-GLCM_6","Har-Log_-GLCM_7","Har-Log_-GLCM_8","Har-Log_-GLCM_9","Har-Mel96_-FACM_1","Har-Mel96_-FACM_10","Har-Mel96_-FACM_11","Har-Mel96_-FACM_12","Har-Mel96_-FACM_2","Har-Mel96_-FACM_3","Har-Mel96_-FACM_4","Har-Mel96_-FACM_5","Har-Mel96_-FACM_6","Har-Mel96_-FACM_7","Har-Mel96_-FACM_8","Har-Mel96_-FACM_9","Har-Mel96_-GLCM_1","Har-Mel96_-GLCM_10","Har-Mel96_-GLCM_11","Har-Mel96_-GLCM_12","Har-Mel96_-GLCM_2","Har-Mel96_-GLCM_3","Har-Mel96_-GLCM_4","Har-Mel96_-GLCM_5","Har-Mel96_-GLCM_6","Har-Mel96_-GLCM_7","Har-Mel96_-GLCM_8","Har-Mel96_-GLCM_9","Har-RecChr_-FACM_1","Har-RecChr_-FACM_10","Har-RecChr_-FACM_11","Har-RecChr_-FACM_12","Har-RecChr_-FACM_2","Har-RecChr_-FACM_3","Har-RecChr_-FACM_4","Har-RecChr_-FACM_5","Har-RecChr_-FACM_6","Har-RecChr_-FACM_7","Har-RecChr_-FACM_8","Har-RecChr_-FACM_9","Har-RecChr_-GLCM_1","Har-RecChr_-GLCM_10","Har-RecChr_-GLCM_11","Har-RecChr_-GLCM_12","Har-RecChr_-GLCM_2","Har-RecChr_-GLCM_3","Har-RecChr_-GLCM_4","Har-RecChr_-GLCM_5","Har-RecChr_-GLCM_6","Har-RecChr_-GLCM_7","Har-RecChr_-GLCM_8","Har-RecChr_-GLCM_9","Har-TriChr_-FACM_1","Har-TriChr_-FACM_10","Har-TriChr_-FACM_11","Har-TriChr_-FACM_12","Har-TriChr_-FACM_2","Har-TriChr_-FACM_3","Har-TriChr_-FACM_4","Har-TriChr_-FACM_5","Har-TriChr_-FACM_6","Har-TriChr_-FACM_7","Har-TriChr_-FACM_8","Har-TriChr_-FACM_9","Har-TriChr_-GLCM_1","Har-TriChr_-GLCM_10","Har-TriChr_-GLCM_11","Har-TriChr_-GLCM_12","Har-TriChr_-GLCM_2","Har-TriChr_-GLCM_3","Har-TriChr_-GLCM_4","Har-TriChr_-GLCM_5","Har-TriChr_-GLCM_6","Har-TriChr_-GLCM_7","Har-TriChr_-GLCM_8","Har-TriChr_-GLCM_9","Lin_AutoCorrelation","Lin_Centroid","Lin_EssEntropy","Lin_RollOff","Lin_SpectralComplex","Lin_SpectralFlux","Lin_StrongPeak","Lin_flatness","Lin_kurtosis","Lin_skewness","Lin_spread","Log_AutoCorrelation","Log_Centroid","Log_EssEntropy","Log_RollOff","Log_SpectralComplex","Log_SpectralFlux","Log_StrongPeak","Log_flatness","Log_kurtosis","Log_skewness","Log_spread","MFCC-1","MFCC-10","MFCC-11","MFCC-12","MFCC-13","MFCC-2","MFCC-3","MFCC-4","MFCC-5","MFCC-6","MFCC-7","MFCC-8","MFCC-9","Mel96_AutoCorrelation","Mel96_Centroid","Mel96_EssEntropy","Mel96_RollOff","Mel96_SpectralComplex","Mel96_SpectralFlux","Mel96_StrongPeak","Mel96_flatness","Mel96_kurtosis","Mel96_skewness","Mel96_spread","RecChr_AutoCorrelation","RecChr_Centroid","RecChr_EssEntropy","RecChr_RollOff","RecChr_SpectralComplex","RecChr_SpectralFlux","RecChr_StrongPeak","RecChr_flatness","RecChr_kurtosis","RecChr_skewness","RecChr_spread","Rhythm13-bpm","Rhythm13-confidence","TCToTotal","TriChr_AutoCorrelation","TriChr_Centroid","TriChr_EssEntropy","TriChr_RollOff","TriChr_SpectralComplex","TriChr_SpectralFlux","TriChr_StrongPeak","TriChr_flatness","TriChr_kurtosis","TriChr_skewness","TriChr_spread","ZeroCrossingRate","chords_changes_rate","chords_key","chords_number_rate","chords_scale","key_key","key_scale","key_strength","sample_length"};

void printFeatureListCLI(){
    std::cout << "AutoCorrelation_sig       0.225969\nBark28_AutoCorrelation    0.280858\nBark28_Centroid           0.458489\nBark28_EssEntropy         0.008534\nBark28_RollOff                 NaN\nBark28_SpectralComplex    0.046394\nBark28_SpectralFlux       0.092329\nBark28_StrongPeak         0.113710\nBark28_flatness                NaN\nBark28_kurtosis           0.149821\nBark28_skewness           0.117950\nBark28_spread             0.074125\nEffectiveDuration         0.024715\nEffectiveToStdDuration    0.123251\nHar-Bark28_-FACM_1        0.112187\nHar-Bark28_-FACM_10       0.417385\nHar-Bark28_-FACM_11       0.010623\nHar-Bark28_-FACM_12       0.344520\nHar-Bark28_-FACM_2        0.006807\nHar-Bark28_-FACM_3        0.048864\nHar-Bark28_-FACM_4        0.241229\nHar-Bark28_-FACM_5        0.021386\nHar-Bark28_-FACM_6        0.123680\nHar-Bark28_-FACM_7        0.100068\nHar-Bark28_-FACM_8        0.041968\nHar-Bark28_-FACM_9        0.086810\nHar-Bark28_-GLCM_1        0.100060\nHar-Bark28_-GLCM_10       0.065160\nHar-Bark28_-GLCM_11       0.062871\nHar-Bark28_-GLCM_12       0.096908\nHar-Bark28_-GLCM_2        0.047513\nHar-Bark28_-GLCM_3        0.075654\nHar-Bark28_-GLCM_4        0.045570\nHar-Bark28_-GLCM_5        0.096893\nHar-Bark28_-GLCM_6             NaN\nHar-Bark28_-GLCM_7        0.170336\nHar-Bark28_-GLCM_8        0.073585\nHar-Bark28_-GLCM_9        0.171924\nHar-Log_-FACM_1           0.189459\nHar-Log_-FACM_10          0.533862\nHar-Log_-FACM_11          0.153283\nHar-Log_-FACM_12          0.481785\nHar-Log_-FACM_2           0.073067\nHar-Log_-FACM_3           0.208580\nHar-Log_-FACM_4           0.170098\nHar-Log_-FACM_5           0.104191\nHar-Log_-FACM_6           0.160948\nHar-Log_-FACM_7           0.316935\nHar-Log_-FACM_8           0.257569\nHar-Log_-FACM_9           0.318352\nHar-Log_-GLCM_1           0.054978\nHar-Log_-GLCM_10          0.197968\nHar-Log_-GLCM_11          0.084887\nHar-Log_-GLCM_12          0.240047\nHar-Log_-GLCM_2           0.087747\nHar-Log_-GLCM_3           0.056306\nHar-Log_-GLCM_4           0.162597\nHar-Log_-GLCM_5           0.056897\nHar-Log_-GLCM_6                NaN\nHar-Log_-GLCM_7           0.038497\nHar-Log_-GLCM_8           0.034627\nHar-Log_-GLCM_9           0.039246\nHar-Mel96_-FACM_1         0.071543\nHar-Mel96_-FACM_10        0.373660\nHar-Mel96_-FACM_11        0.136723\nHar-Mel96_-FACM_12        0.285412\nHar-Mel96_-FACM_2         0.103704\nHar-Mel96_-FACM_3         0.023076\nHar-Mel96_-FACM_4         0.189808\nHar-Mel96_-FACM_5         0.030479\nHar-Mel96_-FACM_6         0.103244\nHar-Mel96_-FACM_7         0.203140\nHar-Mel96_-FACM_8         0.162615\nHar-Mel96_-FACM_9         0.197928\nHar-Mel96_-GLCM_1         0.122663\nHar-Mel96_-GLCM_10        0.100974\nHar-Mel96_-GLCM_11        0.090060\nHar-Mel96_-GLCM_12        0.122049\nHar-Mel96_-GLCM_2         0.078717\nHar-Mel96_-GLCM_3         0.110556\nHar-Mel96_-GLCM_4         0.012723\nHar-Mel96_-GLCM_5         0.145511\nHar-Mel96_-GLCM_6              NaN\nHar-Mel96_-GLCM_7         0.167895\nHar-Mel96_-GLCM_8         0.098558\nHar-Mel96_-GLCM_9         0.168106\nHar-RecChr_-FACM_1        0.191916\nHar-RecChr_-FACM_10       0.447002\nHar-RecChr_-FACM_11       0.173571\nHar-RecChr_-FACM_12       0.384273\nHar-RecChr_-FACM_2        0.172934\nHar-RecChr_-FACM_3        0.206050\nHar-RecChr_-FACM_4        0.191801\nHar-RecChr_-FACM_5        0.011947\nHar-RecChr_-FACM_6        0.072274\nHar-RecChr_-FACM_7        0.022439\nHar-RecChr_-FACM_8        0.158732\nHar-RecChr_-FACM_9        0.018382\nHar-RecChr_-GLCM_1        0.000199\nHar-RecChr_-GLCM_10       0.032988\nHar-RecChr_-GLCM_11       0.025014\nHar-RecChr_-GLCM_12       0.000835\nHar-RecChr_-GLCM_2        0.041650\nHar-RecChr_-GLCM_3        0.023890\nHar-RecChr_-GLCM_4        0.073466\nHar-RecChr_-GLCM_5        0.073707\nHar-RecChr_-GLCM_6             NaN\nHar-RecChr_-GLCM_7        0.031179\nHar-RecChr_-GLCM_8        0.034866\nHar-RecChr_-GLCM_9        0.032806\nHar-TriChr_-FACM_1        0.183473\nHar-TriChr_-FACM_10       0.441772\nHar-TriChr_-FACM_11       0.112018\nHar-TriChr_-FACM_12       0.352475\nHar-TriChr_-FACM_2        0.167851\nHar-TriChr_-FACM_3        0.228104\nHar-TriChr_-FACM_4        0.199415\nHar-TriChr_-FACM_5        0.030257\nHar-TriChr_-FACM_6        0.100374\nHar-TriChr_-FACM_7        0.120472\nHar-TriChr_-FACM_8        0.066179\nHar-TriChr_-FACM_9        0.126883\nHar-TriChr_-GLCM_1        0.001659\nHar-TriChr_-GLCM_10       0.067388\nHar-TriChr_-GLCM_11       0.048124\nHar-TriChr_-GLCM_12       0.084595\nHar-TriChr_-GLCM_2        0.053899\nHar-TriChr_-GLCM_3        0.003122\nHar-TriChr_-GLCM_4        0.072398\nHar-TriChr_-GLCM_5        0.025945\nHar-TriChr_-GLCM_6             NaN\nHar-TriChr_-GLCM_7        0.082767\nHar-TriChr_-GLCM_8        0.038318\nHar-TriChr_-GLCM_9        0.083413\nLin_AutoCorrelation       0.013097\nLin_Centroid              0.446877\nLin_EssEntropy            0.329154\nLin_RollOff                    NaN\nLin_SpectralComplex       0.027054\nLin_SpectralFlux          0.030345\nLin_StrongPeak            0.155451\nLin_flatness                   NaN\nLin_kurtosis              0.120456\nLin_skewness              0.176127\nLin_spread                0.058533\nLog_AutoCorrelation       0.009902\nLog_Centroid              0.287973\nLog_EssEntropy            0.079473\nLog_RollOff                    NaN\nLog_SpectralComplex       0.244997\nLog_SpectralFlux          0.285436\nLog_StrongPeak                 NaN\nLog_flatness              0.079650\nLog_kurtosis              0.021600\nLog_skewness              0.137679\nLog_spread                0.030895\nMFCC-1                    0.031778\nMFCC-10                   0.218863\nMFCC-11                   0.337367\nMFCC-12                   0.067975\nMFCC-13                   0.141987\nMFCC-2                    0.101751\nMFCC-3                    0.474201\nMFCC-4                    0.421036\nMFCC-5                    0.127371\nMFCC-6                    0.160609\nMFCC-7                    0.408978\nMFCC-8                    0.349315\nMFCC-9                    0.038528\nMel96_AutoCorrelation     0.200944\nMel96_Centroid            0.325749\nMel96_EssEntropy          0.055495\nMel96_RollOff                  NaN\nMel96_SpectralComplex     0.058039\nMel96_SpectralFlux        0.052546\nMel96_StrongPeak          0.111866\nMel96_flatness                 NaN\nMel96_kurtosis            0.000778\nMel96_skewness            0.055045\nMel96_spread              0.056840\nRecChr_AutoCorrelation    0.181228\nRecChr_Centroid           0.481289\nRecChr_EssEntropy         0.136699\nRecChr_RollOff                 NaN\nRecChr_SpectralComplex    0.021411\nRecChr_SpectralFlux       0.108608\nRecChr_StrongPeak         0.115116\nRecChr_flatness                NaN\nRecChr_kurtosis           0.209882\nRecChr_skewness           0.208357\nRecChr_spread             0.075093\nRhythm13-bpm              0.020154\nRhythm13-confidence       0.050093\nTCToTotal                 0.191565\nTriChr_AutoCorrelation    0.324688\nTriChr_Centroid           0.365617\nTriChr_EssEntropy         0.215391\nTriChr_RollOff                 NaN\nTriChr_SpectralComplex    0.046383\nTriChr_SpectralFlux       0.015333\nTriChr_StrongPeak         0.136863\nTriChr_flatness                NaN\nTriChr_kurtosis           0.242005\nTriChr_skewness           0.199664\nTriChr_spread             0.046785\nZeroCrossingRate          0.355586\nchords_changes_rate       0.017986\nchords_key                0.010182\nchords_number_rate        0.182013\nchords_scale              0.039794\ngt                        1.000000\nkey_key                   0.033257\nkey_scale                 0.014278\nkey_strength              0.100440\nsample_length             0.039492";
}

struct PossibleFeatureValidator : public CLI::Validator {
    PossibleFeatureValidator() {
        name_ = "POSSIBLE";
        func_ = [](const std::string &str) {
            std::cout << str << std::endl;
            std::set<string>::iterator it = possibleFeaturesSet.find(str);
            if (it != possibleFeaturesSet.end()) {
                return str;
            } else
                return str + " is not a possible feature to chose from";
        };
    }
};
const static PossibleFeatureValidator PossibleFeature;

/**
 * handles Command line input for this application
 * @param argc  := number of arguments
 * @param argv  := arguments
 * @param params := reference to static PARAMS to save all arguments
 * @return (0 := abort, 1 := successful)
 */
int handleCommdLineInput(int argc, char** argv, APP_PARAMS & params){
    //TODO: add verbose
    //TODO: format CLI-output (this looks awful)
    CLI::App app("Sample Organizer - Reorganize Audio Samples by Timbre Similarity");
    
    app.set_help_all_flag("--help-all", "Expand all help");
    
    params.libraryPath = "";   //path of sample library
    params.clusters = 40; //amount of clusters to regroup sample library
    params.nUl  = -1; // Some arbitrary upper limit for the amount of samples
        
    params.samplePath = ""; // path of ONE specific sample
    params.compareLibraryPath = ""; // path of sample library
    params.featurePath = fs::path("./json_data/Dirt-Samples2020-09-14.20:53:18.json").string(); // path of precalculated features of sample library
    params.nSimilar = 25;  // get N most similar samples (from samplePath, search in sample library)
    
    params.gtPath = fs::path("./groundTruth/DIRT_LABELS.JSON").string();   // path to "Ground truth labels"
    params.gtLabel = 1;  // specify GT-Label (Cochlear has 3 labels with descending meaningfulness)

    params.saveSamplePath = fs::path("./reorganized_samples/").string();  // path to an existing folder where the regrouped samples will be copied.
    
    params.gtFlagBool = false;
    params.expJSON = false;
    params.onlyFeat = false;
    params.useAllFeat = false;
    params.expSampl = false;
    params.expByGT = false;
    
    // cluster a sample library
    auto lp = app.add_option("--lp", params.libraryPath, "Path to sample library. Default: NONE.");
    auto nCl = app.add_option("--nCl", params.clusters, "Amount of clusters to group the sample library. Default: 40");
    auto nUl = app.add_option("--nUl", params.nUl, "Upper limit for amount of samples to analyse: Default -1 (all)");
    //validates
    lp->check(CLI::ExistingDirectory);
    nCl->check(CLI::PositiveNumber);
    nUl->check(CLI::PositiveNumber);
    
    auto fp = app.add_option("--fp", params.featurePath, "Path to precalculate features of a sample library (JSON file, generated through --eF). Default: NONE");

    //Constrains
    nCl->needs(lp);
//    nUl->needs(lp);
    // get N similar samples to a specified sample
    auto sp = app.add_option("--sp", params.samplePath, "Path to sample. Default: NONE.");
    auto clp = app.add_option("--clp", params.compareLibraryPath, "Path to compare sample library. Default: NONE");
    auto nS = app.add_option("--nSim", params.nSimilar, "Find N most similar samples. Default: 25");
    
    //validates
    sp->check(CLI::ExistingFile);
    clp->check(CLI::ExistingDirectory);
    fp->check(CLI::ExistingFile);
    nS->check(CLI::PositiveNumber);
    //Constrains
    sp->excludes(lp);
    clp->excludes(lp);
    fp->excludes(lp);
    nS->excludes(lp);
    lp->excludes(sp);
    lp->excludes(clp);
    lp->excludes(fp);
    lp->excludes(nS);    
    
    clp->excludes(fp);
    fp->excludes(clp);
    
    clp->needs(sp);
    nS->needs(sp);

    //"Ground truth label"
    auto gtFlag = app.add_flag("--gtF", params.gtFlagBool, "Load preannotated 'ground truth' of sample library. Default: NOT set.");
    auto gt = app.add_option("--gt", params.gtPath, "Path to a preannotated 'ground truth'-JSON file of a sample library. Default: ./groundTruth/DIRT_LABELS.JSON");
    auto gtl = app.add_option("--gtL", params.gtLabel, "Which GT-Label {1,2,3}. Default: 1.");
    auto gtExpo = app.add_flag("--gtOF", params.expByGT, "Copying all samples according to their preannotated ground truth label (nothing else will be done). Default: NOT set.");

    //Validate
    gt->check(CLI::ExistingFile);
    gtl->check(CLI::IsMember({1,2,3}));
    //Constrains
    gtFlag->needs(lp);
    gtFlag->needs(gt);
    gtFlag->needs(gtl);
    
    gt->needs(gtFlag);
    gtl->needs(gt);
    
    gtExpo->needs(gtFlag);
    gtExpo->needs(gt);
    gtExpo->needs(gtl);
    gtExpo->needs(lp);
    gtExpo->excludes(sp);
    sp->excludes(gtExpo);
    
    //exportJson
    auto oJson = app.add_flag("--eF", params.expJSON, "Export features to JSON file. Default: NOT set.");
    auto justFeat = app.add_flag("--onlyF", params.onlyFeat, "Only export JSON feature file. Default: NOT set.");
    auto allFeat = app.add_flag("--allF", params.useAllFeat, "Use all features (only recommended for exporting features to JSON file. Default: NOT set.");
    
    //Constrains
    justFeat->needs(oJson);
    allFeat->needs(oJson);
    fp->excludes(oJson);
    
    //copy samples to new folders
    auto opFlag = app.add_flag("--oF", params.expSampl, "Save all samples according to their {clustered-label}/{n most similar} in specific folders. Default: NOT set.");
    auto op = app.add_option("-o,--op", params.saveSamplePath, "Path to save {clustered}/{n most similar} samples to (manual path must exist). Default: ./estimateSongs/...");
    //Validate
    op->check(CLI::ExistingPath);
    //Constrains
    op->needs(opFlag);
    
//    std::string sub_arg;
    auto featVector = app.add_option<std::vector<std::string>>("--features", params.features,
            "Specify the features on which the calculation should be based.\nThe default values are quite good! For more infos, type --feature_list. Default:\nHar-Log_-FACM_10 MFCC-4 MFCC-7 Har-RecChr_-FACM_12 TriChr_Centroid ZeroCrossingRate MFCC-8");
    
    featVector->delimiter(',');
//    featVector->check(PossibleFeature);
    bool featL = false; // flag for printing all possible features
    auto featList = app.add_flag("--feature_list", featL, "Active this flag to print all features with their correlation value to the precalculated annotation.\n");

    // parse command line input
    try {
        app.parse(argc, argv);
        
        //manual check if feature list is correct
        // the better way would be to go deeper into "check"-function of CL11.hpp (PossibleFeature)
        bool failed = false;
        
        std::vector<string> failedStrings;
        if(params.useAllFeat){
            std::cout << "Use all Features" << std::endl;
            params.features = possibleFeaturesVector;
        }
        
        for(auto & featSel : params.features){
             std::set<string>::iterator it = possibleFeaturesSet.find(featSel);
            if (it != possibleFeaturesSet.end()) {
                continue;
            } else{
                failed = true;
                failedStrings.push_back(featSel);
            }
        }
        if(failed){
            std::cout <<"\n";
            int i = failedStrings.size()-1;
            for( string & fsl : failedStrings){
                std::cout << fsl;
                if(i > 0){
                    std::cout <<",";
                }
                --i;
            }
            std::cout << "\tis/are not a possible feature/s to chose from.\nType: --feature_list for the full list of all features with their 'score'\nIf you are unsure, skip the feature selection and take the default ones." << std::endl;
            return 1;
        }
    } catch(const CLI::ParseError &e) {
        std::cout << e.get_exit_code() << ":" <<e.get_name() << " -> "<< e.what() << std::endl;
        app.exit(e);
        return 1;
    }
    
    if(params.libraryPath.empty() && params.featurePath.empty() && params.samplePath.empty() && !featL){
        std::cout << "Please chose a mode via (-lp or -fp) or -sp (a library path or a single sample path)\n for more information type --help" << std::endl;
        return 1;
    }
    
    
    //print all features
    if(featL){
        std::cout<<"\n\nFor calculating ALL FEATURES add : '--allF'" << std::endl;
        printFeatureListCLI();
        std::cout<<"\n\nFor calculating ALL FEATURES add : '--allF'" << std::endl;
        return 1;
    }
    
    
    std::string absPath = cleanPathFromRelative(argv[0]);
    
    // get absolute path of all directories and files
    if( (!fs::is_directory(fs::path(params.libraryPath)) || fs::path(params.libraryPath).is_relative()) && !params.libraryPath.empty() ){
        params.libraryPath = fs::path(findInFileSystemBack(absPath,params.libraryPath, 4));
    }
    if(!fs::path(params.gtPath).is_absolute() &&  !params.gtPath.empty() ){
        params.gtPath = fs::path(findInFileSystemBack(absPath,params.gtPath, 4));
    }
    if(!fs::path(params.samplePath).is_absolute() && !params.samplePath.empty()){
        params.samplePath = fs::path(findInFileSystemBack(absPath,params.samplePath, 4));
    }
    if((!fs::is_directory(fs::path(params.compareLibraryPath)) || !fs::path(params.compareLibraryPath).is_absolute()) && !params.compareLibraryPath.empty() ){
        params.compareLibraryPath = fs::path(findInFileSystemBack(absPath,params.compareLibraryPath, 4));
    }
    if(!fs::path(params.featurePath).is_absolute()){
        params.featurePath = fs::path(findInFileSystemBack(absPath,params.featurePath, 4));
    }
    if(!fs::path(params.saveSamplePath).is_absolute()){
        fs::create_directory(params.saveSamplePath);
        params.saveSamplePath = cleanPathFromRelative(fs::absolute(params.saveSamplePath));
//        params.saveSamplePath = fs::path(findInFileSystemBack(absPath,params.saveSamplePath, 4));
    }
    
    
    return 0;
}
