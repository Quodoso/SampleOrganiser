/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Json_Handler.h
 * Author: lars
 *
 * Created on August 12, 2020, 5:07 PM
 */

#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H

/**This struct serves the purpose of managing the process flow of this program
 */
typedef struct PARAMS_
{
    std::string samplesDirPath;
    std::string gtPath;
    bool withGT = false;
    int gtLabel = 1;
    bool exportFeatureJSON = true;
    bool exportStatisticJSON = false;
    bool reorganizeSamplesByGT = false;
    std::string analyseSingleFilePath;
    
}PARAMS;


/** Application flow params for handling and managing the application flow (surprise)
 */
extern PARAMS APP_PARMS;

/**
 * Reads the json file for handling this program. Returns false when errors occur while reading/extracting 
 * @param path := path of the OrganizerFlowParams.json file | default := ./OrganizerFlowParams.json
 * @return true = reading/extracting correct | false = error
 */
bool readProgramFlowParams(std::string path = "./OrganizerFlowParams.json");

void writeJSON(nlohmann::json data, std::string path = "./data/json_data/", bool pretty = false);

void wirteAudioFeatures(std::vector<AudioFile> & files, std::string path = "./data/json_data/", std::string fileName = "FEATURE_DATA", int gtLabel = 1, bool pretty=true, bool verbose = false);


nlohmann::json readJSON(std::string path);

std::vector<AudioFile> readAudioFeaturesFromJSON(std::string path, std::vector<string> featureNames);

void  addConstantsToJSON(nlohmann::json &allData);
/**
 * Checks if ground truth json file has the necessary keys
 * @param gtFilePath := path of GT-json file
 * @return true if valid | false if not valid
 */
bool isValidGTFile(std::string gtFilePath);
bool isValidGTFile(nlohmann::json & paramData);
/**
 * Checks if Program flow file has the necessary keys
 * @param appFlowFilePath := path of GT-json file
 * @return true if valid | false if not valid
 */
bool isValidProgramFlowFile(std::string appFlowFilePath);
bool isValidProgramFlowFile(nlohmann::json & paramData );
#endif /* JSON_HANDLER_H */

