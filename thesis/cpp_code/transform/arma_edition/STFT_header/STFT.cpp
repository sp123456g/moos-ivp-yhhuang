#ifndef  STFT_H 
#define  STFT_H
//***********************************************************************
//Author: Yen-Hsiang Huang(yhhuang)
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
#include "STFT.h"
using namespace arma;
using namespace std;

const double pi = 3.1415926;
const complex<double> i(0,1);

//Function list:
//1. mat recSTFT(arma::vec x, arma::vec t, arma::vec f, int B)
//2. mat Gabor(arma::vec x, int fs, int sigma, double dt, double df)
//3. mat recSTFT_new(arma::vec x, int fs, int B, double dt, double df)


mat recSTFT(arma::vec x, arma::vec t, arma::vec f, int B){
//-------------------------------------------------------
// Short time fourier transform using retangular function
// output: matrix with x:time and y:frequency
//      x: input
//      t: output time length
//      f: output frequency length
//      B: analog bandwidth
//-------------------------------------------------------

//1.Calculate parameter
//-------------------------------------------------------
    double dt = t(1)-t(0); 
    double df = f(1)-f(0); 
    float n0 = t(0)/dt;
    float m0 = f(0)/dt;
    unsigned int T = t.n_elem;               // length of x
    unsigned int F = f.n_elem;               // length of output frequency series
    unsigned int N = round(1/(dt*df));       // constraint of FFT base implimentation
//-------------------------------------------------------
    
//2.window function decide
//-------------------------------------------------------
    unsigned int Q = B/dt;
    vec window = ones<vec>(2*Q);    // rectangular window with length 1 sec
    vec zero= zeros<vec>(Q);
    vec addzerox = join_vert(zero,x);
    addzerox = join_vert(addzerox,zero);    //add zero in front and behind
//-------------------------------------------------------
//3.FFT loop for calculating (4 steps)
//-------------------------------------------------------
 cx_mat X_mod(T,N);
 cx_mat X(T,N);    // row is time, column is frequency now
 mat output(T,N);
     for(int n=n0;n<n0+T;n++){
//step1 using window function to access input
     vec x1q = zeros<vec>(2*Q);
    
      for(int q=0;q<2*Q;q++){
        x1q(q) = window.at(q)*addzerox.at(n+q); 
      }
//step2 use x1q to do fft
       X.row(n) = fft(x1q,N).t();   //after fft is column vector
    
//step3 modified amplitude of output 
      for(int m=m0;m<m0+F;m++){
        X_mod(n,m) = abs(X(n,m))*dt*exp(i*2.0*pi*(double)((double)Q-n)*(double)m/(double)N);
      }
    }
//step4 Let complex be scalar and transpose 
//output: row is frequency and column is time, just means x is time and y is frequency
    output = abs(X_mod).t();
    return(output);
}

mat Gabor(arma::vec x, int fs, int sgm=200, double dt=0.01, double df=1){
//------------------------------------------------------------------------
// output: matrix with x:time(an element stand for a dt), y:frequency(an element stand for a df)
// output: frequency element represent for the band, ex: index 10 is 90~100 for case dt=10)
//      x: input, need to be single channel data in arma::vec form
//     fs: sample rate
//    sgm: sigma, for scale gabor transform
//     dt: output time resolution
//     df: output frequency resolution
//-------------------------------------------------------------------------
     double dtau = 1/(double)fs;  // input sampling interval
     vec    tau  = regspace<vec>(0,1/(double)fs,x.n_elem/(double)fs); // digital input time 
     double   B  = 1.9141/sqrt(sgm);  // gaussian window bandwidth
     vec      t  = regspace<vec>(0,dt,max(tau));
//     vec      f  = regspace<vec>(20,df,1000);
//-------------------------------------------------------------------------       
     float n0 = t(0)/dt;                      // first point of t vector 
//     float m0 = f(0)/dt;                      // second point of f vector
     unsigned int T = t.n_elem;               // length of x
//     unsigned int F = f.n_elem;               // length of output frequency series
     unsigned int N = round(1/(dtau*df));     // constraint of FFT base implimentation
     unsigned int C = tau.n_elem;             // digital input length
     unsigned int Q = round(B*fs);            // digital bandwidth
     unsigned int S = dt/dtau;                // unbalance form factor
//-------------------------------------------------------------------------
//window function
//-------------------------------------------------------------------------
     vec window_time = regspace<vec>(dtau,dtau,Q*dtau);
     vec right_half_gaussian = exp(-sgm*pi*square(window_time));
     vec left_half_gaussian = flipud(right_half_gaussian);
     vec window = join_vert(right_half_gaussian,left_half_gaussian);
      
     vec zero= zeros<vec>(Q);
     vec addzerox = join_vert(zero,x);
     addzerox = join_vert(addzerox,zero);    //add zero in front and behind
//-------------------------------------------------------------------------
//FFT loop for calculating using unbalance form
//-------------------------------------------------------------------------
  cx_mat X(T,N);    // row is time, column is frequency now
  cout<<"2*Q="<<2*Q<<",N="<<N<<endl;

  
  if(2*Q>N)
      cout<<"Warning: N must greater than 2*Q+1, you may want to change sgm, df,dt"<<endl;

     for(int n=n0;n<n0+C;n+=S){   
 //step1 using window function to access input
      vec x1q = zeros<vec>(2*Q);
       for(int q=0;q<2*Q;q++){
        x1q(q) = window.at(q)*addzerox.at(n+q); 
       }
 //step2 use x1q to do fft
        X.row(n/S) = fft(x1q,N).t();
     }

//step3 Let complex be scalar and transpose
//and mapping the frequency to let the output resolution be df=1    
//ex: if df=10, the first frequency element is frequency 1~10 Hz, 4410 is 44090~44100
    mat output(N*df,T);
    mat absX = abs(X).t();
    for(int k=0;k<absX.n_rows*df;k++){
        output.row(k) = absX.row((int)(k/df)); 
    }
        output = output.submat(0,0,fs/2-1,T-1);
//output: row is frequency and column is time, just means x is time and y is frequency
    return(output);
}
//-------------------------------------------------------------------------

mat recSTFT_new(arma::vec x, int fs=44100, double B=0.02, double dt=0.01, double df=20){
//------------------------------------------------------------------------
// output: matrix with x:time(an element stand for a dt), y:frequency(an element stand for a df)
// output: frequency element represent for the band, ex: index 10 is 90~100 for case dt=10)
//      x: input, need to be single channel data in arma::vec form
//     fs: sample rate 
//      B: window length (sec)
//     dt: output time resolution
//     df: output frequency resolution
//-------------------------------------------------------------------------
     double dtau = 1/(double)fs;  // input sampling interval
     vec    tau  = regspace<vec>(0,1/(double)fs,x.n_elem/(double)fs); // digital input time
     vec      t  = regspace<vec>(0,dt,max(tau));
//     vec      f  = regspace<vec>(20,df,1000);
//-------------------------------------------------------------------------       
     float n0 = t(0)/dt;                      // first point of t vector 
//     float m0 = f(0)/dt;                      // second point of f vector
     unsigned int T = t.n_elem;               // length of x
//     unsigned int F = f.n_elem;               // length of output frequency series
     unsigned int N = round(1/(dtau*df));     // constraint of FFT base implimentation
     unsigned int C = tau.n_elem;             // digital input length
     unsigned int S = round(dt/dtau);                // unbalance form factor
     unsigned int Q = round(B*fs);
     double       Overlap =1-(double)S/(2*Q);
//-------------------------------------------------------------------------
//window function
//-------------------------------------------------------------------------
 
    vec window = ones<vec>(2*Q);    // rectangular window with length 1 sec
    vec zero= zeros<vec>(Q);
    vec addzerox = join_vert(zero,x);
    addzerox = join_vert(addzerox,zero);    //add zero in front and behind
//-------------------------------------------------------------------------
//FFT loop for calculating using unbalance form
//-------------------------------------------------------------------------
//  cx_mat X(T,N);    // row is time, column is frequency now
  cx_mat X(T,N);    // row is time, column is frequency now
  cout<<"Window length  2*Q ="<<2*Q<<endl;
  cout<<"fft samples      N ="<<N<<endl;
  cout<<"Unbalance factor S ="<<S<<endl;
  cout<<"Overlap:            "<<Overlap*100<<"%"<<endl; 
//Constrain cheching 
  if(2*Q>N)
      cout<<"\nWarning: N must greater than 2*Q+1, you may want to change B, df,dt"<<endl;
  if(Overlap<0)
     cout<<"\nWarning: Change B to let Overlap>=0%"<<endl;

// Calculate spectrogram  
     for(int n=n0;n<n0+C;n+=S){  
 //step1 using window function to access input
      vec x1q = zeros<vec>(2*Q);
       for(int q=0;q<2*Q;q++){
        x1q(q) = window.at(q)*addzerox.at(n+q); 
       }
 //step2 use x1q to do fft
        X.row(n/S) = fft(x1q,N).t();
     }

//step3 Let complex be scalar and transpose
//and mapping the frequency to let the output resolution be df=1    
//ex: if df=10, the first frequency element is frequency 1~10 Hz, 4410 is 44090~44100
    mat output(N*df,T);
    mat absX = abs(X).t();
    for(int k=0;k<absX.n_rows*df;k++){
        output.row(k) = absX.row((int)(k/df)); 
    }
        output = output.submat(0,0,fs/2-1,T-1);
//output: row is frequency and column is time, just means x is time and y is frequency
    return(output);
}

// detect_whistle algorithm
void detect_whistle(arma::mat &P, double threshold){
        
    for(int n=0;n<P.n_cols;n++){
        for(int m=0;m<P.n_rows;m++){
            if(P(m,n)>threshold)
                P(m,n) = 1;
            else
                P(m,n) = 0;
        }
    }
}
#endif
