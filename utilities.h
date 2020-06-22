/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   utilities.h
 * Author: lars
 *
 * Created on May 20, 2020, 12:13 PM
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <set>
#include <map>




#define GET_VARIABLE_NAME(Variable) (#Variable)

/* small value for operations where 0 is not not allowed (i.e. division) */
#define EPS 0.00000001



using namespace std;
namespace fs = std::filesystem;

/***
 *
 *
 *..... utility structs
 *
 * 
 */

/**This struct serves the purpose of bundling  low statistical features of one AudioFeature of
 * one GroundTruth-Group
 */
typedef struct FeatureStats_
{
    std::string name;
    double mean;
    double median;
    double std;
    double min;
    double max;
    double mad;
    double absDeviation;
    std::vector<double> entries;
    std::vector<std::string> strEntries;
    
    void calcStatis(){
        this->mean = 0.0;
        this->median = 0.0;
        this->std = 0.0;
        this->min = 0.0;
        this->max = 0.0;
        this->mad = 0.0;
        this->absDeviation = 0.0;
        if(entries.size() > 0){
//            this->mean = getMean(entries);
//            this->median = getMedian(entries);
//            this->std = getStandardDeviation(entries, true, this->mean);
//            getMinMax(entries,this->min, this->max);
//            this->mad = getMAD(entries,true);
//            this->absDeviation = getAbsDeviation(entries,true,this->mean);
        }

    }
    
}FeatureStats;

/**This struct serves the purpose of bundling low statistical features of one AudioFeature of
 * one GroundTruth-Group in one wrapper struct
 */
typedef struct GTStatsFeatures_
{
    int childAmount;
    std::vector<FeatureStats> stats;
    std::vector<std::vector<double>> singleOccurences;
    
}GTStatsFeatures;




/**This struct serves the purpose of bundling GroundTruthLabel with its location
 */
typedef struct SampleGT_
{
    std::string dirName = "_";
    std::string fileName = "";
    std::string label01 = "";
    std::string label02 = "";
    std::string label03 = "";
    
    std::string getLabel(int selector = 1){
        switch (selector){
            case 1: return this->label01;
            case 2: return this->label02;
            case 3: return this->label03;
            default: return this->label01;
        }
    }
}SampleGT;

/** Helper Struct for switching between ouput modes
 * it differentiate between reorganise by "cluster/gt_label/one file"
 */
typedef struct OrgaStruct_
{
    std::string value;
    std::string name;
    
    void set(std::string v, std::string name){
        this->value = v;
        this->name = name;
    };
  
}OrgaStruct;


extern const std::vector<string> SPECTRA_NAMES;
/***
 *
 *
 *..... utility statics
 *
 * 
 */


/*
 *   Creates an ordinal vector filled from 0,1,...,length.
 *   offset for for each entry can be defined
 *   as well as a linear operation for each entry.
 */
vector<float> getOrdinalVector(const int, const int, const float);

/*
 * Creates vector filled with linear frequency bins
 */
vector<float> getLinearFrequencyAxes();
/*
 * Get all .wav files from a given Directory with all .wav files in all subdirectories
 */
vector<string> getAllWavPaths(string);


/**
 * replaces substrings out of a string with a given substring
 * @param str  := string to modify
 * @param from := substring to replace
 * @param to   := substring which will be inserted
 * @return     := freshly baked string
 */
std::string replaceAll(std::string str, const std::string from, const std::string to);

/*
 * Prints all entity related features grouped by its cluster
 */
//void printClusterFeatureMap(std::map<int, std::vector<DBPoint>>&);

/**
 * Generic free vector routine
 * @param vec  := vector which memory should be set free
 */
template <typename T>
inline void freeVector(std::vector<T>& vec){
    vec.clear();
    vec.shrink_to_fit();
}

template<typename T>
inline std::vector<T> convPointArrToVec(T* arr, int n){
    std::vector<T> vec(arr, arr + n);
    return vec;
}

template<typename T>
inline T* convVecToPointArr(std::vector<T> & vec){
//    return vec.data();   // PAIN IN THE ASS!! results in "double free or corruption"
    T* p = new T[vec.size()];
    for( int i = 0;  i < vec.size(); ++i){
        p[i] = vec[i];
    }
    return p;
}

/**
 * Converts a 2D-Matix of floats to a 2D-Matrix of double
 * @param toConvert := matrix which should be converted  (std::vector<std::vector<float>>)
 * @return          := converted matrix (std::vector<std::vector<double>>)
 */
std::vector<std::vector<double>> convFloatMatrixToDouble(std::vector<std::vector<float>> & toConvert);

/**
 * Extract numbers from a string. optional decimal and kilo character as well as negative prefix.
 * @param text := string which may contain 0,1..n numbers
 * @param decPoint := character for decimal point (10.00 or 15,02)
 * @param kPoint  := character for kilo convention (20.00.000,10 or 40,123,321,00)
 * @param neg   := prefix sign ( -5)
 * @param split := specify a splitting sequence (5123|13.1241,42  or 23,12323,1231415415,123)
 * @return  := vector of a specified NumericValue (int, float, etc)
 */
template <class NumericValue>
std::vector<NumericValue> getNumbersFromString(string text, string split = "", string decPoint = "", string kPoint = "", string neg = "" ){
//       std::regex e (neg+"?([0-9]+" +kPoint+ ")*[0-9]+(" + decPoint + "[0-9]{2})?");
    std::vector<NumericValue> res;
    std::regex e (neg+"?(((([0-9]{1-2}"+kPoint+")?([0-9]{3}"+kPoint+")*)([0-9]{3}))|([0-9]+))(" + decPoint + "[0-9]{2})?");
    std::smatch m;
    while (std::regex_search (text,m,e)) {
        for (auto x:m) std::cout << x << " ";
        std::cout << std::endl;
        text = m.suffix().str();
        res.push_back(std::stoi(text));
      }
    return res;
}

template <class T>
void printVector(std::vector<T> has_toString, int lineOffSet = 5){
    int i = 1;
    for(T e : has_toString){
        std::cout << e << " ";
        if( i % lineOffSet == 0){
            std::cout << std::endl;
            ++i;
            i = i % lineOffSet;
        }
    }
    std::cout << std::endl;
}
template <class T>
void printVector(std::vector<std::vector<T>> has_toString){
    for(std::vector<T> e : has_toString){
        printVector(e);
    }
    std::cout << std::endl;
}

/*
 * Only positive Modulo operation
 */
template <typename T>
inline T posModulo(T & a,T & m){
    return (m + (a%m)) % m;
}

template <class NumericType>
double Entropy(std::vector<NumericType> vec) {
    double result = 0.0;
    for (int i = 0; i < vec.size(); i++){
        result += vec[i] * log(vec[i] + EPS);
    }
    return -1 * result;
}

template <class NumericType>
NumericType vectorSum(std::vector<std::vector<NumericType>> & vec2){
    NumericType sum = 0;
    std::for_each (std::begin(vec2), std::end(vec2), [&](std::vector<NumericType> vec) {
        std::for_each (std::begin(vec), std::end(vec), [&](const double d) {
            sum += d;
        });
    });
    return sum;
}

template <class NumericType>
void meanStd(std::vector<NumericType> v, NumericType &m, NumericType &stdev) {
    double sum = 0.0;
    std::for_each (std::begin(v), std::end(v), [&](const double d) {
        sum += d;
    });
    m =  sum / v.size();

    double accum = 0.0;
    std::for_each (std::begin(v), std::end(v), [&](const double d) {
        accum += (d - m) * (d - m);
    });

    stdev = sqrt(accum / (v.size()-1));
}

template<class NumericType>
NumericType getVectorMax(std::vector<std::vector<NumericType>> & v){
    NumericType max = 0;
    for(std::vector<NumericType> & v1 : v){
        NumericType tmp = *std::max_element(v1.begin(), v1.end());
        if( tmp > max){
            max = tmp;
        }
    }
    return max;
}

template<class NumericType>
NumericType getVectorMin(std::vector<std::vector<NumericType>> & v){
    NumericType min = 0;
    for(std::vector<NumericType> & v1 : v){
        NumericType tmp = *std::min_element(v1.begin(), v1.end());
        if( tmp < min){
            min = tmp;
        }
    }
    return min;
}

template<class NumericType>
int getCompressedIndex(NumericType value,NumericType minValue, NumericType maxValue, int newMax){
   return (int)std::round(((value*1.0-minValue*1.0)/(maxValue*1.0 - minValue*1.0))* newMax*1.0);
//    return   (int)((value*1.0-minValue)/(maxValue*1.0-minValue))*newMax*1.0;

//            return (int)std::round((value/((double)this->maxValue))*this->newMax);
}
/**
 *  Elementwise scalar operation
 * @param v1
 * @param v2
 * @return 
 */
template<class NumericType>
std::vector<std::vector<NumericType>> getVectorAdd(std::vector<std::vector<NumericType>>  v1, std::vector<std::vector<NumericType>>  v2, float scalar = 1){
    try{
        
        std::vector<std::vector<NumericType>> res(v1.size(), std::vector<NumericType> (v1[0].size(), 0));
        
        if(v1.size() != v2.size()) throw(-1);
        
        for(int i = 0; i < v1.size(); ++i){
            
            if(v1[i].size() != v2[i].size()) throw(-1);
                    
            for(int j =0; j < v1[i].size(); ++j){
                res[i][j] = ((v1[i][j] + v2[i][j]) * 1.0)/scalar;
            }
        }
       return res;
    }catch (int s1){
        std::cout << "Both matrices have not the same shape" << std::endl;
        return v1;
    }
}


template<class NumericValue>
std::map<double, int> get_N_Minimum(std::vector<NumericValue> & vec, int n, bool verbose = false){
    std::map<double, int> sorted;
    std::map<double, int> res;
    int i = 0;
    for(NumericValue & val : vec){
        sorted[static_cast<double>(val)] = i;
        ++i;
    }
    if(n < vec.size()){ 
        int i = 0;
        for( auto it = sorted.begin(); i <= n; ++it){
            res[it->first] = it->second;
            ++i;
        }
        if(verbose){
            std::map<double, int>::iterator it = res.begin();
            while(it != res.end()){
                std::cout << "(i)" <<it->second << ":(d)" << it->first << std::endl;
                it++;
            }
        }
        return res;
    }
    else{
        return res;
    }
}
/**
 * Checks if a file exists or not. Prints an error message if not.
 * @param filePath := path of a possible file
 * @return := true = file exists | false = file does NOT exist
 */
bool isFileValid(std::string filePath);

/**
 * Checks if a folder exists or not. Prints an error message if not.
 * @param folderPath := path of a possible file
 * @return := true = file exists | false = file does NOT exist
 */
bool isFolderValid(std::string folderPath);

bool isFileRegular(std::string filePath);

//Path functions
std::string getNameInPath(std::string path, int target = 1, bool appendSlash = false, bool prepandSlash = false);

std::string getFolderPath(std::string path, bool withEndSlash = true);

std::string getFileName(std::string loc, bool replaceSpace = false, bool withExt = false);

std::string getFileExt(std::string path, bool withDot = true);

std::string cleanPathFromRelative(std::string path);

std::string findInFileSystemBack(std::string absolutePath, std::string name , int depth = 4);


inline std::string printBool(bool b){ return b ? "True" : "False"; }

std::vector<SampleGT> readAndGetGroundTruth(std::string jsonLoc, bool withGT, bool verbose = true);

const std::string currentDateTime( bool replacePoint = false);

std::string convertSecondsToTime(const double secs);
#endif /* UTILITIES_H */

