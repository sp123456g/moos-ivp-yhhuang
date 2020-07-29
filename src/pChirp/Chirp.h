/************************************************************/
/*    NAME: hs                                              */
/*    ORGN: NTU                                             */
/*    FILE: Chirp.h                                          */
/*    DATE: December 17th, 2018                             */
/************************************************************/

#ifndef Chirp_HEADER
#define Chirp_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
//#include <string>
class Chirp : public AppCastingMOOSApp
{
 public:
   Chirp();
   ~Chirp();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();
//   void sleeptime(int);
 private: // Configuration variables
   std::string m_loiter;
   std::string m_chirp;
   std::string m_station;
   std::string m_whistle;
 private: // State variables
};

#endif 
