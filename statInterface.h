/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   simpleStats.h
 * Author: lars
 *
 * Created on August 14, 2020, 12:51 PM
 */

#ifndef SIMPLESTATS_H
#define SIMPLESTATS_H


double getMean(std::vector<double> values);

double getMedian(std::vector<double> values);

void getMinMax(std::vector<double> values, double & destMin, double & destMax);

double getStandardDeviation(std::vector<double> values, bool withMean = false, double mean = 0);

double getAbsDeviation(std::vector<double> values, bool withMean = false, double mean = 0);

double getMAD(std::vector<double> values, bool withBiasCorrection = true);

double getAutoCorrelation(std::vector<double> values, bool withMean = false, double mean = 0);

double getCovariance(std::vector<double> values1, std::vector<double> values2, bool withMean = false, double mean1 = 0, double mean2 = 0);

double getCorrelation(std::vector<double> values1, std::vector<double> values2);


//std::vector<int> calcHierarchCluster(std::vector<std::vector<double>> features, int nCluster = 40);
/**
 * Function call of the hierarchical cluster algorithm from hpp_cluster.
 * 
 * @param features  := all features in form row(1. order) = entry, column (2. etc ) = feature
 * @param nCluster  := cluster amount (default is 40, to compare with the annotation)
 * @return  vector with each entry corresponding to the label of each row from features.
 */
std::vector<int> calcHierarchCluster(std::vector<std::vector<double>> features, int nCluster = 40);

/**
 * Rescales a numeric vector to a new range
 * @param vec := numeric vector
 * @param newMax := new maximum 
 * @param newMin := new minimum
 * @return  := rescaled vector
 */
template <class NumericType>
std::vector<double> scaleVector(std::vector<NumericType> vec, double newMax = 1.0, double newMin = 0.0){
    
    std::vector<double> scales(vec.size());
    double oldMin = static_cast<double>(std::min(vec));
    double oldMax = static_cast<double>(std::max(vec));
    
    if (oldMax - oldMin == 0 || newMax - newMin == 0){
        for(int i = 0; i < vec.size(); ++i){
            scales[i] = static_cast<double>(vec[i]);
        }
    }else{
        for(int i = 0; i < vec.size(); ++i){
            scales[i] = (newMax - newMin) / (oldMax - oldMin) * (static_cast<double>(vec[i])- oldMin) + oldMin;
        }
    }
    return scales;
}

template <class NumericType>
double scaleValue(NumericType val, double oldMax = 1.0, double oldMin = 0.0, double newMax = 1.0, double newMin = 0.0){
    if (oldMax - oldMin == 0 || newMax - newMin == 0){
        return static_cast<double>(val);
    }else{
        return (newMax - newMin) / (oldMax - oldMin) * (static_cast<double>(val)- oldMin);
    }
    return static_cast<double>(val);
}
#endif /* SIMPLESTATS_H */

