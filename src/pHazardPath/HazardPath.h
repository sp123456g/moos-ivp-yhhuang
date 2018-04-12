/************************************************************/
/*    NAME: yhhuang                                              */
/*    ORGN: MIT                                             */
/*    FILE: HazardPath.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef HAZARD_PATH_HEADER
#define HAZARD_PATH_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "XYPoint.h"
#include <vector>

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
   void handleMailDetectList(std::string);

   void CalculateRegion(std::string, double, double);

   void string2XYPoint(std::vector<std::string>);
   void greedy_path(double, double);
   void i_path(double, double);
   std::string XYPoint2string();

 private: // Configuration variables
   std::vector<XYPoint> m_xypoint_list;
   std::vector<std::string> m_history_detect_buff;

   std::string m_path;
   std::string m_region;
   std::string m_height;
   std::string m_width;
   std::string m_s_path_one;
   std::string m_s_path_two;
   double m_nav_x;
   double m_nav_y;
   double m_middle_y_offset;
   double m_second_lane_width;
   int m_history_detect_size;

   double m_lane_width;
 private: // State variables
};

#endif 
