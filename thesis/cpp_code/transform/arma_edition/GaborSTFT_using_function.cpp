// This code is for STFT with gaussian window function (Gabor transform)
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
#include "STFT.h"

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

    if(argc!=2){
      cout<<"Usage: ./GaborSTFT_using_function filename"<<endl;
      cout<<"Calculate 9k to 20k 3 seconds for default"<<endl;  
      
      input_x = readfile("9k_20k_3s.txt");
    }
    else{
      cout<<"Calculate for "<<argv[1]<<endl;
      input_x = readfile(argv[1]);
    }
//form the data to arma::vec form
    vec x = zeros<vec>(input_x.size());
//for single channel: using i++
//for stereo channel: using i+=2
    for(int i=0;i<input_x.size();i++){   
       in<<input_x[i];
          x(i) = input_x[i];
    }

//----------------------------------------------------------
//setup the parameters:
//----------------------------------------------------------
   
     double         dt  = 0.02;     // output time resolution 0.02 is good
     double         df  = 20;       // output frequency resolution  20 is good
        int        sgm  = 6000;     // scale gabor factor
     double          B  = 0.001;    // rectangular window length (seconds)
        int         fs  = 44100;    // sample rate
        int  threshold  = 30;       // threshold for detection
     double  do_dectect = true;     //using detection algorithm to seperate data

//----------------------------------------------------------

//time start
clock_t t_one;
t_one =clock();

// Using Gabor if want
//      mat P   = Gabor(x,fs,sgm,dt,df);   // P: row is time and column is frequency
//Using recSTFT if want
        mat P   = recSTFT_new(x,fs,B,dt,df);   // P: row is time and column is frequency
        
//put detection algorithm in
      if(do_dectect){
// set a threshold to catch the chirp data
        for(int n=0;n<P.n_cols;n++){
          for(int m=0;m<P.n_rows;m++){
            if(P(m,n)>threshold)
              P(m,n) = 1;
            else
              P(m,n) = 0;
          }
        }
      }
//time end
t_one = (clock()-t_one);
double total_time = (double)t_one/CLOCKS_PER_SEC;
cout<<"Total time:"<<total_time<<" seconds"<<endl;

//using gp class to do the plotting
//------------------------------------------------------------
 sp::gplot   gp;
 std::string command = "";   //gnuplot command can be type here
 gp.title("Spectrogram");
 gp.ylabel("Frequency");
 gp.xlabel("Time");
// gp.xlim(0,T);
// gp.ylim(0,F);
 gp.send2gp(command.c_str());    //send pipe command to gnuplot
 gp.set_parula_palette();        // color style
 gp.image(P);                    // Just like imagesc of Matlab
//-------------------------------------------------------------
}
