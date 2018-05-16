#include <fstream>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <sstream>
using namespace std;

class WavData {

    public:
        short* data;
        unsigned long size;


        WavData(){
            data = NULL;
            size = 0;
        }
};

void loadWaveFile(std::string inputname, WavData *ret){

        char *fname=  new char[inputname.length()+1];
        strcpy(fname,inputname.c_str());
        FILE* fp = fopen(fname,"rb");
        if(fp){
         char id[5];
         unsigned long size;
         short format_tag, channels, block_align, bits_per_sample;
         unsigned long format_length, sample_rate, avg_bytes_sec, data_size;
        
         fread(id, sizeof(char),4,fp);
         id[4] = '\0';
stringstream ss;
string str_sample_rate;
ss<<sample_rate;
ss>>str_sample_rate;
            cout<<id<<endl;
         if(!strcmp(id,"RIFF")){
            fread(id, sizeof(char),4,fp);
            cout<<"probe1"<<endl;
            fread(&format_length, sizeof(unsigned long),1,fp);
            cout<<"probe2"<<endl;
            fread(&format_tag, sizeof(short),1,fp);
            cout<<"probe3"<<endl;
            fread(&channels, sizeof(short),1,fp);
            cout<<"probe4"<<endl;
            fread(&sample_rate, sizeof(short),1,fp);
            cout<<"sample_rate:"<<str_sample_rate<<endl;
            fread(&avg_bytes_sec, sizeof(unsigned long), 1, fp);
            cout<<"probe6"<<endl;
            fread(&block_align, sizeof(short), 1, fp);
            cout<<"probe7"<<endl;
            fread(&bits_per_sample, sizeof(short), 1, fp);
            cout<<"probe8"<<endl;
            fread(id, sizeof(char), 4, fp);
            cout<<"probe9"<<endl;
            fread(&data_size, sizeof(unsigned long), 1, fp);
            cout<<"probe10"<<endl;

            ret->size = data_size/sizeof(short);
            cout<<"probe11"<<endl;
            ret->data = (short*) malloc(data_size);
            cout<<"probe12"<<endl;
            //fread(ret->data, sizeof(short), ret->size, fp);
            cout<<"probe13"<<endl;
         }
         else{
            cout<<"Error: RIFF file but not a wave file\n";
         }
        }
        else{
            cout<<"Error: not a RIFF file\n";
        }
        fclose(fp);
}

int main(){

    WavData song;
    loadWaveFile("Chord.wav",&song);
    cout<<song.data<<endl;
    cout<<"there are "<<song.size/2<<" samples in this WAV file."<<endl;
    return 0;
}
