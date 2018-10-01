#include "STFT.h"
#include <math.h>

int main(){

    vector<double> data;

    for(int i=0;i<32;i++){
        data.push_back(5*cos(2*i*180));
    cout<<"data["<<i<<"]="<<data[i]<<endl;
    }
//    for(int i=2049;i<4096;i+=1){
//    
//        data.push_back(i*i*i);
//    }

    STFT_with_FFTW3f(data,44100);
}
