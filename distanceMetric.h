/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   distanceMetric.h
 * Author: lars
 *
 * Created on May 27, 2020, 12:57 PM
 */

#ifndef DISTANCEMETRIC_H
#define DISTANCEMETRIC_H



double euklidDistance(const std::vector<double>&, const std::vector<double>&);
double manhattanDistance(std::vector<double>&, std::vector<double>&);
double maxDistance(std::vector<double>&, std::vector<double>&);

/**
 * Calculates a condensed distance matrix (mostly used for hierarchical clustering) (:= upper triangle without diagonal)
 * @param features := A Matrix (vector<vector<double>>)where each row is a data entry and each column a feature
 * @param distance := a function pointer to a distance function
 * @return  := a condensed distance matrix
 */
double* calcDistanceMat(std::vector<std::vector<double>> features, double (*distance)(const std::vector<double>&, const std::vector<double>&));

enum DISTANCE_SEL {EUKLID = 0, MANHATTAN = 1, MAX = 2};
std::vector<double> calcDistanceMat(std::vector<AudioFile> & audiofiles, std::vector<string> featNames, DISTANCE_SEL sel  = EUKLID);

/**
 * Calculates distance for each element to one specific element
 * @param audiofiles := the audio files to get the distance from
 * @param specialEl := the specific element each distance will be calculated
 * @param featNames := a subset of features
 * @return  := the distances 
 */
std::vector<double> calcDistToOneEl(std::vector<AudioFile> & audiofiles, int specialEl, std::vector<string> featNames);

/**
 * Returns the actual argument (index pair) of an element in a condensed distance matrix
 * @param distmat := condensed distance matrix
 * @param index := index in condensed distance matrix which should converted
 * @return  the actual index pair of the elements.
 */
std::pair<int,int> argCondDistMat(std::vector<double> & distmat, int index);


std::vector<int> clusterAudioFiles(std::vector<double> & distmat,int n, int nClust);
#endif /* DISTANCEMETRIC_H */
