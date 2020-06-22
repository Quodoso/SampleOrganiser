/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "stdheader.h"

void printFeatureStats(std::map<std::string, GTStatsFeatures> groupStats){
    //printing all feature stats grouped by its ground truth label
  std::map<std::string, GTStatsFeatures>::iterator it = groupStats.begin();
  while (it != groupStats.end())
  {
      std::setprecision(8);
      std::cout<< "Label:"<< it->first << "  Entries:" << it->second.childAmount << std::endl;
      for(int si = 0; si < it->second.stats.size(); ++si){
          std::cout << "    " << it->second.stats[si].name << " feature stats: " << std::endl;
          std::cout << "    Mean: " << it->second.stats[si].mean;
          std::cout << "    Median: " << it->second.stats[si].median;
          std::cout << "    std: " << it->second.stats[si].std;
          std::cout << "    absDV: " << it->second.stats[si].absDeviation;
          std::cout << "    MAD: " << it->second.stats[si].mad;
          std::cout << "    MinMax: " << it->second.stats[si].min << " " << it->second.stats[si].max;
         std:cout << std::endl;
      }
      it++;
  }
}

void printAllFeatureEntries(std::map<std::string, GTStatsFeatures> groupStats){
        //printing all feature entries grouped by its ground truth label
    std::map<std::string, GTStatsFeatures>::iterator it = groupStats.begin();
    while (it != groupStats.end())
    {
        std::cout<< "Label:"<< it->first << "  Entries:" << it->second.childAmount << std::endl;
        for(int si = 0; si < it->second.stats.size(); ++si){
            std::cout << it->second.stats[si].entries.size() << "  size "<< it->second.stats[si].name << " features: ";
            for(int i =0; i < it->second.stats[si].entries.size(); ++i){
                std::cout <<" " << std::to_string(it->second.stats[si].entries[i]);
            }
           std::cout << std::endl;
        }
        it++;
    }
}