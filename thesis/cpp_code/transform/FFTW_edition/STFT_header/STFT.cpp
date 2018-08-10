//***********************************************************************
//Author: Yen-Hsiang Huang(yhhuang)
//Origin: National Taiwan University
//Date:   Apr.20.2018
//***********************************************************************
#include "STFT.h"
#include <fftw3.h>
using namespace arma;
using namespace std;


//-------------------------------------------------------------------------

mat STFT_with_FFTW3f(std::vector<float> x,int fs,unsigned int N,float overlap_percent,int win)
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
}

unsigned int inv_freq_map(unsigned int input_frq, int fs, int N){

    unsigned int frq_index = round(input_frq*N/fs);

    return(frq_index);
}

float time_mapping(unsigned int input_index,int fs,int N, float overlap_percent){
// input_index is the time index in spectrogram_mat which was output by STFT_with_FFTW3f()

    int     start_index = (N-round(overlap_percent*N))*input_index;
    float  exact_time  = float(start_index*2+N)/float(2*fs);
    return(exact_time);
}

unsigned int inv_time_map(float input_time,int fs, int N,float overlap_percent){
    
    unsigned int time_index = round((input_time*2*fs*N)/(2*(N-round(overlap_percent*N))));
    return(time_index);
}
//--------------------------------------------------------------------------------------
//function for detect algorithm
//1. simple moving average 
//2. median filter
//3. edge detector
//4. moving_square  
//--------------------------------------------------------------------------------------


void simple_mov_avg(arma::mat &P, int n_avg=10){

    
    mat     avg_P(P.n_rows,P.n_cols); 

    for(int m=0;m<P.n_rows;m++){
        int start_index = 0;
        
        while(start_index!=(P.n_cols-1)){
            float  temp    = 0;   //temperary sum
            for(int n=(start_index-n_avg/2);n<(start_index+n_avg/2);n++){
                if(!(n<0 || n>=P.n_cols))
                    temp += P(m,n); 
                
            }
            avg_P(m,start_index) = temp/n_avg; 
            start_index++;
        }
    }
    P = avg_P;
}

void median_filter(arma::mat &P){
// 3*3 median filter

    for(unsigned int x = 1 ; x < P.n_rows-1 ; x++) {
        for(unsigned int y = 1; y < P.n_cols-1 ; y++) {

            unsigned int k = 0;  
            float window[9];  
            for(unsigned int xx = x - 1; xx < x + 2; ++xx){  
                for(unsigned int yy = y - 1; yy < y + 2; ++yy)  
                    window[k++] = P(xx,yy);  
            }
            //   Order elements (only half of them)  
            for (unsigned int m = 0; m < 5; ++m)  
            {  
                unsigned int min = m;  
                for (unsigned int n = m + 1; n < 9; ++n)  
                    if (window[n] < window[min])  
                        min = n;  
                //   Put found minimum element in its place  
               float temp = window[m];  
                window[m] = window[min];  
                window[min] = temp;  
            }  
            P(x,y)= window[4]; 
        }
    }
}



void edge_detector(arma::mat &P,float SNR_threshold,unsigned int jump_num){

    float SNR=0;
    mat P_new(P.n_rows,P.n_cols); 
    P_new.zeros();

    for(int i=0;i<P.n_cols;i++){

        vec time_column = P.col(i);
        for(int j=jump_num;j<time_column.n_elem;j++){
            if(j>=time_column.n_elem-jump_num)
                P(j,i) = 0;
            else{
                if(time_column(j-jump_num)!=0 || time_column(j+jump_num)!=0)    
                    SNR= 10*log(time_column(j)/(time_column(j-jump_num)*0.5+time_column(j+jump_num)*0.5));
                if(SNR > SNR_threshold){
                    P_new(j,i) = 1;
                }
            }
        }
    }
    P = P_new;
}

//narrow band checking 
void moving_square(arma::mat &P,unsigned int fs, unsigned int N, float overlap,float frq){
//moving square and high pass filter with frq Hz
    mat     P_new(P.n_rows,P.n_cols);
    P_new.zeros();
    
    vector<int> x_buf;
    vector<int> y_buf;
    int last_time_x;

    float  time_width = 0.01;
    float  percent_threshold=0.5;
    unsigned int bandwidth_sample = 5;
    unsigned int time_width_sample = 11;
//    unsigned int bandwidth  = 200;
//    float       dt = time_mapping(1,fs,N,overlap)-time_mapping(0,fs,N,overlap);
//    unsigned int bandwidth_sample = inv_freq_map(bandwidth,fs,N);
//    unsigned int time_width_sample= round(time_width/dt);
//    cout<<"bandwidth_sample="<<bandwidth_sample<<endl;
//    cout<<"time_width_sample="<<time_width_sample<<endl;

    if(bandwidth_sample%2==0)
        bandwidth_sample-=1;
    if(time_width_sample%2==0)
        time_width_sample+=1;
 //   unsigned int time_width_sample= inv_time_map(time_width,fs,N,overlap);
    
    for(int x = (bandwidth_sample-1)/2 ; x < P.n_rows-((bandwidth_sample-1)/2) ; x++) {
        for(int y = (time_width_sample-1)/2; y < P.n_cols-((time_width_sample-1)/2) ; y++) {

            float percent = 0;
            int k = 0;  
            x_buf.clear();
            y_buf.clear();

            for(int xx = x-(bandwidth_sample-1)/2; xx < x +((bandwidth_sample-1)/2)+1; ++xx){  
                for(int yy = y-(time_width_sample-1)/2;yy<y+((time_width_sample-1)/2)+1;++yy){  
                    if(P(xx,yy)==1){
                        k++;  
                        x_buf.push_back(xx);
                        y_buf.push_back(yy);
                    }
                        
                }
            }
           if(k>30){
//               cout<<"k="<<k<<endl;
//               cout<<"x="<<x<<endl;
//               cout<<"y="<<y<<endl;
//                   percent =float(k)/float(bandwidth_sample*time_width_sample);
//                   cout<<"percent:"<<percent<<endl;

            }
            percent =float(k)/float(bandwidth_sample*time_width_sample);

            if(percent >= percent_threshold){ 
                if(x>N*frq/fs){     //high pass filter (frq Hz)
                    for(int i=0;i<x_buf.size();i++){
                        P_new(x_buf[i],y_buf[i]) = 1;
                    } 
                }
            }
        }
    }
  
    P = P_new;
}


void moving_square_use_sub_mat(arma::mat &P,unsigned int fs, unsigned int N,float frq){
//moving square and high pass filter with frq Hz
    mat     P_new(P.n_rows,P.n_cols);
    P_new.zeros();

    unsigned int    last_time_index;
    unsigned int    last_frq_index;

    float  time_width = 0.01;
    float  percent_threshold=0.5;

    unsigned int bandwidth_sample = 5;
    unsigned int time_width_sample = 11;

    unsigned int threshold_num = round(percent_threshold*bandwidth_sample*time_width_sample);
    unsigned int sum=0;    

    mat P_sub(bandwidth_sample,time_width_sample);

    for(int x=0;x<P.n_cols-time_width_sample;x++){
        for(int y=inv_freq_map(frq,fs,N);y<P.n_rows-bandwidth_sample;y+=bandwidth_sample){
            P_sub = P.submat(y,x,y+bandwidth_sample,x+time_width_sample);
            sum=accu(P_sub);             

            if(sum>=threshold_num){
                P_new.submat(y,x,y+bandwidth_sample,x+time_width_sample) = P_sub;
            }
        }
    }
    
    P = P_new;
}
// detect_whistle algorithm
void detect_whistle(arma::mat &P,int fs,unsigned int N,float overlap){
        
//step1: simple moving average for each frequency(not good, take it off)
//    simple_mov_avg(P,10);
//step2: median filter
    median_filter(P);
//step3: edge_detector
    edge_detector(P,10,5);
//step4: using moving square for narrow band checking 
//slower edition
//    moving_square(P,fs,N,overlap,3000);
//faster edition
    moving_square_use_sub_mat(P,fs,N,3000);
}

std::vector<whistle> check_result(mat P, int fs,unsigned int N,float overlap){
   
    
    vector<whistle> output;
    output.clear();
    
    xy_index        last_node;
    xy_index        current_node;
    whistle         wh;
    
    wh.fs = fs;
    wh.N  = N;
    wh.overlap = overlap;

    int     x_ind_tor=7;  //x index tolerance
    int     y_ind_tor=10;  //y index tolerance
    bool    first_node = true;
    int     checking_bandwidth = 300;  //Hz
    int     checking_bandwidth_sample = inv_freq_map(checking_bandwidth,fs,N);
    
    for(int i=0;i<P.n_cols;i++){
        for(int j=0;j<P.n_rows;j++){
            if(P(j,i)==1){
                if(first_node){
                    last_node.x = i;
                    last_node.y = j;
                    wh.start_node = last_node;
                    first_node = false;
                }
                else{
                    if(fabs(i-last_node.x)<x_ind_tor && fabs(j-last_node.y)<y_ind_tor){
                        last_node.x = i;
                        last_node.y = j;
                    }
                    else{
                        wh.end_node = last_node;
                        wh.calculate();
                        output.push_back(wh);
                        
                        last_node.x = i;
                        last_node.y = j;
                        wh.start_node = last_node;
                    }
                }
            }    
        }
    }
    
    if(!first_node){
        wh.end_node = last_node;
        wh.calculate();
        output.push_back(wh);
    }

    return(output);
}
