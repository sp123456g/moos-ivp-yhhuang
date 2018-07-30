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

// declare the constant 
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
   
     double          B;      // rectangular window length (seconds)
        int         fs   = 96000;     // sample rate
        int          N   = 2048;
     double    overlap   = 0.9;
     double  threshold   = 30;        //threshold for detection
     bool    do_detect   = true;      //using detection algorithm to seperate data
        int  window_type = 1;         //Window type: 0:rectangular 1:Hanning
//----------------------------------------------------------

    if(argc!=3){
      cout<<"Usage: ./GaborSTFT_using_function filename do_detect"<<endl;
      cout<<"Calculate chirp with noise and do detect for default"<<endl;  
      
      input_x = readfile("chirp_with_noise.txt");
    
    }
    else{
      cout<<"Calculate for "<<argv[1]<<endl;
      input_x = readfile(argv[1]);
      do_detect = atof(argv[2]);
      cout<<"Do detect="<<argv[2]<<endl;
    }
//for single channel: using i++
//for stereo channel: using i+=2

//time start
clock_t t_one;
t_one =clock();

        mat P   = STFT_with_FFTW3f(input_x,fs,N,overlap,window_type);   
// P: row is time and column is frequency

//put detection algorithm in
      if(do_detect){
        detect_whistle(P,threshold); 
      }
//time end
t_one = (clock()-t_one);
double total_time = (double)t_one/CLOCKS_PER_SEC;
cout<<"\nTotal time:"<<total_time<<" seconds"<<endl;

//using gp class to do the plotting
//------------------------------------------------------------

double x_data;
double y_data;
double power_value;

//Save data
    FILE *fp;    
            fp = fopen("data_for_plot.dat","w");

            for(unsigned int i=0;i<P.n_rows;i++){
                for(unsigned int j=0;j<P.n_cols;j++){
                    x_data = double(time_mapping(j,fs,N,overlap));
                    y_data = double(frequency_mapping(i,fs,N));
                    power_value = P(i,j);
                    fprintf(fp, "%f %s %f %s %f %s",x_data,"  ",y_data,"  ",power_value,"\n");
                }
            }
    fclose(fp);

            stringstream ss_title;
            string title;
            double dt,df;
            
            dt = time_mapping(1,fs,N,overlap) - time_mapping(0,fs,N,overlap);
            df = (double)fs/(double)N; 
            ss_title<<"Spectrogram(df="<<setprecision(3)<<df<<",dt="<<dt<<")";
            ss_title>>title;
            
 sp::gplot   gp;
// std::string command0 = "set xtics ('0' 0,'10' 1000 )";   //gnuplot command can be type here
// std::string command1 = "set xtics 100.0";   //gnuplot command can be type here
// std::string command2 = "set grid";   //gnuplot command can be type here
// std::string command = "";   //gnuplot command can be type here
 gp.title(title.c_str());
 gp.ylabel("Frequency sample");
 gp.xlabel("Time sample");
// gp.xlim(0,T);
// gp.ylim(0,F);
// gp.send2gp(command0.c_str());    //send pipe command to gnuplot
// gp.send2gp(command1.c_str());    //send pipe command to gnuplot
// gp.send2gp(command2.c_str());    //send pipe command to gnuplot
 gp.set_parula_palette();        // color style
 gp.image(P);                    // Just like imagesc of Matlab
//-------------------------------------------------------------
}
