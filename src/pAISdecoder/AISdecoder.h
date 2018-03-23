/************************************************************/
/*    NAME: YHHuang                                              */
/*    ORGN: MIT                                             */
/*    FILE: AISdecoder.h                                          */
/*    DATE: MAR 19th 2018                             */
/************************************************************/

#ifndef AISdecoder_HEADER
#define AISdecoder_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "AisNode.h"
#include <deque>
class AISdecoder : public AppCastingMOOSApp
{
 public:
   AISdecoder();
   ~AISdecoder();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables
    
   std::deque<AisNode> m_input_buff; 

 private: // State variables

   std::string m_orin;
   std::string m_result;
};

#endif 
