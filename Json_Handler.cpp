/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "stdheader.h"

/** Global application flow control variable**/
PARAMS APP_PARMS;

namespace fs = std::filesystem;

void addConstantsToJSON(nlohmann::json &allData){
    allData["CPP_FLOW_DATA"]["FRAMESIZE"] = FRAMESIZE;
    allData["CPP_FLOW_DATA"]["HOPSIZE"] = HOPSIZE;
    allData["CPP_FLOW_DATA"]["SAMPLERATE"] = SAMPLERATE;
    allData["CPP_FLOW_DATA"]["WINDOW_TYPE"] = WINDOW_TYPE;
    allData["CPP_FLOW_DATA"]["MELBAND"] = MELBAND;
    allData["CPP_FLOW_DATA"]["DBFS_ADJUST"] = DBFS_ADJUST;
//    allData["CPP_FLOW_DATA"]["CPP_CONTROL"] = APP_PARMS;
}

/**
 * Writes all features in a flat json object optimized for loading into a python panda DataFrame
 * this doesnt need to group audio files first.
 * 
 * @param groupStats := vector of audio files
 * @param path := path to save json file
 * @param name := name of file 
 * @param pretty := make pretty json (4 space intend) : bool (true, false)
 */
void wirteAudioFeatures(std::vector<AudioFile> & files, std::string path, std::string fileName, int gtLabel, bool pretty, bool verbose){
    
    std::string currTime = currentDateTime();
    //ugly windows does not allow some characters
    currTime = replaceAll(currTime, ".", "-");
    currTime = replaceAll(currTime, ":", ",");
    nlohmann::json allData;
    
    if(files.size() == 1){
        fileName = fileName +"Single";
    }
    
    std::string fileNameAndPath = fs::path(path) / ( fileName + currTime + ".json");
    
    for(int i = 0; i < files.size(); ++i){
        //add Ground Truth labels to a flat vector
        allData["gt"].push_back(files[i].getGroundTruthLabel(gtLabel));
        std::unordered_map<string, AudioFeature>::iterator itF = files[i].getAudioFeatures().begin();
        while( itF != files[i].getAudioFeatures().end() ){
            const string & fName = itF->first;
            AudioFeature & feat = itF->second;
            if(feat.values.size() == 1){
                allData[fName].push_back(feat.values[0]);
                if(verbose && i == files.size()-1){
                    std::cout << fName << " has size: " << allData[fName].size() << std::endl;
                }
            }
            if(feat.strValues.size() == 1){
                allData[fName].push_back(feat.strValues[0]);
                if(verbose &&i == files.size()-1){
                    std::cout << fName << " has size: " << allData[fName].size() << std::endl;
                }
            }
            itF++;
        }
    }
    writeJSON(allData, fileNameAndPath, pretty);
}
/**
 * Read JSON-file and return the nlohmann object
 * @param path := path to feature json file
 * @return nlohmann::json object
 */

nlohmann::json readJSON(std::string path){
    std::ifstream ifs(path);
    return nlohmann::json::parse(ifs);
}

std::vector<AudioFile> readAudioFeaturesFromJSON(std::string path, std::vector<string> featureNames){
    std::vector<AudioFile> audioFiles;
    
    nlohmann::json features = readJSON(path);
    
    if( features.find("file_path") != features.end()){
        for( std::string path : features["file_path"]){
            audioFiles.push_back(AudioFile(path, true));
        }
    }else{
        std::cout << "No file_path found in json-data-file. Please review your json" << std::endl;
        exit(0);
    }
    for(std::string featureName : featureNames){
        if( features.find(featureName) != features.end()){
            for(int i = 0; i < features[featureName].size(); ++i){
                audioFiles[i].addFeature(featureName, AudioFeature(featureName, features[featureName][i]));
            }
        }else{
            std::cout << "No " << featureName << " feature found ind json-data-file. Please review your json or generate a new one containing said feature" << std::endl;
            exit(0);
        }
    }
    
    return audioFiles;
}

void writeJSON(nlohmann::json data, std::string path, bool pretty){
    
    
    std::string fileName = getFileName(path);
    std::string folderPath = getFolderPath(path);
    
    fs::create_directories(folderPath);
    
    
    std::ofstream o(path);
    if(pretty){
        o << std::setw(4) << data << std::endl;
    }else{
        o << data << std::endl;
    }
    o.close();
    std::cout<< "Json data file writte to "<< path << std::endl;
}

/**
 * Reads the json file for handling this program. Returns false when errors occur while reading/extracting 
 * @param path := path of the OrganizerFlowParams.json file | default := ./OrganizerFlowParams.json
 * @return true = reading/extracting correct | false = error
 */
bool readProgramFlowParams(std::string path){
    bool valid = true;
    if( ! fs::exists(path)){
        std::cout<< path << " <- does not exist. This program can not operate without it" << std::endl;
        return false;
    }else{
        std::ifstream paramFill(path);
        nlohmann::json paramData = nlohmann::json::parse(paramFill);
        
        if( ! isValidProgramFlowFile(paramData)){
            std::cout << path + " does not contain necessary keys: ";
            std::cout << "sample_library_path   or ";
            std::cout << "ground_truth_path   or ";
            std::cout << "Please check the corresponding json file" << std::endl;
            std::cout << paramData.dump(4) << std::endl;
            valid = false;
        }else{
            
            APP_PARMS.samplesDirPath = paramData["sample_library_path"];
            APP_PARMS.gtPath = paramData["ground_truth_path"];
            APP_PARMS.gtLabel = paramData["select_GT_label"];
            APP_PARMS.reorganizeSamplesByGT = paramData["reorganize_samples_by_GT"];
            APP_PARMS.exportFeatureJSON = paramData["export_features_to_JSON"];
            APP_PARMS.exportStatisticJSON = paramData["export_GT_Feature_statistics"];
            APP_PARMS.analyseSingleFilePath = paramData["analyse_one_file"];
            
            if( isFileValid(APP_PARMS.analyseSingleFilePath)){
                std::cout << APP_PARMS.analyseSingleFilePath << " is a valid file. \n Only calculate features for this file." << std::endl;
            }else{
                std::cout << APP_PARMS.analyseSingleFilePath << " is not a valid file, the application will proceed as normal" << std::endl;
                APP_PARMS.analyseSingleFilePath  = "FALSE";
            }
            
            if( isFolderValid(APP_PARMS.samplesDirPath)){
                std::cout << APP_PARMS.samplesDirPath << " is a valid folder" << std::endl;
            }else{
                valid = false;
            }
         
            if( isFileValid(APP_PARMS.gtPath) && isValidGTFile(APP_PARMS.gtPath)){
                std::cout << APP_PARMS.gtPath << " is a valid ground truth file" << std::endl;
                APP_PARMS.withGT = true;
            }else{
                std::cout << APP_PARMS.gtPath << " is not a valid ground truth file" << std::endl;
                std::cout << "Evaluation will not be possible" << std::endl;
                APP_PARMS.withGT = false;
            }
            
            
        }   
        return valid;
        
    }
}
/**
 * Checks if ground truth json file has the necessary keys
 * @param gtFilePath := path of GT-json file
 * @return true if valid | false if not valid
 */
bool isValidGTFile(std::string gtFilePath){
    
    std::ifstream paramFill(gtFilePath);
    nlohmann::json paramData = nlohmann::json::parse(paramFill);
    
    return isValidGTFile(paramData);
}
/**
 * Checks if ground truth json file has the necessary keys
 * @param paramData := JSON file as nlohmann object
 * @return true if valid | false if not valid
 */
bool isValidGTFile(nlohmann::json & paramData){
    if(paramData.contains("files") && paramData["files"].size() > 0){
        if( paramData["files"][0].contains("dir_name")  && paramData["files"][0].contains("file_name")
            && paramData["files"][0].contains("label_01") && paramData["files"][0].contains("label_02") && paramData["files"][0].contains("label_03")){
            
            return true;
        }
    }

    return false;
}
/**
 * Checks if Program flow file has the necessary keys
 * @param appFlowFilePath := path of GT-json file
 * @return true if valid | false if not valid
 */
bool isValidProgramFlowFile(std::string appFlowFilePath){
    
    std::ifstream paramFill(appFlowFilePath);
    nlohmann::json paramData = nlohmann::json::parse(paramFill);
    
    return isValidProgramFlowFile(paramData);

}
/**
 * Checks if Program flow file has the necessary keys
 * @param paramData := JSON file as nlohmann object
 * @return true if valid | false if not valid
 */
bool isValidProgramFlowFile(nlohmann::json & paramData ){
    
    if( paramData.contains("ground_truth_path") && paramData.contains("sample_library_path") 
        && paramData.contains("reorganize_samples_by_GT") && paramData.contains("export_features_to_JSON")
        && paramData.contains("select_GT_label") && paramData.contains("export_GT_Feature_statistics")
            && paramData.contains("analyse_one_file")){
        return true;
    }
    return false;
}