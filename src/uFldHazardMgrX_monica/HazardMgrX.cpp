/*****************************************************************/
/*    NAME: Michael Benjamin                                     */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: HazardMgrX.cpp                                        */
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

#include <iterator>
#include <stdlib.h>
#include "MBUtils.h"
#include "HazardMgrX.h"
#include "XYPoint.h"
#include "XYFormatUtilsHazard.h"
#include "XYFormatUtilsPoly.h"
#include "ACTable.h"
using namespace std;

//---------------------------------------------------------
// Constructor

HazardMgrX::HazardMgrX()
{
  // Config variables
  m_swath_width_desired = 25;
  m_pd_desired          = 0.9;

  // State Variables 
  m_sensor_config_requested = false;
  m_sensor_config_set   = false;
  m_swath_width_granted = 0;
  m_pd_granted          = 0;
  m_wpt_index           = 0;
  m_hit_communicate_point = false;
  m_need_station_keep   = true;
  m_survey_redetect     = false;
  //m_col_send_time       = 0;
  m_sensor_config_reqs = 0;
  m_sensor_config_acks = 0;
  m_sensor_report_reqs = 0;
  m_detection_reports  = 0;

  m_summary_reports = 0;

  m_detection_reports_str_buff.clear();
  m_history_detect_buff.clear();
  m_self_result = true;
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool HazardMgrX::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key   = msg.GetKey();
    string sval  = msg.GetString(); 
    double dval  = msg.GetDouble();

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
    
    if(key == "UHZ_CONFIG_ACK") 
      handleMailSensorConfigAck(sval);

    else if(key == "UHZ_OPTIONS_SUMMARY") 
      handleMailSensorOptionsSummary(sval);

    else if(key == "UHZ_DETECTION_REPORT"){ 
      handleMailDetectionReport(sval);

  string buff_input = sval;    
        m_detection_reports_str_buff.push_back(buff_input);
    }

    else if(key == "HAZARDSET_REQUEST") {
      handleMailReportRequest();
        //Notify("WAPOINTING","true");
        //Notify("STATION", "false");
    }

    else if(key == "UHZ_MISSION_PARAMS") 
      handleMailMissionParams(sval);
    
    else if(key == "COL_RESULT"){
      //m_col_send_time++;
      m_need_station_keep = false;
     //  if(sval != "\""){
        vector<string> col_parse_buff = parseString(sval, '#');
            while(!col_parse_buff.empty()){
                string message_in = col_parse_buff.back();
                    m_self_result = false;
                    handleMailDetectionReport(message_in);
                    col_parse_buff.pop_back();
                    string summary_report = m_hazard_set.getSpec("final_report");
                Notify("HAZARDSET_REPORT", summary_report);
            }
     //  }
    }
    else if(key == "WPT_INDEX") 
      m_wpt_index = dval;
    else if (key == "ARRIVE") {
      if(m_wpt_index%4 == 3) {
        m_hit_communicate_point = true;
      }
    }
    else if (key == "SURVEY") {
      if(sval == "REDETECT") {
        m_survey_redetect = true;
        //------------------for configure request----------------  
        string re_config;
        stringstream ss_re;
        ss_re<<"vname="<<m_host_community<<",width="<<m_sec_detect_width<<",pd="<<m_sec_pd;
        ss_re>>re_config;
        Notify("UHZ_CONFIG_REQUEST",re_config);
      }
    }
    else if (key == "UHZ_HAZARD_REPORT"){
        getHazardToOutput(sval);
    }
    else 
      reportRunWarning("Unhandled Mail: " + key);
  }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool HazardMgrX::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool HazardMgrX::Iterate()
{
  AppCastingMOOSApp::Iterate();

  if(!m_sensor_config_requested)
    postSensorConfigRequest();

  if(m_sensor_config_set)
    postSensorInfoRequest();

  if(m_hit_communicate_point) {
    if(m_need_station_keep)
      Notify("STATION", "true");
    m_need_station_keep = true;
    handleMailSend2Other();
    m_hit_communicate_point = false;
  }

  if(m_survey_redetect) {
    postHistoryDetectList();
    m_survey_redetect = false;
  }

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool HazardMgrX::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(true);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = tolower(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if((param == "swath_width") && isNumber(value)) {
      m_swath_width_desired = atof(value.c_str());
      handled = true;
    }
    else if(((param == "sensor_pd") || (param == "pd")) && isNumber(value)) {
      m_pd_desired = atof(value.c_str());
      handled = true;
    }
    else if(param == "report_name") {
      value = stripQuotes(value);
      m_report_name = value;
      handled = true;
    }
    else if(param == "region") {
      XYPolygon poly = string2Poly(value);
      if(poly.is_convex())
	m_search_region = poly;
      handled = true;
    }
    else if(param == "second_width"){
        m_sec_detect_width = value;
      handled = true;
    }
    else if(param == "second_pd"){
        m_sec_pd = value;
        handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);
  }
  
  m_hazard_set.setSource(m_host_community);
  m_hazard_set.setName(m_report_name);
  m_hazard_set.setRegion(m_search_region);
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void HazardMgrX::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("UHZ_DETECTION_REPORT", 0);
  Register("UHZ_CONFIG_ACK", 0);
  Register("UHZ_OPTIONS_SUMMARY", 0);
  Register("UHZ_MISSION_PARAMS", 0);
  Register("HAZARDSET_REQUEST", 0);
  Register("COL_RESULT",0);
  Register("WPT_INDEX",0);
  Register("ARRIVE",0);
  Register("SURVEY",0);
  Register("UHZ_HAZARD_REPORT",0);

}

//---------------------------------------------------------
// Procedure: postSensorConfigRequest

void HazardMgrX::postSensorConfigRequest()
{
  string request = "vname=" + m_host_community;
  
  request += ",width=" + doubleToStringX(m_swath_width_desired,2);
  request += ",pd="    + doubleToStringX(m_pd_desired,2);

  m_sensor_config_requested = true;
  m_sensor_config_reqs++;
  Notify("UHZ_CONFIG_REQUEST", request);
}

//---------------------------------------------------------
// Procedure: postSensorInfoRequest

void HazardMgrX::postSensorInfoRequest()
{
  string request = "vname=" + m_host_community;

  m_sensor_report_reqs++;
  Notify("UHZ_SENSOR_REQUEST", request);
}

void HazardMgrX::postHistoryDetectList()
{
  // send detect repo to Hazard Path
  deque<string>::iterator it;
  string repo;
  for(it=m_history_detect_buff.begin(); it!=m_history_detect_buff.end(); ++it) {
    string &detect_point = *it;
    repo += "#" + detect_point;
  }
  reportEvent(repo);
  Notify("HISTORY_DETEC_LIST", repo);
}

//---------------------------------------------------------
// Procedure: handleMailSensorConfigAck

bool HazardMgrX::handleMailSensorConfigAck(string str)
{
  // Expected ack parameters:
  string vname, width, pd, pfa, pclass;
  
  // Parse and handle ack message components
  bool   valid_msg = true;
  string original_msg = str;

  vector<string> svector = parseString(str, ',');
  unsigned int i, vsize = svector.size();
  for(i=0; i<vsize; i++) {
    string param = biteStringX(svector[i], '=');
    string value = svector[i];

    if(param == "vname")
      vname = value;
    else if(param == "pd")
      pd = value;
    else if(param == "width")
      width = value;
    else if(param == "pfa")
      pfa = value;
    else if(param == "pclass")
      pclass = value;
    else
      valid_msg = false;       

  }


  if((vname=="")||(width=="")||(pd=="")||(pfa=="")||(pclass==""))
    valid_msg = false;
  
  if(!valid_msg)
    reportRunWarning("Unhandled Sensor Config Ack:" + original_msg);

  
  if(valid_msg) {
    m_sensor_config_set = true;
    m_sensor_config_acks++;
    m_swath_width_granted = atof(width.c_str());
    m_pd_granted = atof(pd.c_str());
  }

  return(valid_msg);
}

//---------------------------------------------------------
// Procedure: handleMailDetectionReport
//      Note: The detection report should look something like:
//            UHZ_DETECTION_REPORT = vname=betty,x=51,y=11.3,label=12 

bool HazardMgrX::handleMailDetectionReport(string str)
{
  m_detection_reports++;

  XYHazard new_hazard = string2Hazard(str);
  new_hazard.setType("hazard");

  string hazlabel = new_hazard.getLabel();
  
  if(hazlabel == "") {
    reportRunWarning("Detection report received for hazard w/out label");
    return(false);
  }

  int ix = m_hazard_set.findHazard(hazlabel);
  if(ix == -1)
    m_hazard_set.addHazard(new_hazard);
  else
    m_hazard_set.setHazard(ix, new_hazard);

  string event = "New Detection, label=" + new_hazard.getLabel();
  event += ", x=" + doubleToString(new_hazard.getX(),1);
  event += ", y=" + doubleToString(new_hazard.getY(),1);

  //reportEvent(event);
    if(m_self_result){
        string req = "vname=" + m_host_community + ",label=" + hazlabel;

        Notify("UHZ_CLASSIFY_REQUEST", req);
    }
    else{
    
        m_self_result = true;    // change back to true
    }
  return(true);
}


//---------------------------------------------------------
// Procedure: handleMailReportRequest

void HazardMgrX::handleMailReportRequest()
{
  m_summary_reports++;

  m_hazard_set.findMinXPath(20);
  //unsigned int count    = m_hazard_set.findMinXPath(20);
  //  string summary_report_local = m_hazard_set.getSpec("final_report");

  //handleMailSend2Other();
   
  //checking if the label is the same

        string summary_report = m_hazard_set.getSpec("final_report");
          //  Notify("HAZARDSET_REPORT", summary_report);
            Notify("HAZARDSET_REPORT", summary_report);
    
}

// modify by YHH for merging two vehicle report
void HazardMgrX::handleMailSend2Other()
{
//sending report to another vehicle  
   string hostname=m_report_name;      
   string dest_name="all";     
   string moos_varname="COL_RESULT";
   string msg_contents="\"";
   bool ishazard=true; 
   
   if(!m_detection_reports_str_buff.empty()){
        
       while(!m_detection_reports_str_buff.empty()){
        bool repetitive = false;
        string input = m_detection_reports_str_buff.front();
    
         if(!m_history_detect_buff.empty()){
           for(int i=0;i<m_history_detect_buff.size();){
               if( input == m_history_detect_buff[i]){
                repetitive = true;
                break;
            }
                else{
                i++;
                }
           }
         }

           if(!repetitive){        //not repetitive and it's hazard
               
               m_history_detect_buff.push_back(input);
           }
                m_detection_reports_str_buff.pop_front();
        } 
   }
   if(!m_output_buff.empty()){
       
       for(int i=0;i<m_output_buff.size();i++){    
        
        if(!m_output_buff.empty()){
          string elemet = m_output_buff.front();
              msg_contents += elemet;
              m_output_buff.pop_front();
            if(!m_output_buff.empty())
             msg_contents+="#";
        }
        if(i>4)
            break;
       }
             msg_contents+="\"";
        string msg;
        msg += "src_node="   + hostname;
        msg += ",dest_node=" + dest_name;
        msg += ",var_name="  + moos_varname;
        msg += ",string_val="+ msg_contents; 

            Notify("NODE_MESSAGE_LOCAL", msg);
       }

}

//---------------------------------------------------------

void HazardMgrX::getHazardToOutput(string input){

   
   size_t type_index = input.find("type=");
   string type;
   if(type_index!=string::npos)
    type = input.substr(type_index);

   if(type == "type=hazard"){
       string hazard = input.substr(0,type_index-1);
    m_output_buff.push_back(hazard);
   }
}

//---------------------------------------------------------
// Procedure: handleMailMissionParams
//   Example: UHZ_MISSION_PARAMS = penalty_missed_hazard=100,               
//                       penalty_nonopt_hazard=55,                
//                       penalty_false_alarm=35,                  
//                       penalty_max_time_over=200,               
//                       penalty_max_time_rate=0.45,              
//                       transit_path_width=25,                           
//                       search_region = pts={-150,-75:-150,-50:40,-50:40,-75}


void HazardMgrX::handleMailMissionParams(string str)
{
  vector<string> svector = parseStringZ(str, ',', "{");
  unsigned int i, vsize = svector.size();
  for(i=0; i<vsize; i++) {
    string param = biteStringX(svector[i], '=');
    string value = svector[i];
    // This needs to be handled by the developer. Just a placeholder.
  }
}


//------------------------------------------------------------
// Procedure: buildReport()

bool HazardMgrX::buildReport() 
{
  m_msgs << "Config Requested:"                                  << endl;
  m_msgs << "    swath_width_desired: " << m_swath_width_desired << endl;
  m_msgs << "             pd_desired: " << m_pd_desired          << endl;
  m_msgs << "   config requests sent: " << m_sensor_config_reqs  << endl;
  m_msgs << "                  acked: " << m_sensor_config_acks  << endl;
  m_msgs << "------------------------ "                          << endl;
  m_msgs << "Config Result:"                                     << endl;
  m_msgs << "       config confirmed: " << boolToString(m_sensor_config_set) << endl;
  m_msgs << "    swath_width_granted: " << m_swath_width_granted << endl;
  m_msgs << "             pd_granted: " << m_pd_granted          << endl << endl;
  m_msgs << "--------------------------------------------" << endl << endl;

  m_msgs << "               sensor requests: " << m_sensor_report_reqs << endl;
  m_msgs << "             detection reports: " << m_detection_reports  << endl << endl; 

  m_msgs << "   Hazardset Reports Requested: " << m_summary_reports << endl;
  m_msgs << "      Hazardset Reports Posted: " << m_summary_reports << endl;
  m_msgs << "                   Report Name: " << m_report_name << endl;
  m_msgs << "                    Message in: " << m_message_in<<endl;
  m_msgs << "History Buffer Size:"<<m_history_detect_buff.size()<<endl;
  m_msgs << "Detection Buffer Size:"<<m_detection_reports_str_buff.size()<<endl;
  m_msgs << "Output Buffer Size:"<<m_output_buff.size()<<endl;
  m_msgs << "--------------------------------------------" << endl << endl;
  m_msgs << "Need Station Kepp: " << m_need_station_keep << endl;
  m_msgs << "Hit Communicate Point: " << m_hit_communicate_point << endl;
  return(true);
}





