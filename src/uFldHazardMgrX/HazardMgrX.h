/*****************************************************************/
/*    NAME: Michael Benjamin                                     */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: HazardMgrX.h                                          */
/*    DATE: Oct 26th 2012                                        */
/*                                                               */
/* This file is part of MOOS-IvP                                 */
/*                                                               */
/* MOOS-IvP is free software: you can redistribute it and/or     */
/* modify it under the terms of the GNU General Public License   */
/* as published by the Free Software Foundation, either version  */
/* 3 of the License, or (at your option) any later version.      */
/*                                                               */
/* MOOS-IvP is distributed in the hope that it will be useful,   */
/* but WITHOUT ANY WARRANTY; without even the implied warranty   */
/* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See  */
/* the GNU General Public License for more details.              */
/*                                                               */
/* You should have received a copy of the GNU General Public     */
/* License along with MOOS-IvP.  If not, see                     */
/* <http://www.gnu.org/licenses/>.                               */
/*****************************************************************/

#ifndef UFLD_HAZARD_MGR_HEADER
#define UFLD_HAZARD_MGR_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "XYHazardSet.h"
#include "XYPolygon.h"
#include <deque>

class HazardMgrX : public AppCastingMOOSApp
{
 public:
   HazardMgrX();
   ~HazardMgrX() {}

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected: // Registration, Configuration, Mail handling utils
   void registerVariables();
   bool handleMailSensorConfigAck(std::string);
   bool handleMailSensorOptionsSummary(std::string) {return(true);}
   bool handleMailDetectionReport(std::string);
   bool handleMailHazardReport(std::string) {return(true);}
   void handleMailReportRequest();
   void handleMailMissionParams(std::string);
   void handleMailSend2Other();
   void getHazardToOutput(std::string);
 
 protected: 
   void postSensorConfigRequest();
   void postSensorInfoRequest();
   void postHazardSetReport();
   void postHistoryDetectList();
   
 private: // Configuration variables
   double      m_swath_width_desired;
   double      m_pd_desired;
   std::string m_report_name;
   std::string m_col_result;
   //unsigned int m_col_send_time;
   bool        m_need_station_keep;
   bool        m_hit_communicate_point;
 private: // State variables
   bool   m_sensor_config_requested;
   bool   m_sensor_config_set;
   bool   m_survey_redetect;

   unsigned int m_sensor_config_reqs;
   unsigned int m_sensor_config_acks;
   unsigned int m_wpt_index;

   unsigned int m_sensor_report_reqs;
   unsigned int m_detection_reports;
   unsigned int m_summary_reports;
   std::deque<std::string>  m_detection_reports_str_buff;
   std::deque<std::string> m_history_detect_buff;
   std::deque<std::string>  m_output_buff;
   std::vector<XYPoint> m_xypoint_list;
   double m_swath_width_granted;
   double m_pd_granted;

   double m_nav_x;
   double m_nav_y;
    
   bool m_self_result; // if it's from own vehicle, not collaborator
   XYHazardSet m_hazard_set;
   XYPolygon   m_search_region;
   double      m_transit_path_width;
   std::string m_message_in;
   std::string m_sec_detect_width;
   std::string m_sec_pd; 
};

#endif 








