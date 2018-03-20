/************************************************************/
/*    NAME: YHHuang                                              */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign_Auto.h                                          */
/*    DATE: MAR 20th, 2018                             */
/************************************************************/

#ifndef PointAssign_Auto_HEADER
#define PointAssign_Auto_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <deque>
class PointAssign_Auto : public AppCastingMOOSApp
{
 public:
   PointAssign_Auto();
   ~PointAssign_Auto();

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
  
   std::string m_vname1; // vehicle1
   std::string m_vname2; // vehicle2
   std::string m_region; // assign_by_region
   std::string m_node_report_local;
   int m_input_index; 
   
   std::deque<std::string> m_visit_point_buffer;  //input buff
   std::deque<std::string> m_visit_point_buff_v1;
   std::deque<std::string> m_visit_point_buff_v2;


 private: // State variables
};

#endif 
