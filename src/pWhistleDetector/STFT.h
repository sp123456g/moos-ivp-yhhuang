#ifndef  STFT_H 
#define  STFT_H
//***********************************************************************
//Author: Yen-Hsiang Huang
//Origin: National Taiwan University
//Date:   Aug 17th 2018
//***********************************************************************
#include <iostream>
#include <armadillo>
#include <cmath>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <deque>
#include <iomanip>


const float pi = 3.1415926;
const std::complex<float> i(0,1);


//Function list:
//1. mat STFT_WITH_FFTW3f(std::vector<float>, int fs)
//2. unsigned int frequency_mapping(unsigned int,int,int);
//3. float time_mapping(unsigned int,int,int);
//4. void detect_whistle(arma::mat &P,float threshold);
//5. void detect_click();
//-------------------------------------------------------------------------

arma::mat STFT_with_FFTW3f(float* x, int fs=96000,unsigned int N=2048,float overlap_percent=0.9, int win=1, int data_size=96000);
//Use fft in FFTW package, is faster than arma package
//------------------------------------------------------------------------
// output: matrix with x:time(an element stand for a dt), y:frequency(an element stand for a df)
// output: frequency element represent for the band, need frequency_mapping and time_mapping function to get excact value)
//               x: input, need to be single channel data in std::vector<float> form
//              fs: sample rate
//               N: window length 
// overlap_percent: overlap_percentage of overlap    
//             win: window type, 0 is rectangular 1 is hanning
//-------------------------------------------------------------------------

unsigned int frequency_mapping(unsigned int input_index,int fs,int N);
// input_index is the frequency index in spectrogram_mat which was output by STFT_with_FFTW3f()

float time_mapping(unsigned int input_index,int fs,int N,float overlap_percent);
// input_index is the time index in spectrogram_mat which was output by STFT_with_FFTW3f()

struct xy_index{
    
    int x;
    int y;
};

class whistle{
    public:
        xy_index start_node;
        xy_index end_node;
    
        int     fs;
        int     N;
        float   overlap;

        float duration;
        unsigned int start_frq;
        unsigned int end_frq;

        void calculate(){
            duration = time_mapping(end_node.x,fs,N,overlap)-time_mapping(start_node.x,fs,N,overlap);
            start_frq = frequency_mapping(start_node.y,fs,N);
            end_frq   = frequency_mapping(end_node.y,fs,N);

        }
};


void detect_whistle(arma::mat &P,int fs,unsigned int N,float overlap);

std::vector<whistle> check_result(arma::mat P,int fs, unsigned int N,float overlap);
#endif
