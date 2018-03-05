/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: CommunicationAngle_yhhuang.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef CommunicationAngle_yhhuang_HEADER
#define CommunicationAngle_yhhuang_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <sstream>
class CommunicationAngle_yhhuang : public AppCastingMOOSApp
{
 public:
   CommunicationAngle_yhhuang();
   ~CommunicationAngle_yhhuang();

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
    double m_surface_sound_speed;
    double m_sound_speed_gradient;
    double m_water_depth;       //Onstartup variables
    double m_time_interval;           
    std::string m_collaborator;
   
    std::string m_collnavx;
    std::string m_collnavy;
    std::string m_collnavdepth;
    std::string m_collnavspeed;   // Subscribed variables
    std::string m_collnavhead;
    
    double m_NAV_X;
    double m_NAV_Y;
    double m_NAV_HEADING;
    double m_NAV_DEPTH;
    double m_NAV_SPEED; 
    double m_OWNER;
    double m_CO_NAV_X;
    double m_CO_NAV_Y;        
    double m_CO_NAV_DEPTH;    // OnNewMail variables
    double m_CO_NAV_SPEED;
    double m_CO_NAV_HEADING;
    double m_greater_depth;
    double thetaZeroCalculate(double theta, double Radius);
    double RCalculate();
    double thetaCalculate(double radius);
};

#endif 
