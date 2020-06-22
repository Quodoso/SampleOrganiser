/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   distanceMetric.cpp
 * Author: lars
 * 
 * Created on May 27, 2020, 12:57 PM
 */

#include "stdheader.h"
double euklidDistance(const std::vector<double>& pointCore, const  std::vector<double>& pointTarget)
{
    double distance = 0.0;
    int power =2;
    for(int i=0; i< pointCore.size(); i++){
        distance += pow(pointCore[i] - pointTarget[i],power);
    }
    return std::pow(distance, 1.0/static_cast<double>(power));
}

double manhattanDistance(std::vector<double>& pointCore, std::vector<double>& pointTarget)
{
    double distance = 0.0;
    int power =1;
    for(int i=0; i< pointCore.size(); i++){
        distance += std::abs(pointCore[i] - pointTarget[i]);
    }
    return distance;
}

double maxDistance(std::vector<double>& pointCore, std::vector<double>& pointTarget)
{
    double distance = 0.0;
    int power =1;
    for(int i=0; i< pointCore.size(); i++){
        distance = std::max(std::abs(pointCore[i] - pointTarget[i]), distance);
    }
    return distance;
}

/**
 * Calculates a condensed distance matrix (mostly used for hierarchical clustering)
 * @param features := A Matrix (vector<vector<double>>)where each row is a data entry and each column a feature
 * @param distance := a function pointer to a distance function
 * @return  := a condensed distance matrix
 */
double* calcDistanceMat(std::vector<std::vector<double>> features, double (*distance)(const std::vector<double> &, const std::vector<double> &)){
    
    int n = features.size();
    double* distmat = new double[(n*(n-1))/2];
    int k,i,j;
    for (i=k=0; i<n; i++) {
      for (j=i+1; j<n; j++) {
        distmat[k] = distance(features[i], features[j]);
        k++;
      }
    }
    return distmat;
}

/**
 * Calculateds distance matrix from a vector of AudioFiles. The resulting vector contains
 * the condensed distance matrix
 * @param audiofiles := a vector of all audio files with their calculated features
 * @param featNames := a subset of features to calculate the distance matrix
 * @param sel := mode for distance function
 * @return := condensed distance matrix with (n*n -n)/2 entries
 */
std::vector<double> calcDistanceMat(std::vector<AudioFile> & audiofiles, std::vector<string> featNames, DISTANCE_SEL sel){
    
    const int FEAT = 0;
    int n = audiofiles.size();
    
    std::vector<double> distmat; 
//    distmat.resize((n*(n-1))/2);

//    int di = 0;
    //f1, f2 := file1 and file2
    for (int f1 = 0; f1 < n; ++f1 ) {
        
      for (int f2 = f1+1; f2 < n; ++f2 ) {
          
            double dist = 0.0;
            std::unordered_map<string, AudioFeature>::iterator itF = audiofiles[f1].getAudioFeatures().begin();
            while( itF != audiofiles[f1].getAudioFeatures().end()){

                if (std::find(featNames.begin(), featNames.end(), itF->first) != featNames.end()){
//                    std::cout << "name:"<< itF->first <<"         f1:"<<audiofiles[f1].getAudioFeatures()[itF->first].values[FEAT] << " f2:" << audiofiles[f2].getAudioFeatures()[itF->first].values[FEAT] << std::endl;
                      dist += (audiofiles[f1].getAudioFeatures()[itF->first].values[FEAT] - audiofiles[f2].getAudioFeatures()[itF->first].values[FEAT])
                              * (audiofiles[f1].getAudioFeatures()[itF->first].values[FEAT] - audiofiles[f2].getAudioFeatures()[itF->first].values[FEAT]);
                }
                itF++;
          }
          distmat.push_back(std::pow(dist, 0.5));
      }
    }
    return distmat;
}
/**
 * Calculates distance for each element to one specific element
 * @param audiofiles := the audio files to get the distance from
 * @param specialEl := the specific element each distance will be calculated
 * @param featNames := a subset of features
 * @return  := the distances 
 */
std::vector<double> calcDistToOneEl(std::vector<AudioFile> & audiofiles, int specialEl, std::vector<string> featNames){
    const int FEAT = 0;
    int n = audiofiles.size();
    
    std::vector<double> distmat;
    
    int f1 = specialEl;
    for (int f2 = 0; f2 < n; ++f2 ) {
        if(f1 != f2){
            double dist = 0.0;
            std::unordered_map<string, AudioFeature>::iterator itF = audiofiles[f1].getAudioFeatures().begin();
            while( itF != audiofiles[f1].getAudioFeatures().end()){

                if (std::find(featNames.begin(), featNames.end(), itF->first) != featNames.end()){
//                    std::cout << "name:"<< itF->first <<"         f1:"<<audiofiles[f1].getAudioFeatures()[itF->first].values[FEAT] << " f2:" << audiofiles[f2].getAudioFeatures()[itF->first].values[FEAT] << std::endl;
                      dist += (audiofiles[f1].getAudioFeatures()[itF->first].values[FEAT] - audiofiles[f2].getAudioFeatures()[itF->first].values[FEAT])
                              * (audiofiles[f1].getAudioFeatures()[itF->first].values[FEAT] - audiofiles[f2].getAudioFeatures()[itF->first].values[FEAT]);
                }
                itF++;
          }
          distmat.push_back(std::pow(dist, 0.5));
        }
    }
    return distmat;
}


/**
 * Converts an index pointing to an element in an condensed matrix to the pair which it originates from
 * ("inspired" from https://stackoverflow.com/a/36867493 )
 * @param distmat := condensed distance matrix
 * @param index := index
 * @return pair<int,int> of indices where the single index was pointing to.
 *
 */
std::pair<int,int> argCondDistMat(std::vector<double> & distmat, int index){
    int  size = distmat.size();
    std::pair<int,int> res;
    res.first = 
    res.second = 
    res.first  = static_cast<int>(0.5*(- std::pow((-8*size + 4 * std::pow(index,2) - 4*index -7),0.5) + 1 * index -1)-1);
    res.second = static_cast<int>(index- (res.first * (index - 1 - res.first) + (res.first*(res.first + 1))/2) + size);

    return res;
}

std::vector<int> clusterAudioFiles(std::vector<double> & distmat, int n, int nClust){
    
    int* labels = new int[n];    
    double* distmat_P = convVecToPointArr<double>(distmat);
    double* height = new double[n];  
    int* merge = new int[2*(n)];
    
    hclust_fast(n,  distmat_P, HCLUST_METHOD_AVERAGE, merge, height);
    
    cutree_k(n, merge, nClust, labels);
    delete[] distmat_P;
    delete[] height;
    delete[] merge;
    return convPointArrToVec(labels, n);
    
}