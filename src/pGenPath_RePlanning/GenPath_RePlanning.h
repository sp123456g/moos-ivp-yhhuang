/************************************************************/
/*    NAME: YHHUANG                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath_RePlanning.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef GenPath_RePlanning_HEADER
#define GenPath_RePlanning_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <deque>
#include <XYSegList.h>
#include <string>
#include <sstream>
#include <math.h>
#include <unistd.h>    //use for usleep function
struct Node
{
    double xyid[3];    // Node: x|y|id;

};


class GenPath_RePlanning : public AppCastingMOOSApp
{
 public:
   GenPath_RePlanning();
   ~GenPath_RePlanning();

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
   std::deque<std::string> m_ReGenerate_buffer;

   //XYSegList my_seglist;

   size_t m_found_x;
   size_t m_found_c1;  //index of , behind x=
   size_t m_found_c2;
   size_t m_found_y;   //index of , behind y=
   size_t m_found_id;
   
   std::string sub_x1;   //x string value
   std::string sub_y1;   //y string value
   std::string m_update_str; //output string
   std::string m_id;
   std::string m_arrive;
   std::string m_visit_radius;
   std::string m_ReGenerate;
   std::string m_str_TARGET_X;
   std::string m_str_TARGET_Y;

   double x; // x double value
   double y; // y double value
   double id;
   double m_NAV_X;
   double m_NAV_Y;
   double m_TARGET_X;
   double m_TARGET_Y;
   
   Node node;
   
   int    m_input_index;
   int    m_calculated_index;
   int    m_waiting_mail_index;
   int    m_OnNewMail_index;
   
   bool   m_input_mail_or_not;
   
 private: // State variables
};

#endif 
