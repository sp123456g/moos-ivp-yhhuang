/************************************************************/
/*    NAME: yhhuang                                         */
/*    ORGN: NTU, Taipei                                     */
/*    FILE: SimAngle.h                                      */
/*    DATE: Jan 16th, 201                                   */
/************************************************************/

#ifndef SimAngle_HEADER
#define SimAngle_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class SimAngle : public AppCastingMOOSApp
{
 public:
   SimAngle();
   ~SimAngle();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();
    
   bool PulseOut();
   bool Coordinate_check();
   bool CheckAngle();
   bool CheckSPL();

 private: // Configuration variables

 private: // State variables

   double m_src_x;
   double m_src_y;

   double m_current_x;
   double m_current_y;
   double m_current_heading;
   double m_source_angle;
   double m_relate_x;
   double m_relate_y;
   double m_avg_spl;
   double m_source_level;

   bool m_whistle_exist;

   std::string m_fill_color;
   std::string m_edge_color;
};

#endif 
