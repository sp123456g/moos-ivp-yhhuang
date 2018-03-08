/************************************************************/
/*    NAME: YHHUANG                                         */
/*    ORGN: MIT/NTU                                             */
/*    FILE: GenPath.h                                          */
/*    DATE: MAR.8/2018                             */
/************************************************************/

#ifndef GenPath_HEADER
#define GenPath_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <deque>
#include <XYSegList.h>
#include <string>
#include <sstream>
#include <math.h>
#include <unistd.h>
struct Node
{
    double xyid[3];    // Node: x|y|id;

};

class GenPath : public AppCastingMOOSApp
{
 public:
   GenPath();
   ~GenPath();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();
   std::deque<Node> Calculate_min(std::deque<Node>);
 private: // Configuration variables
   std::deque<std::string> m_input_buffer;
   std::deque<Node> m_xyid_buff;
   std::deque<Node> m_out_node;
   XYSegList my_seglist;
   size_t m_found_x;
   size_t m_found_c1;  //index of , behind x=
   size_t m_found_c2;
   size_t m_found_y;   //index of , behind y=
   size_t m_found_id;
   std::string sub_x1;   //x string value
   std::string sub_y1;   //y string value
   std::string m_update_str; //output string
   std::string m_id;
   double x; // x double value
   double y; // y double value
   double id;
   Node node;
   
   int    m_input_index;
   int    m_calculated_index;
   int    m_waiting_mail_index;
   int    m_OnNewMail_index;
   bool   m_input_mail_or_not;
 private: // State variables
};

#endif 
