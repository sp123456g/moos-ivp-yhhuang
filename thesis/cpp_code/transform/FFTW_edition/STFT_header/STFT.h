#ifndef  STFT_H 
#define  STFT_H
//***********************************************************************
//Author: Yen-Hsiang Huang
//Origin: National Taiwan University
//Date:   Apr.20.2018
//***********************************************************************
#include <iostream>
#include <armadillo>
#include <cmath>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <iomanip>

using namespace arma;
using namespace std;
//using namespace sp;

const float pi = 3.1415926;
const complex<float> i(0,1);


//Function list:
//1. mat STFT_WITH_FFTW3f(std::vector<float>, int fs)
//2. unsigned int frequency_mapping(unsigned int,int,int);
//3. float time_mapping(unsigned int,int,int);
//4. void detect_whistle(arma::mat &P,float threshold);
//5. void detect_click();
//-------------------------------------------------------------------------

mat STFT_with_FFTW3f(std::vector<float> x, int fs=96000,unsigned int N=2048,float overlap_percent=0.9, int win=1);
//Use fft in FFTW package, is faster than arma package
//------------------------------------------------------------------------
// output: matrix with x:time(an element stand for a dt), y:frequency(an element stand for a df)
// output: frequency element represent for the band, need frequency_mapping and time_mapping function to get excact value)
//               x: input, need to be single channel data in std::vector<float> form
//              fs: sample rate
//               N: window length 
// overlap_percent: overlap_percentage of overlap    
//             win: window type
//-------------------------------------------------------------------------

unsigned int frequency_mapping(unsigned int input_index,int fs,int N);
// input_index is the frequency index in spectrogram_mat which was output by STFT_with_FFTW3f()

float time_mapping(unsigned int input_index,int fs,int N,double overlap_percent);
// input_index is the time index in spectrogram_mat which was output by STFT_with_FFTW3f()

void detect_whistle(arma::mat &P,int fs=96000,unsigned int N=2048,float overlap=0.9);

void detect_click();
#endif
