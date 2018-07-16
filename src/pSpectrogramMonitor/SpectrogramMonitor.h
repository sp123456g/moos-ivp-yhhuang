/************************************************************/
/*    NAME: yhhuang                                              */
/*    ORGN: NTU Taipei, Taiwan                                             */
/*    FILE: SpectrogramMonitor.h                                          */
/*    DATE: July 16th, 2018                             */
/************************************************************/

#ifndef SpectrogramMonitor_HEADER
#define SpectrogramMonitor_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class SpectrogramMonitor : public AppCastingMOOSApp
{
 public:
   SpectrogramMonitor();
   ~SpectrogramMonitor();

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
