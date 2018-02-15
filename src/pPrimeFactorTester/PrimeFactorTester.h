/************************************************************/
/*    NAME: YH_Huang                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactorTester.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef PrimeFactorTester_HEADER
#define PrimeFactorTester_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include <string>
#include <sstream>
#include <iostream>
#include <stdint.h>
#include <cstdlib>
class PrimeFactorTester : public CMOOSApp
{
 public:
   PrimeFactorTester();
   ~PrimeFactorTester();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();

 private: // Configuration variables
    std::string m_input_str;
    bool m_correct;
    uint64_t m_input_value;
    std::vector<uint64_t> m_input_prime;
    uint64_t m_result;
    std::string m_output_str;
 private: // State variables
};

#endif 
