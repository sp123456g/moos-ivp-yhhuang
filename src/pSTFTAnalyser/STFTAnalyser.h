/************************************************************/
/*    NAME: yhhuang                                         */
/*    ORGN: MIT                                             */
/*    FILE: STFTAnalyser.h                                  */
/*    DATE: May 29th 2018                                   */
/************************************************************/

#ifndef STFTAnalyser_HEADER
#define STFTAnalyser_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class STFTAnalyser : public AppCastingMOOSApp
{
 public:
   STFTAnalyser();
   ~STFTAnalyser();

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

 private: // State variables
};

#endif 
