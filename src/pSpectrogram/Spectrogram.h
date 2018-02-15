/************************************************************/
/*    NAME: yhhuang                                              */
/*    ORGN: MIT                                             */
/*    FILE: Spectrogram.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef Spectrogram_HEADER
#define Spectrogram_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
//#include "MOOS/libMOOS/MOOSLib.h"

class Spectrogram : public AppCastingMOOSApp
{
 public:
   Spectrogram();
   ~Spectrogram();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   bool buildReport();
   
 protected:
   void RegisterVariables();

 private: // Configuration variables

 private: // State variables
};

#endif 
