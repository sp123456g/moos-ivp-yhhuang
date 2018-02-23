#include "PrimeNode.h"
using namespace std;

PrimeNode::PrimeNode()
{
// Constructor

            mP_time = 0;
            mP_done = false;
            mP_temp = 1;
            mP_prime_list.clear();

}

//Function for setting member variable
void PrimeNode::setOrinStr(std::string input)
{
        mP_orin=input;
}
void PrimeNode::setReceiInd(double rin)
{
        mP_recei_ind = rin;
}
void PrimeNode::setCalInd(double cin)
{
        mP_cal_ind = cin;
}
void PrimeNode::setDone(bool input)
{
        mP_done = input;
}
void PrimeNode::setTemp(uint64_t input)
{
        mP_temp = input;
}
void PrimeNode::addTime(double input_time)
{
        mP_time+=input_time;
}

//show function, won't change the member variable value
std::string PrimeNode::showOrin() const
{
        return(mP_orin);
}
double PrimeNode::showReceiInd() const
{
        return(mP_recei_ind);
}
double PrimeNode::showCalInd() const
{
        return(mP_cal_ind);
}
bool PrimeNode::showDone() const
{
        return(mP_done);
}
uint64_t PrimeNode::showTemp() const
{
        return(mP_temp);
}
double PrimeNode::showTime() const
{
        return(mP_time);
}
