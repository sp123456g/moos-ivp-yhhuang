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
#include <fftw3.h>
using namespace arma;
using namespace std;

const double pi = 3.1415926;
const complex<double> i(0,1);

//-------------------------------------------------------------------------

mat STFT_with_FFTW3f(std::vector<double> x,int fs=44100,unsigned int N=2048,float overlap_percent=0.9,int win=1)
{
//STEP_1 set up window function 
    float   W;
    int     overlap = round(overlap_percent*N);
    int     no_overlap = N-overlap; 
    float   *window_func;

    if(no_overlap ==0)
        no_overlap =1;

    window_func = new float[N];

// 0 is Rectangular 
// 1 is Hann 
// 2 is Gassian

    int i;
    switch (win) {
    
    case 0:  // rectangular
      for( i=0; i<N; ++i)
	    window_func[i] = 1.0F;
    break;
    
    case 1:  // Hann window (C^1 cont, so third-order tails)
      W = N/2.0F;
      for( i=0; i<N; ++i)
	    window_func[i] = (1.0F + cos(pi*(i-W)/W))/2;
    break;
    
    default:
      fprintf(stderr, "unknown windowtype!\n");
    }
//STEP_2 set up fftw plan 
    float*  in;
    float*  after_fft;
    float*  power;
    unsigned int loop_num = 0;
    int     time_index = 0;
 
    in = new float[N];
    after_fft = new float[N];
    power = new float[N+1];
    fftwf_plan plan;
    plan = fftwf_plan_r2r_1d(N, in,after_fft,FFTW_R2HC,FFTW_MEASURE);

//Get the loop number to set up the size of the output matrice
    for(int start_index=0;start_index<=x.size()-N;start_index+=no_overlap)
        loop_num +=1;

    mat spectrogram_mat(N+1,loop_num);
    mat output(fs/2,loop_num);
//STEP_3 doing fftw loop
    for(int start_index=0;start_index<=x.size()-N;start_index+=no_overlap){
//get data by window function 
        for(int i=start_index,j=0;i<start_index+N;i++,j++){
            in[j] = x[i]*window_func[j];    
        }
//fft 
        fftwf_execute(plan);
// Calculate power and let index 0 = frequency = 1Hz
        for(int k=0 ;k<=N;k++){
            power[k+1] = sqrt(pow(after_fft[k],2)+pow(after_fft[N-k],2));
//Save in the 2 dimensional array
            spectrogram_mat(k,time_index) = power[k];
        }
        time_index++;
    }
// the spectrogram_mat variable is now x:time sample, y:frequency   
//STEP_4 mapping frequency from N sample(spectrogram_mat) to fs sample (output)
// but this step spend too many times
//    for(int i=0;i<fs/2;i++){
//       output.row(i) = spectrogram_mat.row(round(i*N/fs)); 
//       cout<<"round(i*N/fs)="<<round(i*N/fs)<<endl;
//    }
        fftwf_destroy_plan(plan);
        fftwf_cleanup();
//output: row is frequency and column is time, just means x is time and y is frequency
//But remember to change the index to exact frequency and time 
        spectrogram_mat = spectrogram_mat.submat(0,0,N/2,time_index-1);

    return(spectrogram_mat);
}

unsigned int frequency_mapping(unsigned int input_index, int fs,int N){
// input_index is the frequency index in spectrogram_mat which was output by STFT_with_FFTW3f()
    unsigned int exact_frequency = round((fs/N)*input_index);

    return(exact_frequency);
};

double time_mapping(unsigned int input_index,int fs,int N, double overlap_percent){
// input_index is the time index in spectrogram_mat which was output by STFT_with_FFTW3f()

    int     start_index = (N-round(overlap_percent*N))*input_index;
    double  exact_time  = double(start_index*2+N)/double(2*fs);
    return(exact_time);
            
};

void median_filter(arma::mat &P){
// 3*3 median filter

    for(int x = 1 ; x < P.n_rows-1 ; x++) {
        for(int y = 1; y < P.n_cols-1 ; y++) {

            int k = 0;  
            int window[9];  
            for(int xx = x - 1; xx < x + 2; ++xx){  
                for(int yy = y - 1; yy < y + 2; ++yy)  
                    window[k++] = P(xx,yy);  
            }
            //   Order elements (only half of them)  
            for (int m = 0; m < 5; ++m)  
            {  
                int min = m;  
                for (int n = m + 1; n < 9; ++n)  
                    if (window[n] < window[min])  
                        min = n;  
                //   Put found minimum element in its place  
               int temp = window[m];  
                window[m] = window[min];  
                window[min] = temp;  
            }  
            P(x,y)= window[4]; 
        }
    }
    
}

// detect_whistle algorithm
void detect_whistle(arma::mat &P, double threshold){
        

// step1: simple moving average for each frequency
    
    int     n_avg   = 10; //average_sample number
    mat     avg_P(P.n_rows,P.n_cols); 

    for(int m=0;m<P.n_rows;m++){
        int start_index = 0;
        
        while(start_index!=(P.n_cols-1)){
            double  temp    = 0;   //temperary sum
            for(int n=(start_index-n_avg/2);n<(start_index+n_avg/2);n++){
                if(!(n<0 || n>=P.n_cols))
                    temp += P(m,n); 
            }
            avg_P(m,start_index) = temp/n_avg; 
            start_index++;
        }
    }
    P = P - avg_P;
// step2: median filter
    
    median_filter(P);
}

#endif
