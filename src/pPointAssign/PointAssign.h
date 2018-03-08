/************************************************************/
/*    NAME: YHHUANG                                         */
/*    ORGN: MIT/NTU                                             */
/*    FILE: PointAssign.h                                   */
/*    DATE: MAR.8/2018                             */
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
 private: // Configuration variables
   std::deque<std::string> visit_point_buffer;  //input buff
   std::string m_vname1; // vehicle1
   std::string m_vname2; // vehicle2
   std::string m_region; // assign_by_region
   int m_input_index;    
 private: // State variables
};

#endif 
