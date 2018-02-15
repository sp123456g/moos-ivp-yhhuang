/************************************************************/
/*    NAME: YHHUANG                                         */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.h                                   */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef PointAssign_HEADER
#define PointAssign_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <deque>
class PointAssign : public AppCastingMOOSApp
{
 public:
   PointAssign();
   ~PointAssign();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();
   double calculated_middle_x(std::deque<double> x_in);
 private: // Configuration variables
   std::deque<std::string> visit_point_buffer;
   std::string m_vname1;
   std::string m_vname2;
   std::string m_region;
   std::deque<double> m_x_array;
   double m_middle_x;
   int m_input_index;
   int m_x_array_input_index;
 private: // State variables
};

#endif 
