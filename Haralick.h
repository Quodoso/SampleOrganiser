/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * Original File: https://github.com/Abello966/opencv-haralickfeatures/blob/master/src/haralick_feat.h
 * Original Author: Abello966,emmenlau 
 * Modified File:   Haralick.h
 * Modified Author: lars
 *
 * Created on June 6, 2020, 8:45 AM
 */

#ifndef HARALICK_H
#define HARALICK_H

#include <cmath>


#define EPS_SMALL 0.00000000001
/* TO-DO: add Angualr second moment!
 */
template <class NumericType>
class Haralick{
        std::vector<std::vector<NumericType>> glcm;
        std::vector<double> margprobx;
        std::vector<double> margproby;
        std::vector<double> probsum; //sum probability
        std::vector<double> probdiff; //diff probability
        double hx, hy; //entropy of margprobx and y
        double meanx, meany, stddevx, stddevy;
        bool initial=false; //marks if above variables are set
        
        
                /*calculates probsum, probdiff, margprobx and y at once*/
        void fast_init() {
            if (glcm.empty())
                return;
            freeVector(margprobx);
            margprobx.resize(glcm.size(), 0.0);
            freeVector(margproby);
            margproby.resize(glcm[0].size(), 0.0);
            freeVector(probsum);
            probsum.resize(glcm.size() * 2, 0.0);
            freeVector(probdiff);
            probdiff.resize(glcm[0].size(), 0.0); 

            double local;
            for (int i = 0; i < glcm.size(); ++i) {
                for (int j = 0; j < glcm[i].size(); ++j) {
                    local = glcm[i][j];
                    margprobx[i] += local;
                    margproby[j] += local;
                    probsum[i + j] += local;
                    probdiff[abs(i - j)] += local;
                }
            }
            hx = Entropy(margprobx);
            hy = Entropy(margproby);
            meanStd(margprobx, meanx, stddevx);
            meanStd(margproby, meany, stddevy);
            //Everything set up
            initial = true;
        }
        
        
        /*0 => energy, 1 => entropy, 2=> inverse difference */
        /*3 => correlation, 4=> info measure 1, 5 => info measure 2*/
        std::vector<double> cooc_feats() {
            std::vector<double> ans(6, 0.0);
            double hxy1 = 0.0;
            double hxy2 = 0.0;
            double local;
            for (int i = 0; i < glcm.size(); i++) {
                for (int j = 0; j < glcm[i].size(); j++) {
                    local = glcm[i][j];
                    ans[0] += local * local;
                    ans[1] += local * log(local + EPS_SMALL);
                    ans[2] += local * (1 / (1 + (i - j) * (i - j)));
                    ans[3] += (i * j * local) - (meanx * meany);
                    hxy1 += local * log(margprobx[i] * margproby[j] + EPS_SMALL);
                    hxy2 += margprobx[i] * margproby[j] * log(margprobx[i] * margproby[j] + EPS_SMALL);
                }
            }
            hxy1 = hxy1 * -1;
            hxy2 = hxy2 * -1;
            ans[1] = -1 * ans[1];
            ans[3] = ans[3] / (stddevx * stddevy);
            ans[4] = (ans[1] - hxy1) / std::max(hx, hy);
            ans[5] = sqrt(1 - exp(-2 *(hxy2 - ans[1])) + EPS_SMALL);
            return ans;
        }
        
        
        /*0 => contrast, 1 => diff entropy, 2 => diffvariance */
        /*3 => sum average, 4 => sum entropy, 5 => sum variance */
        std::vector<double> margprobs_feats() {
            std::vector<double> ans(6, 0.0);
            for (int i = 0; i < probdiff.size(); i++) {
                ans[0] += i * i * probdiff[i];
                ans[1] += -1 * probdiff[i] * log(probdiff[i] + EPS_SMALL);
            }
            for (int i = 0; i < probsum.size(); i++) {
                ans[3] += i * probsum[i];
                ans[4] += -1 * probsum[i] * log(probsum[i] + EPS_SMALL);
            }
            for (int i = 0; i < probdiff.size(); i++) 
                ans[2] += (i - ans[1]) * (i - ans[1]) * probdiff[i];
            for (int i = 0; i < probsum.size(); i++)
                ans[5] += (i - ans[4]) * (i - ans[4]) * probsum[i];
            return ans;               
        }
        
        public:
            std::vector<double> fast_feats(bool verbose=false) {
                std::vector<double> result(12, 0.0);          
                if (glcm.empty()) {
                    return result;
                }
                if (!initial)
                    fast_init();
                std::vector<double> margfeats = margprobs_feats();
                std::vector<double> coocfeats = cooc_feats();
                for (int i = 0; i < 6; ++i)
                    result[i] = coocfeats[i];
                for (int i = 0; i < 6; ++i)
                    result[6 + i] = margfeats[i];
                for(int i = 0; i< result.size(); ++i){
                    if(std::isnan(result[i]) || std::isinf(result[i])){
                        result[i] = 0;
                    }
                }
                return result;
            }
       
        std::vector<double> getFeaturesFromGLCM(std::vector<std::vector<NumericType>> glcm, bool verbose=false) {
            this->glcm = glcm;
            fast_init(); //initialize internal variables
            std::vector<double> ans = fast_feats();
            if (verbose) {
                std::cout << "Energy: " << ans[0] << std::endl;
                std::cout << "Entropy: " << ans[1] << std::endl;
                std::cout << "Inverse Difference Moment: " << ans[2] << std::endl;
                std::cout << "Correlation: " << ans[3] << std::endl;
                std::cout << "Info Measure of Correlation 1: " << ans[4] << std::endl;
                std::cout << "Info Measure of Correlation 2: " << ans[5] << std::endl;
                std::cout << "Contrast: " << ans[6] << std::endl;
                std::cout << "Difference Entropy: " << ans[7] << std::endl;
                std::cout << "Difference Variance: " << ans[8] << std::endl;
                std::cout << "Sum Average: " << ans[9] << std::endl;
                std::cout << "Sum Entropy: " << ans[10] << std::endl;
                std::cout << "Sum Variance: " << ans[11] << std::endl << std::endl;  
            }
            return ans;
        }
        
       //Constructor for use on single image

        Haralick(std::vector<std::vector<NumericType>> glcm) {
//            if (img.type() != CV_8UC1) {
//                cout << "Unsupported image type" << endl;
//                return;
//            }
            this->glcm = glcm;
        }

        //Constructor for use on various images
        Haralick() {
//            return;
        }
};
template <class NumericType>
std::vector<double> getHaralicks(std::vector<std::vector<NumericType>> glcm, bool printHar = false){
    Haralick<NumericType> har = Haralick<NumericType>();
    return har.getFeaturesFromGLCM(glcm,printHar);
};

#endif /* HARALICK_H */

