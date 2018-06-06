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

const double pi = 3.1415926;
const complex<double> i(0,1);

//Function list:
//1. mat recSTFT(arma::vec x, arma::vec t, arma::vec f, int B)
//2. mat Gabor(arma::vec x, int fs, int sigma, double dt, double df)
//3. mat recSTFT_new(arma::vec x, int fs, int B, double dt, double df)


mat recSTFT(arma::vec x, arma::vec t, arma::vec f, int B);
//-------------------------------------------------------
// Short time fourier transform using retangular function
// output: matrix with x:time and y:frequency
//      x: input
//      t: output time length
//      f: output frequency length
//      B: analog bandwidth
//-------------------------------------------------------

mat Gabor(arma::vec x, int fs, int sgm=200, double dt=0.01, double df=1);
//------------------------------------------------------------------------
// output: matrix with x:time(an element stand for a dt), y:frequency(an element stand for a df)
// output: frequency element represent for the band, ex: index 10 is 90~100 for case dt=10)
//      x: input, need to be single channel data in arma::vec form
//     fs: sample rate
//    sgm: sigma, for scale gabor transform
//     dt: output time resolution
//     df: output frequency resolution
//-------------------------------------------------------------------------

mat recSTFT_new(arma::vec x, int fs, double B, double dt=0.01, double df=1);
//------------------------------------------------------------------------
// output: matrix with x:time(an element stand for a dt), y:frequency(an element stand for a df)
// output: frequency element represent for the band, ex: index 10 is 90~100 for case dt=10)
//      x: input, need to be single channel data in arma::vec form
//     fs: sample rate
//      B: window length (sec)
//     dt: output time resolution
//     df: output frequency resolution
//-------------------------------------------------------------------------

void detect_whistle(arma::mat &P,double threshold);
void detect_click();
#endif
