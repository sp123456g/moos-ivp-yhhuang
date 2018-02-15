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
   void registerVariables();
   std::string int_to_str(int in);
   std::vector<uint64_t> PrimeCalculate(uint64_t input);
   void Done_or_not(std::vector<uint64_t> input_vector);
   std::string vector_to_result(std::vector<uint64_t> in_vec);
   void re_calculate(std::vector<uint64_t> input);
   uint64_t StrToUint64_t(std::string input);
   
 private: // Configuration variables
   
    uint64_t m_input_uint64_t;
    int m_input_index;
    int m_calculate_index;
    std::string m_input_string;
    std::vector<uint64_t> m_prime_vector;  //(orig|prime|done)
    std::deque<std::string> m_input_string_buffer;
    std::deque< std::vector<uint64_t> > m_not_done_buffer;
    std::deque<std::string> m_output_buffer;
    std::string m_output_string;    
 private: // State variables
};

#endif 
