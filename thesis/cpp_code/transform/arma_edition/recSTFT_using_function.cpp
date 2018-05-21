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

//Using recSTFT function 
//-------------------------------------------------------


    mat P = recSTFT(x,t,f,B);  //output: x is time and y is frequency  

//using gp class to do the plotting

    sp::gplot   gp;
    std::string command = "";
    gp.ylabel("Frequency");
    gp.xlabel("Time");

    gp.xlim(0,t.n_elem);
    gp.ylim(0,f.n_elem);
//    gp.send2gp(command.c_str());    //send pipe command to gnuplot
    gp.set_parula_palette();        // color style
    gp.image(P);                    // Just like imagesc of Matlab

}
