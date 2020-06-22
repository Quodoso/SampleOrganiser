/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "stdheader.h"

using namespace std;
namespace fs = std::filesystem;


/* const Static variables
 */

const std::vector<string> SPECTRA_NAMES = {"Lin_", "Log_", "Mel96_", "TriChr_", "RecChr_", "time"};

/*
 Get a vector filled sorted from [offset,offset+1,...,length+offset]
 */
vector<float> getOrdinalVector(const int length, const int offset=0, const float linOp=1){
    vector<float> ordVec(length);
    for(int i=0; i<length; ++i){
        ordVec[i] = (i+offset)*linOp;
    }
    return ordVec;
}


vector<float> getLinearFrequencyAxes(){
    float deltaFreqBin = (SAMPLERATE*1.0)/(FRAMESIZE*1.0);
    return getOrdinalVector(SAMPLERATE/2,0,deltaFreqBin);
}


vector<string> getAllWavPaths(string dirPath="/home/lars/Documents/Samples/test"){
    vector<string> paths;    
    regex fileR("[^.]+.(wav|WAV|flac|FLAC|mp3)$");
    smatch m;
    //case one:  dirPath is actually a file
    if(isFileRegular(dirPath)){
        string temp = dirPath;
        regex_search (temp,m,fileR);
        if (regex_search (temp,m,fileR)){
            string pos = std::experimental::filesystem::system_complete(m.str());
            if(!fs::is_directory(fs::path(pos)))
                paths.push_back(fs::path(pos));
        }else{
        }
        return paths;
    }
    //case two:  dirPath is directory and it will iterate through ALL nested folders!
    for(auto& p: fs::recursive_directory_iterator(dirPath)){
        string temp = p.path();
        if (regex_search (temp,m,fileR) && !fs::is_directory(fs::path(temp))) {
            paths.push_back(std::experimental::filesystem::system_complete(m.str()));
        }
    }
    
    return paths;
}

/**
 * replaces substrings out of a string with a given substring
 * @param str  := string to modify
 * @param from := substring to replace
 * @param to   := substring which will be inserted
 * @return     := freshly baked string
 */
std::string replaceAll(std::string str, const std::string from, const std::string to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

std::vector<SampleGT> readAndGetGroundTruth(std::string jsonLoc, bool withGT, bool verbose){
    std::vector<SampleGT> gts;
    if(withGT){
        std::ifstream paramFill(jsonLoc);
        nlohmann::json paramData = nlohmann::json::parse(paramFill);
        for(size_t i =0; i<paramData["files"].size(); ++i){
             SampleGT gt;
             gt.dirName = paramData["files"][i]["dir_name"];
             gt.fileName = paramData["files"][i]["file_name"];
             gt.label01 = paramData["files"][i]["label_01"];
             gt.label02 = paramData["files"][i]["label_02"];
             gt.label03 = paramData["files"][i]["label_03"];
             
             if(verbose){
               std::cout << i << " GT-entry read"<< std::endl;   
               std::cout << "\tdirname: "<<gt.dirName << "\n\tfileName: "<< gt.fileName<< std::endl;
             }

             gts.push_back(gt);
         }
    }else{
        gts.push_back(SampleGT());
    }
    return gts;
}

/**
 * Get current date/time, format is YYYY-MM-DD.HH:mm:ss
 * @return := current time as string in YYYY-MM-DD.HH:mm:ss format
 */
const std::string currentDateTime(bool replacePoint) {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    std::string res = "";
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    if(replacePoint){
        res = replaceAll(std::string(buf), ":", ";");
        res = replaceAll(std::string(res), ".", ",").c_str();
    }else{
        res = buf;
    }
    return res;
}

/**
 * Converts a 2D-Matix of floats to a 2D-Matrix of double
 * @param toConvert := matrix which should be converted  (std::vector<std::vector<float>>)
 * @return          := converted matrix (std::vector<std::vector<double>>)
 */
std::vector<std::vector<double>> convFloatMatrixToDouble(std::vector<std::vector<float>> & toConvert){
    std::vector<vector<double>> vdd;
    for(std::vector<float> & m : toConvert){
           std::vector<double> vDoubles(m.begin(), m.end());
           vdd.push_back(vDoubles);
    }
    return vdd;
}

/**
 * Checks if a file is regular or not. Prints an error message if not.
 * @param filePath := path of a possible file
 * @return := true = file exists | false = file does NOT exist
 */
bool isFileRegular(std::string filePath){
    if( ! fs::is_regular_file(filePath)){
        std::cout << filePath << " is not regular. Please check the path and/or create the corresponding file" << std::endl;
        return false;
    }
    return true;
}


/**
 * Checks if a file exists or not. Prints an error message if not.
 * @param filePath := path of a possible file
 * @return := true = file exists | false = file does NOT exist
 */
bool isFileValid(std::string filePath){
    if( ! fs::exists(filePath)){
        std::cout << filePath << " does not exist. Please check the path and/or create the corresponding file" << std::endl;
        return false;
    }
    return true;
}
/**
 * Checks if a folder exists or not. Prints an error message if not.
 * @param folderPath := path of a possible file
 * @return := true = file exists | false = file does NOT exist
 */
bool isFolderValid(std::string folderPath){
   if( ! fs::is_directory(folderPath)){
        std::cout << folderPath << " does not exist. Please check the path and/or create the corresponding folder" << std::endl;
        return false;
    }
    return true;
}

/**
 * Returns the n-th name of an folder or file from a file path
 * it counts backwards. target=0 is the file, target=1 the parentDir etc.
 * @param path := filesystem path
 * @param target := file or dir position
 * @return 
 */
std::string getNameInPath(std::string path, int target, bool appendSlash, bool prepandSlash){

    if(path[0] == '/'){
      path = path.substr(1, path.size() - 1);
    }
    if(path[path.size()-1] == '/'){
        path.pop_back();
    }

    std::smatch m;
    std::regex e ("/([^/]+)");
    std::string search = path;
    std::vector<std::string> res;

    while (std::regex_search (search,m,e)) {
      res.push_back(m[1].str());

      search = m.suffix().str();
//      std::cout << search << std::endl;
    }
    int size = (res.size()-1) - target;
//    for(int i =0; i < res.size(); ++i){
//       std::cout << i<<" is " << res[i] << std::endl;   
//    }

    if(size > 0){
        return res[size];
    }
    return path;
   
}
/**
 * Get a folder path to an existing file
 * @param path := filesystem path
 * @return 
 */
std::string getFolderPath(std::string path, bool withEndSlash){
   
    std::string fileName = getFileName(path, false, true);
    path = replaceAll(path, fileName, "");
    
    if(withEndSlash && path[path.size()-1] != '/'){
        path.push_back('/');
    }
    if( !withEndSlash && path[path.size()-1] == '/'){
        path.pop_back();
    }
    return path;

}

/**
 * Gets the file name from a file given path. A file name needs an extension.
 * In case there is no extension, call getNameInPath(path, 0, clean);
 * 
 * @param path := filesystem path
 * @param replaceSpace := replace spaces in file name
 * @param withExt  := include or exclude extension
 * @return 
 */
std::string getFileName(std::string path, bool replaceSpace, bool withExt){
    
    std::string fileName = getNameInPath(path, 0);
    std::smatch fileMatch;
    

    std::regex ext("\\.([^\\.]+)");
    if(std::regex_search(fileName, fileMatch, ext)){
        if(withExt == false){
            fileName = fileMatch.prefix();
         }
    }else{
        return "Failed: No file name with extension";
    }
    if(replaceSpace){
        fileName = replaceAll(fileName, std::string(" "), std::string("_"));
    }
    
    return fileName;
}

std::string getFileExt(std::string path, bool withDot){
    std::string fileName = getNameInPath(path, 0);
    std::smatch fileMatch;
    std::regex ext("\\.([^\\.]+)");
//    std::cout << fileName << "\n";
    if(std::regex_search(fileName, fileMatch, ext)){
        fileName = fileMatch.str();
        if(fileName[0] == '.'){
            if(withDot){
                return fileName;
            }else{
                return std::string(fileName.begin()+1, fileName.end());
            }
        }
    }
    return "Failed: No File extension";
}


/**
 * Search a specific path of a directory/file from a given absolute path.
 * It will go backwards until the recursion is over or the directory is found.
 * There is no iterating through all directories in a current depth. It just goes backwards.
 * @param absolutePath := (fs::path ) Absolute path where the search should begin
 * @param name      := (string) Name of the directory/file which should be found
 * @param depth        := (int) Maximal Steps to iterate backwards
 * @return (string) (absolute path of a specific directory name := found | "" := not found)
 */
std::string findInFileSystemBack(std::string absolutePath, std::string name, int depth){
    //abort condition
    if(depth < 0 || absolutePath == ""){
        return "";
    }else{
        if(fs::exists(fs::path(absolutePath) / name)){
            return fs::path(absolutePath) / name;
        }
        return findInFileSystemBack(fs::path(absolutePath).parent_path() ,name, --depth);
    }
}

/**
 * Cleans a path from "/./" and "/."
 * @param path  := (string) path
 * @return  (string) path without "/./" and "/."
 */
std::string cleanPathFromRelative(std::string path){
    //path of current library and find specific dirs/files
    fs::path p{path};
    std::string rplStr = "/./";
    
    fs::path absPath = fs::absolute(p);
    if( !fs::is_directory(absPath) ){
        absPath = absPath.parent_path();
    }
    
    size_t found = absPath.string().find(rplStr);
    if( found != std::string::npos){
        absPath = absPath.string().erase(found, rplStr.size()-1);
    }
    int pSize = absPath.string().size();
    if( absPath.string()[pSize-2] == '/' && absPath.string()[pSize-1] == '.'){
        absPath = absPath.string().substr(0,pSize-2);
    }
    return absPath;
}

/**
 * Phi-related Hash-function for unordered_map and unordered_set
 * (shamelessly copied from https://www.geeksforgeeks.org/how-to-use-unordered_map-efficiently-in-c/ )
 * @param x 
 * @return 
 */
struct modified_hash { 
  
    static uint64_t splitmix64(uint64_t x) 
    { 
        x += 0x9e3779b97f4a7c15; 
        x = (x ^ (x >> 30)) 
            * 0xbf58476d1ce4e5b9; 
        x = (x ^ (x >> 27)) 
            * 0x94d049bb133111eb; 
        return x ^ (x >> 31); 
    } 
  
    int operator()(uint64_t x) const
    { 
        static const uint64_t random = std::chrono::steady_clock::now()
                                       .time_since_epoch()
                                       .count(); 
        return splitmix64(x + random); 
    } 
}; 


std::string convertSecondsToTime(const double secs){
    
    std::string time = "";
    
    //under second
    int secInt = static_cast<int>(std::floor(secs));
    int secTmp = static_cast<int>(secs);
    double mod = secTmp - secInt;
    time = "," + std::to_string(mod);
    
    //seond
    secInt = std::floor(secInt / 60);
    mod = std::floor((secTmp - secInt * 60));
    secTmp = secInt;
    time = std::to_string(int(mod)) + "s";
    //minute
    if(secInt >= 1){
        secInt = std::floor(secInt / 60);
        mod = std::floor((secTmp - secInt * 60));
        secTmp = secInt;
        time = std::to_string(int(mod)) + "m:" + time;
    }
    //hour
    if(secInt >= 1){
        secInt = std::floor(secInt / 24);
        mod = std::floor((secTmp - secInt * 24 ));
        secTmp = secInt;
        time = std::to_string(int(mod)) + "h:" + time;
    }
    //day
    if(secInt >= 1){
        secInt = std::floor(secInt / 7 );
        mod = std::floor((secTmp - secInt * 7 ));
        secTmp = secInt;
        time = std::to_string(int(mod)) + "d:" + time;
    }
    return time;
}