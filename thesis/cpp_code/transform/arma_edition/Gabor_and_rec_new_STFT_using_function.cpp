// This code is for STFT with gaussian window function (Gabor transform)
// Or STFT using Rectangular window function
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
#include "STFT_header/STFT.h"
// Remember to compile STFT.cpp as well
using namespace std;
using namespace arma;
using namespace sp;
int main(int argc, char* argv[]){

// decline the constant 
    const double pi = 3.1415926;
    const complex<double> i(0,1);
//---------------------------------------------------------
//Read Data to form x (using file only contain voltage data)
//User can input the data by command line, default is 9k_20k_3s
//---------------------------------------------------------
    vec in;
    vector<double> input_x;


//----------------------------------------------------------
//setup the parameters:
//For detecting whistle: dt=0.01 ,df=10,B=0.05 -> overlap 90% slower
//For detecting whistle: dt=0.01 ,df=20,B=0.02 -> overlap 75% faster
//For detecting   click: dt=0.005,df=50,B=0.01 -> overlap 75% fast
//----------------------------------------------------------
   
     double         dt   = 0.01;      // output time resolution
     double         df   = 20;        // output frequency resolution
        int        sgm   = 6000;      // scale gabor factor
     double          B   = 0.02;      // rectangular window length (seconds)
        int         fs   = 44100;     // sample rate
     double  threshold   = 30;        // threshold for detection
     double  do_dectect  = true;     // using detection algorithm to seperate data
     string  window_type = "rec";     // Window type: Gassian and rec
//----------------------------------------------------------

    if(argc!=5){
      cout<<"Usage: ./GaborSTFT_using_function filename dt df B"<<endl;
      cout<<"Calculate 9k to 20k 3 seconds for default"<<endl;  
      
      input_x = readfile("9k_20k_3s.txt");
    
    }
    else{
      cout<<"Calculate for "<<argv[1]<<endl;
      input_x = readfile(argv[1]);
      dt=atof(argv[2]);
      df=atof(argv[3]);
      B =atof(argv[4]);
    }
//form the data to arma::vec form
    vec x = zeros<vec>(input_x.size());
//for single channel: using i++
//for stereo channel: using i+=2
    for(int i=0;i<input_x.size();i++){   
       in<<input_x[i];
          x(i) = input_x[i];
    }

//time start
clock_t t_one;
t_one =clock();

    mat P;
// Using Gabor if want
    if(window_type == "Gassian")
       P   = Gabor(x,fs,sgm,dt,df);   // P: row is time and column is frequency
//Using recSTFT if want
    else if(window_type == "rec")
       P   = recSTFT_new(x,fs,B,dt,df);   // P: row is time and column is frequency
        
//put detection algorithm in
      if(do_dectect){
// set a threshold to catch the chirp data
        detect_whistle(P,threshold); 
      }
//time end
t_one = (clock()-t_one);
double total_time = (double)t_one/CLOCKS_PER_SEC;
cout<<"\nTotal time:"<<total_time<<" seconds"<<endl;

//using gp class to do the plotting
//------------------------------------------------------------

            stringstream ss_title;
            string title;
            ss_title<<"Spectrogram(dt="<<dt<<",df="<<df<<",B="<<B<<")";
            ss_title>>title;
            
 sp::gplot   gp;
 std::string command = "";   //gnuplot command can be type here
 gp.title(title.c_str());
 gp.ylabel("Frequency");
 gp.xlabel("Time");
// gp.xlim(0,T);
// gp.ylim(0,F);
 gp.send2gp(command.c_str());    //send pipe command to gnuplot
 gp.set_parula_palette();        // color style
 gp.image(P);                    // Just like imagesc of Matlab
//-------------------------------------------------------------
}
