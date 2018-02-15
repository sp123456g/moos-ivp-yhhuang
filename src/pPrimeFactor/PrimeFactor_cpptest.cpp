#include <iostream>
#include <stdint.h>
#include <vector>
#include <deque>
#include <sstream>
#include <cstdlib>
#include <string>
#include <math.h>

std::string PrimeCalculate(uint64_t input);
uint64_t StrToUint64_t(std::string input); 
int main(){


    std::string a = "90090";
    std::string b = "56473";
    
    std::cout<<"a="<<a<<std::endl;
    std::cout<<"b="<<b<<std::endl;

    uint64_t a_value = StrToUint64_t(a);
    uint64_t b_value = StrToUint64_t(b);

    std::string result_a = PrimeCalculate(a_value);
    std::string result_b = PrimeCalculate(b_value);

        std::cout<<"a="<<result_a<<std::endl;
        std::cout<<"b="<<result_b<<std::endl;

}

uint64_t StrToUint64_t(std::string input){

    std::stringstream stream(input);
    uint64_t result;
    stream>>result;
    
    return result;
};

std::string PrimeCalculate(uint64_t input){

    std::vector<uint64_t> vector_prime; 
    int i =2;
    while(i!=input){
        int re = input%i;
            if(re==0){
                vector_prime.push_back(i);
                input /= i;
            }
            else{
                i++;
            }     
    }
   vector_prime.push_back(input); 
        std::string primelist="";

        for(int j=0;j<vector_prime.size();j++)
        {
            if(j!=0)
                primelist += "x";
           
            std::stringstream a;
            a<<vector_prime[j];
            std::string prime = a.str();
             
            primelist += prime;
        }
        return primelist;
}
