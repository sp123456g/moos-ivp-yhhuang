#pragma once
#include <iostream>
#include <vector>
#include <stdint.h>
class PrimeNode
{
    public:
       
         PrimeNode();
        ~PrimeNode(){};
        
        void setOrinStr(std::string input);
        void setReceiInd(double rin);
        void setCalInd(double cin);
        void setDone(bool input);
        void setTemp(uint64_t input);
        void addTime(double input_time);

        std::string     showOrin() const;
        double          showReceiInd() const;
        double          showCalInd() const;
        bool            showDone() const;
        uint64_t        showTemp() const;
        double          showTime() const;

    public:

        double                mP_time;
        std::vector<uint64_t> mP_prime_list;

    protected:   

        std::string           mP_orin;
        double                mP_recei_ind;
        double                mP_cal_ind;
        bool                  mP_done;
        uint64_t              mP_temp;  //uint64_t is in the stdint.h
};
