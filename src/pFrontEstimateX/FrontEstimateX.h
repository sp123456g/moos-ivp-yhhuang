/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: FrontEstimateX.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef FrontEstimateX_HEADER
#define FrontEstimateX_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <deque>
class FrontEstimateX : public AppCastingMOOSApp
{
 public:
   FrontEstimateX();
   ~FrontEstimateX();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   bool ShareCommReport();
   bool GenCommReport(std::string report);
 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();
   bool        m_need_share; 
   int         m_input_index; 
   std::string m_report_name;
   std::string m_output_report;
   std::deque<std::string> m_input_buff; 
   std::deque<std::string> m_col_msg_buff; 
 protected:
   void registerVariables();

 private: // Configuration variables

 private: // State variables
};

#endif 
