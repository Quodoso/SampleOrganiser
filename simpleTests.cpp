/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "stdheader.h"

using namespace std;
namespace fs = std::filesystem;

//TODO: prepare an test vector of audiofiles with their features

void testprepareFeatureWhiteList(){
    std::cout << "Start test of selection of feature whitelist" << std::endl;
    std::vector<std::string> selFeat = {"MFCC-5", "Bark28_kurtosis", "AutoCorrelation_sig", "Lin_AutoCorrelation", "TriChr_spread", 
                        "Rhythm13-bpm", "Bark28_spread", "key_key", "chords_key", "chords_number_rate", "key_strength", "EffectiveDuration",
                        "EffectiveToStdDuration"};
    
    std::unordered_map<std::string, std::unordered_set<std::string>> whitelist = prepareFeatureWhiteList(selFeat);
    for(auto & umap : whitelist){
        std::cout << "Spectrum or Mainfeature: " << umap.first << std::endl;
        for( auto & uset : umap.second){
            std::cout << "\t"<<uset;
        }
        std::cout << std::endl;
    }
}

void checkWavFormat(){
    std::string filePath = "/home/lars/Music/generated/fullSine.wav";
    
    std::vector<float> mono;
    getMonoAudio(filePath, mono);
    auto  mini =  std::min_element(std::begin(mono), std::end(mono)) ;
    auto maxi = std::max_element(std::begin(mono), std::end(mono)) ;
    std::cout << "Essentia file format goes from" << std::endl;
    std::cout << std::to_string( mono[std::distance(mono.begin(), mini)]) << "  to " << std::to_string( mono[std::distance(mono.begin(), maxi)]) << std::endl;
}
void checkSpectrumFormat(){
    

    std::string filePath = "/home/lars/Music/generated/dirac.wav";
    std::vector<float> mono;
    getMonoAudio(filePath, mono);
    std::vector<std::vector<float>> spectrogramm =  calcSpectrogram(mono);
    auto mini =  getVectorMin(spectrogramm);
    auto maxi =  getVectorMax(spectrogramm);
    std::cout << "Essentia spectrum format goes from" << std::endl;
    std::cout << std::to_string( mini) << "  to " << std::to_string( maxi) << std::endl;
    std::cout << "The Spectrum contains:" << std::endl;
//    printVector(spectrum);
    
    
}

void testFACMs(){
    
//    std::string filePath = "/home/lars/Documents/Samples/Dirt-Samples/amencutup/002_AMENCUT_003.wav";
    std::string filePath = fs::absolute(fs::path("./testSamples/amencut/020_AMENCUT_021.wav")).string();
    std::cout << "load file: " << filePath << std::endl;
    AudioFile af = AudioFile(filePath);
    
    
    std::cout << "smallest number is EPS " << EPS << std::endl;
    std::vector<float> eqs = std::vector<float>{0,250,2000,5000,20500};
    std::vector<float> mono = af.getMonoData();
    
    std::vector<float> db;
    convertLinearSignalToDBScale(mono,db);
    std::cout << "The mono file is:" << std::endl;
//    printVector(mono);
    std::cout << "The dbScale from mono is:" << std::endl;
//    printVector(db);
    auto  mini =  std::min_element(std::begin(mono), std::end(mono)) ;
    auto maxi = std::max_element(std::begin(mono), std::end(mono)) ;
    std::cout << std::to_string( mono[std::distance(mono.begin(), mini)]) << "  to " << std::to_string( mono[std::distance(mono.begin(), maxi)]) << std::endl;
    std::vector<std::vector<float>> sp = calcSpectrogram(mono);
    std::vector<std::vector<float>> eqSpek = calcFrequencyBandSpectrogram(sp, eqs);
    std::vector<std::vector<float>> recChromSpectrogram  = calcChromaticSpectrogram(sp, "rectangular");
    printf("EQ-Spektrogramm\n");                         
    
    std::vector<std::vector<float>> melbands = calcMelBands(mono);
    printf("MelBands have size: %d x %d \n", melbands.size(), melbands[0].size());
   
    
    std::vector<std::vector<float>> t1 = std::vector<std::vector<float>>{
        {0.9 , 0.3 , 0.9 },
        {0.2 , 0.8 , 0.9},
        {0.1 , 0.8 , 0.8},
        {0.0 , 0.2 , 0.4}
    };
                                                                        
    printf("Matrix to test\n");                                 
    printVector(t1);
     
     FACM<float> facm2 = FACM<float>(t1);     
     
     std::vector<std::vector<double>> f1 = facm2.getFACM(0);
     std::vector<std::vector<double>> f2 = facm2.getFACM(1);
     std::vector<std::vector<double>> f3 = facm2.getFACM(2);
     
     printf("Parallel FACM\n");  
     printVector(f1);
     printf("Sequentiell FACM\n");  
     printVector(f2);
     printf("Combined FACM\n");  
     printVector(f3);
     
     Haralick<double>  har = Haralick<double>(f3);
     auto tmp = har.getFeaturesFromGLCM(f3, true);
    
    std::cout << "\n recChromSpectrogram Haralicks of audio file by calculating step by step" << std::endl;
    FACM<float> facmSp = FACM<float>(recChromSpectrogram); 
    Haralick<double>  har2 = Haralick<double>(facmSp.getFACM(2));
    auto tmp2 = har2.getFeaturesFromGLCM(facmSp.getFACM(2), true);
    
    std::cout << "\n recChromSpectrogram Haralicks of audio file by calculating accumulated functions" << std::endl;
    std::vector<double> harAF = getHaralicks(getFACM(recChromSpectrogram), true);\

    std::cout << "\n calc Haralicks with AudioFile-Class" << std::endl;
    AudioFile audio = AudioFile(filePath);
    
//}
}

void testFolderFileOperation(){
    
//    std::string path = "foo/moo/baaba/lala/OrganizerFlowParamsWAAAAAAA.json";
//    std::string path = "/home/lars/Documents/Samples/Dirt-Samples/sn/STATASA.wav";
//    std::string path = "OrganizerFlowParamsWAAAAAAA.json";
    std::string path = "/home/lars/Documents/Samples/Pres/BIRD_HI_S_LIST_FX_07_15_70.wav";
//    std::string path = "/home/lars/.Documents/Samples/Dirt-Samples/";
    std::string res = getNameInPath(path, 1);
    std::string res2 = getNameInPath(path, 2);
    if( res == "sn" && res2 == "Dirt-Samples"){
        std::cout << res <<" and " << res2 << " are the correct parent folder name for " << path << std::endl;
    }else{
        std::cout << res <<  " and " << res2 <<  " are  NOT THE CORRECT folder name for " << path << std::endl;
    }

    res = getFileName(path, false, true);
    res2 = getFileName(path, false, false);
    if( res == "STATASA.wav" && res2 == "STATASA"){
        std::cout << res << " is file name of " << path << "  and " << res2 << " without extension" <<std::endl;
    }else{
        std::cout << res <<  " and " << res2 <<  " are  NOT THE CORRECT file name of " << path << std::endl;
    }
    
    res = getFolderPath(path, true);
    res2 = getFolderPath(path, false);
    if( res == "/home/lars/Documents/Samples/Dirt-Samples/sn/" && res2 == "home/lars/Documents/Samples/Dirt-Samples/sn"){
        std::cout << res << " is the correct folder path for " << path <<  "   and " << res2 <<" contains no ending slash" <<std::endl;
    }else{
        std::cout << res <<  " and " << res2 <<  " are  NOT THE CORRECT folder path for " << path << std::endl;
    }
    res = getFileExt(path, true);
    res2 = getFileExt(path, false);
    if( res == ".wav" && res2 == "wav"){
        std::cout << res << " is the correct file extention for " << path <<  "   and " << res2 <<" contains no dot" <<std::endl;
    }else{
        std::cout << res <<  " and " << res2 <<  " are NOT THE CORRECT file extention path for " << path << std::endl;
    }
    
    
}

void saveExampleFACM(int seqDist){
    // get all audio file paths
    
    //This contains a Sample of a musical Toy and has a small melody  mostly placed in "monophone" nature
    std::string toySample = "./testSamples/toys/012_ToyNotes.wav";   
    //This contains a classical 808 snare drum
    std::string snareSample = "./testSamples/808sd/SD5010.WAV";
    //This contains a female speaker saying "four"
    std::string voice = "./testSamples/numbers/4.wav";
     //This contains male speaker saying "merry Christmas"
    std::string xmas = "./testSamples/xmas/170535__cognito-perceptu__merry-christmas.wav";
    // a small Sample from a drum loop
    std::string beatLoop = "./testSamples/breaks157/000_PLEAD.WAV";
    
    std::vector<string> audioPaths = std::vector<std::string>{toySample, snareSample, voice, xmas, beatLoop};
    for(auto & f : audioPaths){
        f = fs::absolute(f);
        std::cout << f << std::endl;
    }
    
    // load all audio files with equal loudness (RMS loudness)
    std::cout << "start reading audio files" << std::endl;
    std::vector<AudioFile> allFiles(audioPaths.size());
     for(unsigned int i = 0; i < audioPaths.size(); ++i){
        allFiles[i] = AudioFile(audioPaths[i]);
    }
    std::cout << allFiles.size() << " audio files read" << std::endl;
    
    // calculating sequential, parallel and combined FACM and save it into json
    
    nlohmann::json facmData;
    
    for(AudioFile & file : allFiles){
        std::cout << "Start calculation features for: " <<file.getFileLocation() << std::endl;
        std::vector<float> monoData = file.getMonoData();
        std::vector<std::vector<float>> linSpectrogram = calcSpectrogram(monoData);
        std::vector<std::vector<float>> barkSpectrogram = calcBarkBandSpectrogram(linSpectrogram);
        
        facmData["path"].push_back(file.getFileLocation());
        
        FACM<float> facm = FACM<float>(barkSpectrogram);
        //iterate through all 3 basic FACM-Possibilities
        for(int i = 0; i < 3; ++i){
            std::cout << "Calculate FACM_"<< i << std::endl;
            facmData["FACM_"+std::to_string(i)].push_back(facm.getFACM(i));
        } 
    }
    writeJSON(facmData, "./testSamples/exampleFACMs" +currentDateTime() +".json", true);

    
    
    
    
}


void testGLCM(){
//    string papaDir = "/home/lars/Documents/Samples/Dirt-Samples/";
//    std::vector<string> kinfile = getAllWavPaths(papaDir);
//    kinfile.resize(2);
//    int temp = kinfile.size();
////    int temp = 40;
//    vector<AudioFile> allFiles(temp);
//    for(unsigned int i = 0; i<temp; ++i){
//        allFiles[i] = AudioFile(kinfile[i]);
//    }
    
    std::vector<std::vector<float>> t1 = std::vector<std::vector<float>>{{2.0, 4.0, 8, 1.3},
                                                                        {9.1, 6.6, 5, 4.8},
                                                                        {2.9, 9.6, 5, 6.8},
                                                                        {1.5, 1.2, 1, 1/5},
                                                                        {0.1, 1.1, 0, 1.5},
                                                                        {2.7, 2.8, 2.2, 2.7}};
                                                                        
    std::vector<std::vector<float>> t2 = std::vector<std::vector<float>>{{5.0, 2.0, 1.0, 9.0},
                                                                        {3.0, 6.0, 0.0, 0.0},
                                                                        {3.0, 1.0, 8.0, 1.5},
                                                                        {1.5, 0.1, 2.1, 0.2},
                                                                        {0.2, 1.1, 6.1, 1.9},
                                                                        {3.5, 9.5, 0.6, 6.7}};   
                                                                        
                                                                        
                                                                        
    std::vector<std::vector<float>> t3 = getVectorAdd(t1,t1, 1);                                                                    
     printf("First Vector\n");                                 
     printVector(t1);
     printf("Second Vector\n");   
     printVector(t2);
          printf("third Vector\n");   
     printVector(t3);
     int a = getCompressedIndex(0.25, -1.0, 1.0, 5);
     std::cout << a << " correct?" << std::endl;
     
     float max_t1 = getVectorMax(t1);
     float max_t2 = getVectorMax(t2);
     printf("max_t1: %f.4, max_t2: %f.4\n",max_t1, max_t2);
     GLCM<float> glcm1 = GLCM<float>(t1, max_t1, 5);
     GLCM<float> glcm2 = GLCM<float>(t2, max_t2,5);     
     
     std::vector<std::vector<double>> g1 = glcm1.getGLCM();
     
     printf("First GLCM\n");  
     printVector(g1);
     printf("Second GLCM\n");  
//     printVector(g2);
//}
}

void checkGLCM(string papaDir){
    vector<string> audioPaths = getAllWavPaths(papaDir);
    
    audioPaths.resize(50);
//    
    // load all audio files with equal loudness
    std::cout << "start reading audio files" << std::endl;
    vector<AudioFile> allFiles(audioPaths.size());
     for(unsigned int i = 0; i<audioPaths.size(); ++i){
        allFiles[i] = AudioFile(audioPaths[i]);
    }
    std::cout << "audio files read" << std::endl;
        
    
    for(AudioFile & file : allFiles){
        std::vector<double> har = getHaralicks(file.getGLCM(file.getSpectrogram()));
        printVector(har);
    }
}
void testFACM(){
      
//    std::vector<std::vector<int>> img{  {0, 0, 0, 2, 1, 3},
//                                        {0, 2, 2, 1, 3, 2},
//                                        {0, 2, 1, 3, 0, 0},
//                                        {2, 1, 3, 0, 0, 2},
//                                        {1, 3, 0, 0, 0, 1}};
//                                        
//    GLCM glcm = GLCM(img,3,-1,1,1,true);
    
//    glcm.printOutGLCM();
    
//    Haralick har = Haralick(glcm.getGLCM());

//    har.getFeaturesFromGLCM(glcm.getGLCM(),false);

    
    
//    
//    std::vector<std::vector<float>> ffts{   {0.1, 0.0, 0.2, 0.6, 0.1, 1},
//                                            {0.5, 0.4, 0.3, 1.0, 0.8, 1},
//                                            {0.1, 0.2, 0.3, 0.4, 0.5, 0.6},
//                                            {0.0, 0.4, 0.7, 0.0, 0.2, 0.1}};
                                            
    std::vector<std::vector<double>> ffts{ {0.1,0.5,0.1,0.001},
                                           { 0.0001,0.4,0.2,0.4}};
                                            
//        
//        {0.1, 0.0},
//                                            {0.5, 0.4},
//                                            {0.1, 0.2},
//                                            {0.0, 0.4}};
                                            
    FACM<double> facm = FACM<double>(ffts,4, 4);
    
    
    std::cout<<"parallel" << std::endl;
    Haralick<double> har = Haralick<double>(facm.getFACM(0));
    facm.printFACM(0);
    har.getFeaturesFromGLCM(facm.getFACM(0),true);
    
    std::cout<<"sequential" << std::endl << std::endl;
    har = Haralick<double>(facm.getFACM(1));
    har.getFeaturesFromGLCM(facm.getFACM(1),true);
    facm.printFACM(1);

    std::cout<<"combined" << std::endl;
    har = Haralick<double>(facm.getFACM(2));
    har.getFeaturesFromGLCM(facm.getFACM(2),true);
    facm.printFACM(2);

}

void testStats(){
    std::vector<double> a{1.0,2.0,3.0, -100};
    
    double b = getMean(a);
    std::cout << "mean:" <<b  << std::endl;
    
    double min,max;
    
    getMinMax(a,min,max);
    std::cout << "minx:" <<min  << " max:" <<max<< std::endl;
    
    double c = getAutoCorrelation(a);
    std::cout << "AutoCorrelation:" << c  << std::endl;
}


void testChromatic(){
    
    std::vector<float> chromoOne = getChromaticFreqs(-37.0, 108.0, 1.0, 440, true, false);
    std::cout<<"Chromatic halftone scale with averages 50 cents steps:" << std::endl;
    printVector(chromoOne);
    
    std::vector<float> chromoTwo = getChromaticFreqs(-37.0, 108.0, 1.0, 440, false, false);
    std::cout<<"Chromatic halftone " << std::endl;
    printVector(chromoTwo);
    
    
    std::vector<float> chromoThree = getChromaticFreqs(-37.0, 108.0, 1.0, 440, true, true);
    std::cout<<"Chromatic halftone + 50 cents  " << std::endl;
    printVector(chromoThree);
    
}