// This code is for TDOA algorithm for calculating the angle
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <time.h>
#include <algorithm>
#include <iostream>
#include "readfile.h"
using namespace std;

double CalculateTDOA(vector<double>,vector<double>, double,double,double);
int main(int argc, char* argv[]){

// declare the constant 
    const double pi = 3.1415926;
//---------------------------------------------------------
//Read Data to form x (using file only contain voltage data)
//User can input the data by command line, default is 9k_20k_3s
    vector<double> input_one;
//----------------------------------------------------------
   
    if(argc!=2){
      cout<<"Usage: ./GaborSTFT_using_function filename dt df B"<<endl;
      cout<<"Calculate 9k to 20k 3 seconds for default"<<endl;  
      
      input_one = readfile("9k_20k_3s.txt");
    
    }
    else{
      cout<<"Calculate for "<<argv[1]<<endl;
      input_one = readfile(argv[1]);
    }
//set up testing channel 2 
    vector<double> input_two(input_one.size(),0);

//time start
clock_t t_one;
t_one =clock();

    double sound_speed  = 1500;     // meter/second
    double distance_mic = 0.1;      // meter
    double assume_theta = 30*(pi/180);  // 45 degree change to radical
    double fs           = 192000;     //sample_rate
    double assume_TDOA  = (distance_mic/sound_speed)*cos(assume_theta);
    double TDOA;   // time difference of arrival
    
   
   if(assume_theta <= pi/2){
    for(unsigned int i=0;i<input_one.size()-round(assume_TDOA*fs);i++){
       input_two[i+round(assume_TDOA*fs)] = input_one[i];
    }
    TDOA = CalculateTDOA(input_one,input_two, fs, distance_mic, sound_speed);
    double theta = acos(TDOA*sound_speed/distance_mic);
    cout<<"theta="<<theta*(180/pi)<<endl;
   }
   else{
    for(unsigned int i=0;i<input_one.size()+round(assume_TDOA*fs);i++){
       input_two[i-round(assume_TDOA*fs)] = input_one[i];
    }
    TDOA = CalculateTDOA(input_two,input_one, fs, distance_mic, sound_speed);
    double theta = pi-acos(TDOA*sound_speed/distance_mic);
    cout<<"theta="<<theta*(180/pi)<<endl;

   }

    cout<<"round(assume_TDOA*fs)="<<round(assume_TDOA*fs)<<endl;
    cout<<"TDOA="<<TDOA<<endl;
//  if(assume_theta>pi/2)
 //   TDOA = CalculateTDOA(input_two,input_one, fs, distance_mic, sound_speed);
////testing signal
//   vector<double> test_ch1(1000,0);
//   vector<double> test_ch2(1000,0);
//   test_ch1[1] = 20;
//   test_ch2[2] = 20;
//   TDOA = CalculateTDOA(test_ch1,test_ch2,fs, distance_mic,sound_speed);
    t_one = (clock()-t_one);
double total_time = (double)t_one/CLOCKS_PER_SEC;
cout<<"\nTotal time:"<<total_time<<" seconds"<<endl;


//    cout<<"theta = "<<theta*180/pi<<endl;
    
}

double CalculateTDOA(vector<double> ch1, vector<double> ch2, double sample_rate, double mic_distance=0.1, double sound_speed=1500){
// maximum distance between two microphone default is 0.1 meter, and sound_speed default is 1500

    double                  true_tdoa;   // unit:seconds
    
    unsigned int            correlation_factor = 3+round((mic_distance/sound_speed)*sample_rate);    //add "correlation_factor" zeros at front and behind 
    cout<<"correlation_factor = "<<correlation_factor<<endl;
    unsigned int            output_size = 1+correlation_factor*2;
//initialize array
    vector<double>  output(output_size,0);
    vector<double>  addzero_ch1(ch1.size()+correlation_factor*2,0); 
//add zero at front and behind
        for(unsigned int i=0;i<ch1.size();i++){
            addzero_ch1[i+correlation_factor] = ch1[i]; 
        }

//correlation for only 2*correlation_factor+1 times to save time 
    unsigned int i=0;
    while(1){     //i for ch1
        double  temp = 0;
        unsigned int i1=i;
        for(unsigned int j=0;j<ch2.size();j++){      //j for ch2
            temp = temp + (addzero_ch1[i1]*ch2[j]);             
            i1++;
        }
            output[i]=temp;
            i++;
            if(i1==addzero_ch1.size())
                break;
    }
//find the maximun index after correlation
    vector<double>::iterator maxPosition = max_element(output.begin(), output.end());
    cout << *maxPosition << " at the postion of " << maxPosition - output.begin() <<endl;
    int max_index = maxPosition - output.begin();

    int     digi_TDOA = max_index - correlation_factor;
    double  true_TDOA;
// check digital TDOA digital TDOA is samples different and trueTDOA is in seconds

    if(digi_TDOA ==correlation_factor-3){
     cout<<"\nCh2 arrive first, and the digital TDOA="<<-digi_TDOA<<endl;
     true_TDOA = mic_distance/sound_speed;
    }
    else if(-digi_TDOA ==correlation_factor-3){
     cout<<"\nCh1 arrive first, and the digital TDOA="<<-digi_TDOA<<endl;
     true_TDOA = mic_distance/sound_speed;
    }
    else if(digi_TDOA < 0){
     cout<<"\nCh1 arrive first, and the digital TDOA="<<-digi_TDOA<<endl;
     
     true_TDOA = -digi_TDOA*(1/sample_rate);
     
     cout<<"true tdoa = "<<true_TDOA<<endl;
    }
    else if(digi_TDOA > 0){   
     cout<<"\nCh2 arrive first, and the digital TDOA="<<digi_TDOA<<endl;
     
     true_TDOA = digi_TDOA*(1/sample_rate);
     
     cout<<"true tdoa = "<<true_TDOA<<endl;
    }
    else if(digi_TDOA == 0){
        cout<<"\nTwo channel arrive at the same time"<<endl;
        true_TDOA = 0;
    } 
    return(true_TDOA);
}
