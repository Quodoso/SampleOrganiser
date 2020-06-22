/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AudioFeature.h
 * Author: lars
 *
 * Created on August 11, 2020, 12:15 PM
 */

/***
 * AudioFeature contains one single extracted audio feature.
 * Could contain multi valued features
 */
#ifndef AUDIOFEATURE1D_H
#define AUDIOFEATURE1D_H

#include <vector>


//TODO: rewrite audiofeatures, so that name is a key and the remaining part is the feature
//and maybe make a struct and not a class

class AudioFeature {

public:
    std::string name;
    std::string notes;
    std::vector<double> values;
    std::vector<std::string> strValues;
    
    AudioFeature();

    /*String Features*/
    
    AudioFeature(std::string name, std::vector<std::string> strValues, std::string notes = ""){
        this->name = name;
        this->strValues = strValues;
        this->notes = notes;
    };
    
    AudioFeature(std::string name, std::string strValue, std::string notes = ""){
        this->name = name;
        this->strValues.push_back(strValue);
        this->notes = notes;
    };
    
    /*NumberFeatures*/
    
    //Constructors for each allowed number or vector of number
    template <class NumericType>
    AudioFeature(std::string name, NumericType value, std::string notes = ""){
        this->name = name;
        this->values = std::vector<double>{static_cast<double>(value)};
        this->notes = notes;
    };
    
    template <class NumericType>
    AudioFeature(std::string name,  std::vector<NumericType> value, std::string notes = ""){
        this->name = name;
        for(size_t i =0; i < value.size(); ++i){
            this->values.push_back(static_cast<double>(value[i]));
        }
        this->notes = notes;
    };
 
    AudioFeature(const AudioFeature& orig){
        this->name = orig.name;
        this->values.assign(std::begin(orig.values), std::end(orig.values));
        this->notes = orig.notes;
        this->strValues = orig.strValues;
    };
    
    virtual ~AudioFeature(){
        freeVector<double>(values);
        freeVector<string>(strValues);
    };


private:

};

#endif /* AUDIOFEATURE1D_H */

