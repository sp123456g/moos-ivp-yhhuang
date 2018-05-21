// This code is for STFT with gaussian window function
//
// 1. getting input 
// 2. calculate parameter: 
//    (1) n0 first point of time input
//    (2) m0 first point of frequency output
//    (3) T  length of input time point
//    (4) F  length of frequency of the output freq range
//    (5) N=1/dt*df  need to be integer, due to digital
//    (6) Q =B/dt (digital bandwidth of window function)
// 3. decide window:
//    (1) window type 
//    (2) window length 
//    (3) change to digital window 
#include <iostream>
#include <armadillo>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <time.h>
#include "readfile.h"
#include "gplot.h"

using namespace std;
using namespace arma;

int main(){

// decline the constant 
    const double pi = 3.1415926;
    const complex<double> i(0,1);

//decide window length and window parameter decide
//------------------------------------------------------
    int    sgm  = 200;  // scale gabor factor
    int    fs   = 48000;   // sample rate
// ------------------------------------------------------
//Read Data to form x (using file only contain voltage data)
//-------------------------------------------------------
    vec x;
    vec in;

    vector<double> input_x;
    
    input_x = readfile("1kto10k.txt");
    
//for stereo channel
 //    for(int i=0;i<input_x.size();i+=2){   
 //       in<<input_x[i];
  //      x = join_vert(x,in);
   //  }
//for single channel
clock_t t_one;
t_one =clock();
    for(int i=0;i<input_x.size();i++){   
        in<<input_x[i];
        x = join_vert(x,in);
     }
//-------------------------------------------------------
              
//Calculate parameter   the only input: x,fs,sigma
//-------------------------------------------------------

    
     double dtau = 1/(double)fs;  // input sampling interval
     vec    tau  = regspace<vec>(0,1/(double)fs,x.n_elem/(double)fs); // digital input time 
     double  dt  = 0.05;     //output time resolution
     double  df  = 1;        //output frequency resolution  
     double   B  = 1.9141/sqrt(sgm);  // gaussian window bandwidth
     vec      t  = regspace<vec>(0,dt,max(tau));
     vec      f  = regspace<vec>(20,df,1000);
//---------------------------------------------------------       
    
     float n0 = t(0)/dt;
     float m0 = f(0)/dt;
     unsigned int T = t.n_elem;               // length of x
     unsigned int F = f.n_elem;               // length of output frequency series
     unsigned int N = round(1/(dtau*df));       // constraint of FFT base implimentation
     unsigned int C = tau.n_elem;               // digital input length
     unsigned int Q = round(B*fs);
     unsigned int S = dt/dtau;
// //-------------------------------------------------------
//window function
//-------------------------------------------------------
     vec window_time = regspace<vec>(dtau,dtau,Q*dtau);
     vec right_half_gaussian = exp(-sgm*pi*square(window_time));
     vec left_half_gaussian = flipud(right_half_gaussian);
     vec window = join_vert(right_half_gaussian,left_half_gaussian);
      
     vec zero= zeros<vec>(Q);
     vec addzerox = join_vert(zero,x);
     addzerox = join_vert(addzerox,zero);    //add zero in front and behind
//-------------------------------------------------------

//FFT loop for calculating
//-------------------------------------------------------
  cx_mat output(T,N);
  cx_mat X(C/S+1,N);    // row is time, column is frequency now
  mat    x_split(C/S+1,2*Q);
     for(int n=n0;n<n0+C;n+=S){   //C=fs+1, so size of x_split need to +1
 //step1 using window function to access input
//      vec x1q = zeros<vec>(2*Q);
       for(int q=0;q<2*Q;q++){
      //   x1q(q) = window.at(q)*addzerox.at(n+q); 
         x_split(n/S,q) = window.at(q)*addzerox.at(n+q);
       }
     }
// //step2 use x1q to do fft
      for(int i=n0;i<C/S+1;i++){
        X.row(i) = fft(x_split.row(i),N);
    
// //step3 modified amplitude of output 
//       for(int m=m0;m<m0+F;m++){
 //       output(n/S,m) = abs(X(n/S,m))*dt*exp(i*2.0*pi*(double)((double)Q-n)*(double)m/(double)N);
 //      }
     }
//Calculate the time it spend for calculate
//--------------------------------------------------------------------
t_one = (clock()-t_one);
double total_time = (double)t_one/CLOCKS_PER_SEC;
cout<<"Total time:"<<total_time<<" seconds"<<endl;
//--------------------------------------------------------------------
//
//using gp class to do the plotting

 mat P   = abs(X).t();   // P: row is time and column is frequency
 sp::gplot   gp;
 std::string command = "";
 gp.ylabel("Frequency");
 gp.xlabel("Time");

// gp.xlim(0,T);
// gp.ylim(0,F);
 gp.send2gp(command.c_str());    //send pipe command to gnuplot
 gp.set_parula_palette();        // color style
 gp.image(P);                    // Just like imagesc of Matlab

}

