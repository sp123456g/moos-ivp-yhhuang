// This code is for TDOA algorithm for calculating the angle
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
using namespace std;
using namespace arma;

double CalculateTDOA(vec,vec, double);
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
   
    if(argc!=2){
      cout<<"Usage: ./GaborSTFT_using_function filename dt df B"<<endl;
      cout<<"Calculate 9k to 20k 3 seconds for default"<<endl;  
      
      input_x = readfile("9k_20k_3s.txt");
    
    }
    else{
      cout<<"Calculate for "<<argv[1]<<endl;
      input_x = readfile(argv[1]);
    }
//form the data to arma::vec form
    vec x_one = zeros<vec>(input_x.size());
    vec x_two = zeros<vec>(input_x.size());
//for single channel: using i++
//for stereo channel: using i+=2
    for(unsigned int i=0;i<input_x.size();i++){   
       in<<input_x[i];
          x_one(i) = input_x[i];
    }

    
    double sound_speed  = 1500;   //meter per second
    double distance     = 0.1; // meter
    double assume_theta = 45*(pi/180);

    double fs           = 96000;
    double assume_TDOA  = (distance/sound_speed)*cos(assume_theta);
    cout<<"assume_TDOA = "<<assume_TDOA<<endl;

    for(unsigned int i=0;i<input_x.size()-round(assume_TDOA*fs);i++){
        x_two(i+round(assume_TDOA*fs)) = x_one(i);
    }

    cout<<"round(assume_TDOA*fs)="<<round(assume_TDOA*fs)<<endl;
//time start
clock_t t_one;
t_one =clock();


    double TDOA = CalculateTDOA(x_one,x_two, fs);
    
    double theta = acos(TDOA*sound_speed/distance);

t_one = (clock()-t_one);
double total_time = (double)t_one/CLOCKS_PER_SEC;
cout<<"\nTotal time:"<<total_time<<" seconds"<<endl;

    cout<<"theta = "<<theta*180/pi<<endl;
}

double CalculateTDOA(vec ch1, vec ch2, double sample_rate){

unsigned int element = ch1.n_elem;
mat mat_ch1(element,1);
mat mat_ch2(element,1);
mat cor_result;

    mat_ch1 = ch1;
    mat_ch2 = ch2;


//    cout<<"size of mat_ch2 ="<<size(mat_ch2)<<endl;
    cor_result = conv(mat_ch1,flipud(mat_ch2),"same");


//    cout<<"size of cor_result = "<<size(cor_result)<<endl;

    unsigned int max_index = cor_result.index_max();
    unsigned int mid_point = (0+element-1)/2;    // set the mid point to be t=0 point
//    cout<<"max index = "<<max_index<<endl;
//    cout<<"mid index = "<<mid_point<<endl;
    int digi_tdoa = max_index - mid_point;

    double true_tdoa = -digi_tdoa/sample_rate;

//    cout<<"digi_tdoa = "<<digi_tdoa<<endl;
//    cout<<"true_tdoa = "<<true_tdoa<<endl;
    
    return(true_tdoa);
}
