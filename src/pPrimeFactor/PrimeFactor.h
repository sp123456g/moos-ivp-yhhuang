/************************************************************/
/*    NAME: YH_Huang                                              */
/*    ORGN: MIT/NTU                                             */
/*    FILE: PrimeFactor.h                                          */
/*    DATE: FEB.21/2018                            */
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
#include "PrimeNode.h"
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


   void     registerVariables();
   void     PrimeCalculate(PrimeNode &input);
   uint64_t StrToUint64_t(std::string input);
   
   std::string Node_to_result(PrimeNode input);
   std::string double_to_str(double in);
   
 private: // Configuration variables
   
 private: // State variables
    
    double m_input_index;
    double m_calculate_index;
    
    std::list<PrimeNode> m_input_list;
    std::string          m_output_result;
};

#endif 
