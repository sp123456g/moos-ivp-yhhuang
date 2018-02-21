/************************************************************/
/*    NAME: YH_Huang                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactor.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef PrimeFactor_HEADER
#define PrimeFactor_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <stdint.h>
#include <vector>
#include <deque>
#include <list>
#include <sstream>
#include <cstdlib>

class PrimeFactor : public AppCastingMOOSApp
{
 public:
   PrimeFactor();
   ~PrimeFactor();

 protected: // Standard MOOSApp functions to overload 
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();
   
 protected:

   class Prime_Node
   {
    public:
        Prime_Node(){
            mP_time = 0;
            mP_done = false;
            mP_prime_list.clear();
            mP_temp = 1;
            
        };
        std::string           mP_orin;
        double                mP_recei_ind;
        double                mP_cal_ind;
        double                mP_time;
        std::vector<uint64_t> mP_prime_list;
        bool                  mP_done;
        uint64_t              mP_temp;
   };

   void registerVariables();
   std::string double_to_str(double in);
   void PrimeCalculate(Prime_Node &input);
   std::string Node_to_result(Prime_Node input);
   uint64_t StrToUint64_t(std::string input);
   
 private: // Configuration variables
   
 private: // State variables
    std::list<Prime_Node> m_input_list;
    double m_input_index;
    double m_calculate_index;
    std::string m_output_result;
};

#endif 
