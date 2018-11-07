/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: DataReceiver.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef DataReceiver_HEADER
#define DataReceiver_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class DataReceiver : public AppCastingMOOSApp
{
 public:
   DataReceiver();
   ~DataReceiver();

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
