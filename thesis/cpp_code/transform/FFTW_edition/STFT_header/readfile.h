#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
using namespace std;
 
vector<double> readfile(string str_filename){
    
    
    char *c_filename = new char[str_filename.length()+1];
    strcpy(c_filename,str_filename.c_str());
    fstream fp;
    char line[128];
    vector<double> output;

    fp.open(c_filename, ios::in);//開啟檔案
    if(!fp){//如果開啟檔案失敗，fp為0；成功，fp為非0
            cout<<"Fail to open file: "<<c_filename<<endl;
        }
    cout<<"File Descriptor: "<<fp<<endl;
    while(fp.getline(line, sizeof(line), '\n')){
            
            double element;
            stringstream ss;
            ss<<line;
            ss>>element;
            output.push_back(element);    
    }

 
    fp.close();//關閉檔案
    
    
    return(output);
}
