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
#include "STFT_header/STFT.h"
// Remember to compile STFT.cpp as well
using namespace std;
using namespace arma;
int main(int argc, char* argv[]){

// declare the constant 
    const float pi = 3.1415926;
    const complex<float> i(0,1);
//---------------------------------------------------------
//Read Data to form x (using file only contain voltage data)
//User can input the data by command line, default is 9k_20k_3s
//---------------------------------------------------------
    vec in;
    vector<float> input;
                   input.clear();
    
//----------------------------------------------------------
//setup the parameters:
//For detecting whistle: dt=0.01 ,df=10,B=0.05 -> overlap 90% slower
//For detecting whistle: dt=0.01 ,df=20,B=0.02 -> overlap 75% faster
//For detecting   click: dt=0.005,df=50,B=0.01 -> overlap 75% fast
//----------------------------------------------------------
   
     float          B;      // rectangular window length (seconds)
        int         fs   = 96000;     // sample rate
        int          N   = 2048;
     float    overlap   = 0.9;
     bool    do_detect   = true;      //using detection algorithm to seperate data
        int  window_type = 1;         //Window type: 0:rectangular 1:Hanning
        int  channel     = 1;
//----------------------------------------------------------

    if(argc!=4){
      cout<<"Usage: ./GaborSTFT_using_function filename do_detect channel_number"<<endl;
      cout<<"Calculate chirp with noise and do detect for default"<<endl;  
      
      input = readfile("chirp_with_noise.txt");
    
    }
    else{
      cout<<"Calculate for "<<argv[1]<<endl;
      input = readfile(argv[1]);
      do_detect = atof(argv[2]);
      cout<<"Do detect="<<argv[2]<<endl;
      channel = atof(argv[3]);
      cout<<"Channel number = "<<argv[3]<<endl;
    }
//for single channel: using i++
//for stereo channel: using i+=2
    vector<float> input_x(round(input.size())/2,0);

                   

    if(channel==1)
        input_x = input;
    
    else{
        int j=0;
        for(int i=0;i<input.size();i+=2){
            input_x[j] = input[i];
            j++;
        }
    }


//time start
clock_t t_one;
t_one =clock();

        mat P   = STFT_with_FFTW3f(input_x,fs,N,overlap,window_type);   
// P: row is time and column is frequency

//put detection algorithm in
      if(do_detect){
        detect_whistle(P); 
      }
//time end
t_one = (clock()-t_one);
float total_time = (float)t_one/CLOCKS_PER_SEC;
cout<<"\nTotal time:"<<total_time<<" seconds"<<endl;

//using gp class to do the plotting
//------------------------------------------------------------

float x_data;
float y_data;
float power_value;

//Save data in matrix format
    FILE *fp;    
            fp = fopen("/home/yhhuang/thesis/Matlab/data_for_plot.dat","w");

            for(unsigned int i=0;i<P.n_rows;i++){
                for(unsigned int j=0;j<P.n_cols;j++){
              //      x_data = float(time_mapping(j,fs,N,overlap));
              //      y_data = float(frequency_mapping(i,fs,N));
                    power_value = P(i,j);
              //      fprintf(fp, "%d %s %d %s %f %s",j,"  ",i,"  ",power_value,"\n");
                    fprintf(fp, "%f %s",power_value,"  ");
                }
                fprintf(fp,"%s","\n");
            }
    fclose(fp);
}
