#include <iostream>
#include <vector>
#include <stdint.h>
class Prime_Node
{
    public:
       
        Prime_Node(){
            mP_time = 0;
            mP_done = false;
            mP_prime_list.clear();
            mP_temp = 1;
            mP_product_num = 1;
        };
       
        std::string           mP_orin;
        double                mP_recei_ind;
        double                mP_cal_ind;
        double                mP_time;
        std::vector<uint64_t> mP_prime_list;
        bool                  mP_done;
        uint64_t              mP_temp;  //uint64_t is in the stdint.h
        uint64_t              mP_orin_value;
        uint64_t              mP_product_num;
};
