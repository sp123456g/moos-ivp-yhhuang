#include <iostream>
#include <deque>
#include <sstream>
using namespace std;

int main(){

    deque<std::string> str_buff;

    for(int i=0;i<=100;i++){
        stringstream a;
        a<<i;
        string i_str = a.str();    
        str_buff.push_back(i_str);
        cout<<i_str<<endl;
    }
cout<<"pop_out"<<endl;
    while(!str_buff.empty()){
        cout<<str_buff.front()<<endl;
        str_buff.pop_front();
    }
}
