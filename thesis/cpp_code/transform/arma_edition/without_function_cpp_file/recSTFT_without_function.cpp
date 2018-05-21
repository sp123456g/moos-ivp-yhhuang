// This code is for STFT with rectangular window function
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
#include "STFT.h"
using namespace std;
using namespace arma;


int main(){

// decline the constant 
    const double pi = 3.1415926;
    const complex<double> i(0,1);

//decide window length and window parameter decide
//------------------------------------------------------
    int B = 1;  // 1 sec  
    
// simulated input
// ------------------------------------------------------
    double dt = 0.05;
    double df = 0.05;
    
    vec t1 = regspace<vec>(0,dt,10);
    vec t2 = regspace<vec>(10,dt,20);
    vec t3 = regspace<vec>(20,dt,30);
    vec t  = regspace<vec>(0,dt,30);  //upper lower bound need to be double
    vec f  = regspace<vec>(0,df,5);    // f is decide by user 
   
    
    vec a  = cos(2*pi*t1);
    vec b  = cos(6*pi*t2);
    vec x  = join_vert(cos(2*pi*t1),cos(6*pi*t2));
    
    x = join_vert(x,cos(4*pi*t3));     // x is input
//-------------------------------------------------------

//Calculate parameter
//-------------------------------------------------------

    float n0 = t(0)/dt;
    float m0 = f(0)/dt;
    unsigned int T = t.n_elem;               // length of x
    unsigned int F = f.n_elem;               // length of output frequency series
    unsigned int N = round(1/(dt*df));       // constraint of FFT base implimentation
//-------------------------------------------------------
    
//window function decide
//-------------------------------------------------------
    unsigned int Q = B/dt;
    vec window = ones<vec>(2*Q);    // rectangular window with length 1 sec
    vec zero= zeros<vec>(Q);
    vec addzerox = join_vert(zero,x);
    addzerox = join_vert(addzerox,zero);    //add zero in front and behind
//-------------------------------------------------------

//FFT loop for calculating
//-------------------------------------------------------
 cx_mat output(T,N);
 cx_mat X(T,N);    // row is time, column is frequency now
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
        output(n,m) = abs(X(n,m))*dt*exp(i*2.0*pi*(double)((double)Q-n)*(double)m/(double)N);
      }
    }

//using gp class to do the plotting

    mat P   = abs(output).t();   // P: row is time and column is frequency
    sp::gplot   gp;
    std::string command = "";
    gp.ylabel("Frequency");
    gp.xlabel("Time");

    gp.xlim(0,T);
    gp.ylim(0,F);
    gp.send2gp(command.c_str());    //send pipe command to gnuplot
    gp.set_parula_palette();        // color style
    gp.image(P);                    // Just like imagesc of Matlab

}
