/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "stdheader.h"

//https://www.gnu.org/software/gsl/doc/html/statistics.html

double getMean(std::vector<double> values){
    if(values.size() == 1){
        return values[0];
    }
    double* a = &values[0];
    return gsl_stats_mean(a, 1, values.size());
}

double getMedian(std::vector<double> values){
    if(values.size() == 1){
        return values[0];
    }
    std::vector<double> valueCopy = values;
    double* a = &valueCopy[0];
    return gsl_stats_median(a, 1, values.size());
}

void getMinMax(std::vector<double> values, double & destMin, double & destMax){
    double* a = &values[0];
    return gsl_stats_minmax(&destMin, &destMax, a, 1, values.size());
}

double getStandardDeviation(std::vector<double> values, bool withMean, double mean){
    if(values.size() == 1){
        return 0;
    }
    double* a = &values[0];
    if(withMean){
        return gsl_stats_sd_m(a, 1, values.size(), mean); 
    }else{
        return gsl_stats_sd(a, 1, values.size());
    }
}

double getAbsDeviation(std::vector<double> values, bool withMean, double mean){
    double* a = &values[0];
    if(withMean){
        return gsl_stats_absdev_m(a, 1, values.size(), mean); 
    }else{
        return gsl_stats_absdev(a, 1, values.size());
    }
}

double getMAD(std::vector<double> values, bool withBiasCorrection){
    double* a = &values[0];
    double work[values.size()];
    if(withBiasCorrection){
        return gsl_stats_mad(a, 1, values.size(), work);
    }else{
        return gsl_stats_mad0(a, 1, values.size(), work);
    }
}

double getAutoCorrelation(std::vector<double> values, bool withMean, double mean){
    double* a = &values[0];
    if(withMean){
        return gsl_stats_lag1_autocorrelation_m(a, 1, values.size(), mean); 
    }else{
        return gsl_stats_lag1_autocorrelation(a, 1, values.size());
    }
}

double getCovariance(std::vector<double> values1, std::vector<double> values2, bool withMean, double mean1, double mean2){
    double* a = &values1[0];
    double* b = &values2[0];
    if(withMean){
        return gsl_stats_covariance_m(a, 1, b, 1, values2.size(), mean1,mean2); 
    }else{
        return gsl_stats_covariance(a, 1, b, 1, values2.size()); 
    }
}

double getCorrelation(std::vector<double> values1, std::vector<double> values2){
    double* a = &values1[0];
    double* b = &values2[0];
    
    return gsl_stats_correlation(a, 1, b, 1, values2.size()); 

}

/**
 * Calculates the Aggloremative clustering 
 * @param features := A Matrix (vector<vector<double>>)where each row is a data entry and each column a feature
 * @param nCluster := A positive integer representing the number of clusters to aim for.
 * @return := A vector with size of the number of rows (entries) and each entry assigned to a cluster number
 */
std::vector<int> calcHierarchCluster(const std::vector<std::vector<double>> & features, int nCluster){
    
    int n = features.size();
    int* merge = new int[2*(n-1)];

    double* height = new double[n-1];
    double* distmat = calcDistanceMat(features, euklidDistance);
    
    int* labels = new int[n];

    cutree_k(n, merge, nCluster, labels);
    std::vector<int> vecLabel(labels, labels + n);
    
    delete[] height;
    delete[] distmat;
    delete[] merge;
    delete[] labels;
    
    return vecLabel;
}