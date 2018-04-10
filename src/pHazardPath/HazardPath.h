/************************************************************/
/*    NAME: yhhuang                                              */
/*    ORGN: MIT                                             */
/*    FILE: HazardPath.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef HazardPath_HEADER
#define HazardPath_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class HazardPath : public AppCastingMOOSApp
{
 public:
   HazardPath();
   ~HazardPath();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();
   void handleMailMissionParams(std::string);
   void CalculateRegion(std::string);
 private: // Configuration variables

   std::string m_path;
   std::string m_region;
   std::string m_height;
   std::string m_width;
   std::string m_output_one;
   std::string m_output_two;
 private: // State variables
};

#endif 
