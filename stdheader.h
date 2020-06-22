/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   stdheader.h
 * Author: lars
 *
 * Created on May 19, 2020, 2:40 PM
 */

#ifndef STDHEADER_H
#define STDHEADER_H
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <limits>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstddef>
#include <regex>
#include <cmath> 
#include <cstdlib>
#include <filesystem>
#include <tuple>
#include <map>
#include <utility> 
#include <iomanip>
#include <type_traits>
#include <set>
#include <experimental/filesystem>
#include <filesystem>
#include <time.h>
#include <unordered_set>
#include <unordered_map>
//simple parallel
#include <parallel/algorithm>
//#include <experimental/algorithm>

#include <nlohmann/json.hpp>
#include <gsl/gsl_statistics_double.h>
#include "CLI11.hpp"
#include <essentia/algorithmfactory.h>
#include <essentia/streaming/algorithms/poolstorage.h>
#include <essentia/streaming/algorithms/fileoutput.h>
#include <essentia/pool.h>
#include <essentia/essentiamath.h>

#include "statInterface.h"
#include "utilities.h"
#include "featureSelektor.h"
#include "AudioFeature.h"
#include "GLCM.h"
#include "FACM.h"
#include "Haralick.h"
#include "AudioFile.h"
#include "distanceMetric.h"
#include "essentiaInterface.h"
#include "Json_Handler.h"
#include "printObjects.h"
#include "CLIHandler.h"
#include "src/hierarchCluster/fastcluster.h"

#include "simpleTests.h"
#endif /* STDHEADER_H */
