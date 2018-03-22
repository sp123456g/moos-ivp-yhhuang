#include <iostream>
#include <deque>
#include <math.h>
#include <sstream>
#include <string>
#include <string.h>
using namespace std;
long int BinToDec(std::deque<int> input)  // ex: 11110 input[0]=0 input[1]=1...
{
    long int sum=0;
    if(input[input.size()-1]==1){
         for(int i=0;i<input.size()-1;i++){
      
                sum+=input[i]*pow(2,i);

                std::cout<<input[i]<<"*2^"<<i<<std::endl;
         }
         
         sum+=-1*pow(2,input.size()-1);
         
    }
    else{    
         for(int i=0;i<input.size();i++){
      
                sum+=input[i]*pow(2,i);

                std::cout<<input[i]<<"*2^"<<i<<std::endl;
         }
    }
        return(sum);    
};

int main(){


    std::string a;
    std::cout<<"Enter a binary number:";
        std::cin>>a;

        std::deque<int> binary;

        binary.clear();
        std::cout<<a.length()<<std::endl;
        for(int i=0;i<a.length();i++){
        
            std::stringstream ss;
            int number;
            ss<<a[i];
            
            ss>>number;
            binary.push_front(number);
            number = 0;
        }

        long int output_num;

        for(int i=0;i<binary.size();i++){
            std::cout<<"Element["<<i<<"]="<<binary[i]<<std::endl;
        }
        output_num = BinToDec(binary);

        std::string str_out;
        std::stringstream sss;
        sss<<output_num;
        sss>>str_out;
        

        std::cout<<"The decimal of it is:"<<str_out<<std::endl;

}
