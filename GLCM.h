/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GLCM.h
 * Author: lars
 *
 * Created on June 4, 2020, 2:52 PM
 */

#ifndef COOCCURENCEMATRIX_H
#define COOCCURENCEMATRIX_H

template <class NumericType>
class GLCM{
    NumericType maxValue;
    NumericType minValue;
    int newMax;
    int d;
    int deltaX;
    int deltaY;
    int inputXRange;
    int inputYRange;
    bool isTorus;
    std::vector<std::vector<double>> glcm;
    
    public:
        /**
         * Generic Gray-Level-Co-Occurence Matrix with every Numeric type possible
         * @param inMatrix: typ -> NumericTyp           := Values of 2-Dim-Matrix
         * @param maxValue: NumericType   def=0              := The maximum value of the input 2-Dim-Matrix, in a GLCM, every VALUE of an 2-Dim Matrix will be
                                                            an index for the occurence of each other VALUE. If you chose a floating point number, 
         *                                                  you should absolutely define the value range (newMax)!
         * @param newMax:   int         def=255      := New maximum value, compress the value range of the GLCM 
         *                                                  (i.e. image with 16 bit gray level to 8 bit gray level)
         * @param d:        int         def=1        := distance to look for neighborhood relationship 
         *                                                  (i.e. 1 = NEXT entry in Matrix, 2= leapfrog one entry, etc.)
         * @param rot:      int         def=0        := one of 8 possibilities 
         *                                                  (i.e. 0-3 should be sufficient) 0-> 0°, 1->45°, 2->90°,3->135°
         * @param isTorus:  bool        def=1        := interpret input Matrix as Torus: 
         *                                                  (pros: rotation+180 are transposed Matrix and therefor irrelevant; 
         *                                                   cons: 2-Dim-Matrix (img) is usually not a torus)
         * @param normAllRots: bool     def=true     := calculate all 4 necessary rotations and create a combined normalized GLCM
         */
        GLCM(std::vector<std::vector<NumericType>> inMatrix, NumericType maxValue = 0, NumericType newMax = 255,
                int d = 1, int rot = 0, bool isTorus = true, bool normAllRots = true){
//            static_assert(std::is_arithmetic<NumericType>::value, "NumericType must be numeric");
         // TODO: Write a proper Unit test for GLCM  
         // Problem (maybe) -> newMax is not probably assigned for floating values
         // use a set to get all distinct values of a spectrogram/matrix
         // 
         // Features of new GLCM version differ from old version!!
         //
         // Systematic error found: When input is float, maxValue and newMax is absolute stupid
         //     It needs a input set for maxValue and newMax -> (minValue, stepsize, maxValue)
         //     This would yield 2^16 entries for a 16 bit wav file!
         //     It could possibly just be calculated with: (minValue + i * stepsize) with i as index for a regular array
         //                                                 round(value/stepsize) - minValue -> i
         //                                                 compressed(
            
            this->maxValue = getVectorMax(inMatrix);
            this->minValue = getVectorMin(inMatrix);
            
            this->newMax = newMax;
   
            this->d = d;
            assignRotation(rot);
            this->isTorus = isTorus;
            
            std::vector<std::vector<double>> tmp(this->newMax+1 , std::vector<double> (this->newMax+1, 0.0)); 
            this->glcm = tmp;
            
            this->inputYRange = inMatrix.size();
            this->inputXRange = inMatrix[0].size();
            
            if(this->maxValue == this->minValue){
                this->glcm = {{0.0}};
            }else{
                if(normAllRots == false){
                    populateGLCM(inMatrix);
                }else{
                    //calculates all rotations and add them normalized together
                    for(int i = 0; i < 4; ++i){
                        this->d = d;
                        assignRotation(i);
                        populateGLCM(inMatrix);
                        tmp = getVectorAdd(tmp, this->glcm, 3.0);
                    }
                    this->glcm = tmp;
                    freeVector(tmp);
                }
            }
            
        }
        
        ~GLCM(){};
        
        void printOutGLCM(){
            for(int y=0; y<this->glcm.size(); ++y){
                for(int x=0; x<this->glcm[y].size(); ++x){
                    std::cout << std::fixed << std::setprecision(3) << this->glcm[y][x] << " ";
                }
                std::cout << std::endl;
            }
        }
        
        std::vector<std::vector<double>> getGLCM(){
            return this->glcm;
        }
       
        
        
    private:
        
        void assignRotation(const int & rot){
            switch (rot){
                case 0:
                    this->deltaX = d;
                    this->deltaY = 0;
                    break;
                case 1:
                    this->deltaX = d;
                    this->deltaY = -d;
                    break;
                case 2:
                    this->deltaX = 0;
                    this->deltaY = -d;
                    break;
                case 3:
                    this->deltaX = -d;
                    this->deltaY = -d;
                    break;
                case 4:
                    this->deltaX = -d;
                    this->deltaY = 0;
                    break;
                case 5:
                    this->deltaX = -d;
                    this->deltaY = d;
                    break;
                case 6:
                    this->deltaX = 0;
                    this->deltaY = d;
                    break;
                case 7:
                    this->deltaX = d;
                    this->deltaY = d;
                    break;
                default:
                    this->deltaX = d;
                    this->deltaY = 0;
            }
        }
        
        /**
         * 
         * @param value: NumericType  := Value to set in new range in whole numbers
         * @return     : int          := new value
         */
        int getCompressedIndex(NumericType value){
            if(maxValue - minValue == 0){return 0;}
            return   static_cast<int>(std::max(std::min(static_cast<double>(std::round((static_cast<double>(value)-static_cast<double>(minValue))/(maxValue - minValue))* static_cast<double>(newMax))
                                        ,std::round(static_cast<double>(newMax)))
                              ,0.0));
          
//            return (int)std::round((value/((double)this->maxValue))*this->newMax);
        }
        
        void populateGLCM(std::vector<std::vector<NumericType>>& inMatrix){
            for(int inYi =0; inYi < inMatrix.size(); ++inYi){
                for(int inXi =0; inXi < inMatrix[inYi].size(); ++inXi){
                    int dx = inXi +deltaX;
                    int dy = inYi +deltaY;
                    
                    // index boundary check
                    if(this->isTorus){
                        reassignTorusIndex(dx,dy);
                    }else{
                        if(isXOutOfBounds(dx)){
                            continue;
                        }
                        if(isYOutOfBounds(dy)){
                            break;
                            continue;
                        }
                    }
                    int glcmY = getCompressedIndex(inMatrix[inYi][inXi]);
                    int glcmX = getCompressedIndex(inMatrix[dy][dx]);
                    ++this->glcm[glcmY][glcmX];
                }
            }
            getNormalizedGLCM();
        }
              
        bool isXOutOfBounds(const int & xi){
            return (xi <0 || xi >= inputXRange);
        }
        
        bool isYOutOfBounds(const int & yi){
            return (yi <0 || yi >= inputYRange);
        }
        

        /**
         * Interpret input matrix as Torus
         * @param xi: int   := reassign x_index if out of bounds
         * @param yi: int   := reassign y_index if out of bounds
         */
        inline void reassignTorusIndex(int & xi, int & yi){
            xi = posModulo(xi, this->inputXRange);
            yi = posModulo(yi, this->inputYRange);

        }
        
         /**
         * 
         * normalization := sum of all entries in matrix equals 1
         *                  therefor divide all entries by total amount of additions (thats equal to the size of the input matrix)
         */
        void getNormalizedGLCM(){
            for(int i=0; i<glcm.size(); ++i){
                for(int j=0; j<glcm.size(); ++j){
                    glcm[i][j]  = glcm[i][j] / (inputYRange*inputXRange*1.0);
                }
            }
        }

    
};

#endif /* COOCCURENCEMATRIX_H */

