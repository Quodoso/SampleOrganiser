/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FACM.h
 * Author: lars
 *
 * Created on June 5, 2020, 12:03 PM
 */

#ifndef FACM_H
#define FACM_H


template <class NumericType>
class FACM{
    int maxValue;
    int newMax;
    int y_Range;
    NumericType threshold;
    int deltaX;
    int inputXRange;
    int inputYRange;
    bool isTorus;
    std::vector<std::vector<double>> facmPar;
    std::vector<std::vector<double>> facmSeq;
    std::vector<std::vector<double>> facmComb;
    std::vector<double> sumFacms{0.0,0.0,0.0};
    bool normalize = true;
    
    public:
        /*
           * Generic Frequency-Amplitude-Co-Occurence Matrix with every Numeric type possible
           * 
           * params: T        typ -> NumericTyp   := Values of 2-Dim-Matrix (i.e. of spectrogram. f: time, freq -> amplitude)
           *         int      maxValue            := For FACMS maxValue should be the amount of FFT-Bins (i.e. 4096, 2048, etc.)
           *         int      newMax              := New maximum value, compress the value range of FACM (i.e. newMax:= 512, dimension reduction from 4096 -> 512 )
           *         int      y_Range             := distance range to look for neighborhood relationship in one column
         *                                        := (i.e. 1 = both adjacent neighbors, 2 = both adjacent neighbors + adjacent neighbors of neighbors, etc) maximum-> maxValue/2
         *           int      seqDist             := Distance for calculating the sequential neighbor relationship.
         *                                           This should vary depending on the HopSize and/or Window size of a STDFT (default = 1)
           *         T        threshold           := only consider values above threshold

           *         bool     isTorus             := interpret input Matrix as Torus
         *           bool     normalize           := normalize resulting matrices. Normalize in that sense that the sum of all entries is equal to 1.
           * 
           * 
           */  
       FACM(std::vector<std::vector<NumericType> > inMatrix, int newMax=-1, int y_Range=-1, int seqDist = 1, NumericType threshold=0.0, bool isTorus=true, bool normalize = true){
//               static_assert(std::is_arithmetic<NumericType>::value, "NumericType must be numeric");
           
               this->maxValue = inMatrix[0].size();
               if(newMax == -1){
                   this->newMax = maxValue;
               }else{
                   this->newMax = newMax;
               }
               if(y_Range == -1){
                   this->y_Range = this->newMax;
               }else{
                   this->y_Range = y_Range;
               }
               this->isTorus = isTorus;
               this->threshold = threshold;
               this->normalize = normalize;
               this->deltaX = seqDist;
               
               this->facmPar = std::vector<std::vector<double>>(this->newMax , std::vector<double> (this->newMax, 0.0));
               this->facmSeq = std::vector<std::vector<double>>(this->newMax , std::vector<double> (this->newMax, 0.0));
               this->facmComb = std::vector<std::vector<double>>(this->newMax , std::vector<double> (this->newMax, 0.0));
               
               
               this->inputYRange = inMatrix[0].size();
               this->inputXRange = inMatrix.size();
               
               populateFACMs(inMatrix);

       }
       /*
        * FACM-Selector. 0-> Parallel, 1-> Post, 2->Combined
        */
       void printFACM(int which){
           std::vector<std::vector<double>> print;
            switch(which%3){
                case 0: print= this->facmPar; break;
                case 1: print= this->facmSeq;break;
                case 2: print =this->facmComb;break;
                default: print=this->facmComb;break;
            }
            for(size_t y=0; y<print.size(); ++y){
                for(size_t x=0; x<print[y].size(); ++x){
                    std::cout << std::fixed << std::setprecision(3) << print[y][x] << " ";
                }
                std::cout << std::endl;
            }

       }
       
       std::vector<std::vector<double>>  getFACM(int which){
            switch(which%3){
                case 0: return this->facmPar;
                case 1: return this->facmSeq;
                case 2: return this->facmComb;
                default: return this->facmComb;
            }
       }
       
    private:
        
        /**
         * normalization    := sum of all entries in matrix is equals to 1
         *                  
         * @param facm      := one of three possibilities 
         * @param normFactor:= factor to dive
         */
        void normalizeFACMs(std::vector<std::vector<double>> & facm, double & normFactor){
            for(size_t i=0; i<facm.size(); ++i){
                for(size_t j=0; j<facm[i].size(); ++j){
                    facm[i][j] = facm[i][j] / normFactor;
                }
            }
        }
        /*
         * compress the FACM-Size (i.e. 4096 bins to 512)
         * Must have range from 0 to n. No negative values allowed
        */
        inline int getCompressedValue(NumericType value){

            return (int)std::round((value/static_cast<double>(this->maxValue-1))*(this->newMax-1));
        }

        /*
         * compress value-range of input matrix (amplitudes of spectrogram)
         */
        inline double getNewValue(NumericType value){
            return value;
//                return (value/((double)this->maxValue))*this->newMax;
        }

        /*
         * compress value-range of input matrix (amplitudes of spectrogram)
         */
        inline double getNewValue(NumericType value1, NumericType value2){
            return static_cast<double>(reassignWithThreshold(value1))*reassignWithThreshold(value2);
//                return (value/((double)this->maxValue))*this->newMax;
        }

        bool isXOutOfBounds(const int & xi){
            return (xi < 0 || xi >= inputXRange);
        }

        bool isYOutOfBounds(const int & yi){
            return (yi < 0 || yi >= inputYRange);
        }


        /*
         * Interpret Input Matrix as Torus
         */
        inline void reassignTorusIndex(int & value, int & range){
             value = (range + (value%range)) % range;
        }

        inline NumericType reassignWithThreshold(NumericType & value){
            if(value >= this->threshold){
                return value;
            }
            return 0;
        }

        /**
         * get all y-indexes which will populate the FACM
         * this includes boundary check and torus option
         * 
         * @param index := current Frequency to check its neighbours
         * @param max   := the max number of frequency bins
         * @param range := the neighbour range
         * @return 
         */
        std::set<int> getYDistanceRangeList(const int & index, const int & max, int range){
            range = std::fmin(max-1,range);

            std::set<int> distanceRange;

            for(int di = index-range; di<=index+range; ++di ){
                int addIndex  = di;
                if(this->isTorus){
                    addIndex = (max + (addIndex%max)) % max;
                }
                addIndex = (int)std::fmax(0,addIndex);
                distanceRange.insert((int)std::fmin(max-1,addIndex));
            }
            return distanceRange;

        }

        /* 
         * Populate all 3 FACMs
         */
        void populateFACMs(std::vector<std::vector<NumericType>> & inMatrix){
            for(int inYi =0; inYi < this->inputYRange; ++inYi){
                std::set<int> yIndexList = getYDistanceRangeList(inYi, this->inputYRange, this->y_Range); //list of FFT-Bin-indexes
                int compressed_yi = getCompressedValue(inYi);   //compress FFT-Bins
                for(size_t inXi =0; inXi < inMatrix.size(); ++inXi){
                                        
                    int xPost = inXi + this->deltaX;  // for sequential indexing
                       
                    //this is the compressed FFT-Bin row
                    //interpret input matrix (spectrogram) as torus
                    if(this->isTorus){
                        reassignTorusIndex(xPost,this->inputXRange);
                    }

                    //calculate all 3 FACMs
                    //each entry
                    for(int facmY : yIndexList){
                        int compressed_xi = getCompressedValue(facmY);
//                        printf("inYi:%d, compYI:%d, yindexE:%d, :compYIndex:%d\n",inYi, compressed_yi, facmY, compressed_xi);

                        //parallel neighbors
                        double tmpValue = getNewValue(inMatrix[inXi][facmY],inMatrix[inXi][inYi]);
                        this->facmPar[compressed_yi][compressed_xi] += tmpValue;
                        this->sumFacms[0] += tmpValue;
                        this->facmComb[compressed_yi][compressed_xi] += tmpValue;
                        this->sumFacms[2] += tmpValue;
                        
                        //sequential neighbors
                        if(!isXOutOfBounds(xPost)){
                            tmpValue = getNewValue(inMatrix[xPost][facmY],inMatrix[inXi][inYi]);
                            this->facmSeq[compressed_yi][compressed_xi] += tmpValue;
                            this->sumFacms[1] += tmpValue;
                            this->facmComb[compressed_yi][compressed_xi] += tmpValue;
                            this->sumFacms[2] += tmpValue;
                        }
                    }
                }
            }
            if(normalize){
                normalizeFACMs(this->facmPar, this->sumFacms[0]);
                normalizeFACMs(this->facmSeq, this->sumFacms[1]);
                normalizeFACMs(this->facmComb, this->sumFacms[2]);
            }
        }
};
/**
 * calculates the Frequency-adjacence-coocurence matrix of a matrix (i.e. spectrogram)
 * 
 * @param spectrogram := 2D-Matrix (nested vector) of any NumericType. Should contain a spectrogram
 * @return  FACM matrix
 */    
template <class NumericType>
std::vector<std::vector<double>> getFACM( std::vector<std::vector<NumericType>> spectrogram){
    if(spectrogram.size() > 0 && spectrogram[0].size() > 0){
        return FACM<NumericType>(spectrogram, spectrogram[0].size(), spectrogram[0].size()).getFACM(2);
    }
    return std::vector<std::vector<double>>{{0}};
}


#endif /* FACM_H */

