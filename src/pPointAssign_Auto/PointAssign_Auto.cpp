/************************************************************/
/*    NAME: YHHuang                                              */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign_Auto.cpp                                        */
/*    DATE: MAR 20th 2018                                  */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "PointAssign_Auto.h"
#include <unistd.h>

using namespace std;

//---------------------------------------------------------
// Constructor

PointAssign_Auto::PointAssign_Auto()
{
	m_visit_point_buffer.clear();
    m_visit_point_buff_v1.clear();
    m_visit_point_buff_v2.clear();    

    m_vname1 ="";
    m_vname2 ="";
    m_region ="false";
    m_input_index = 0;
}

//---------------------------------------------------------
// Destructor

PointAssign_Auto::~PointAssign_Auto()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PointAssign_Auto::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

     if(key == "FOO") 
       cout << "great!";
	else if(key == "VISIT_POINT"){
       string input_str = msg.GetString();
       m_visit_point_buffer.push_back(input_str);  //get vistit point and push in input buffer
        m_input_index+=1; 
     }

     else if(key == "NODE_REPORT"){
         
        m_node_report_local = msg.GetString();
     }

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PointAssign_Auto::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PointAssign_Auto::Iterate()
{
  AppCastingMOOSApp::Iterate();

       string str_active_utimerscript = "true";   //Let uTimersrcript be active
    
      if(m_node_report_local!=""){
    
      
       vector<string> parse_result;

     parse_result = parseString(m_node_report_local,',');
        
        string name = parse_result[0];
        
        size_t equal = name.find("=");  // NAME=.....
        
        m_vname1 = name.substr(equal+1);
        m_vname2 = "ss";
    
        string visit_point_vehicle1 = "VISIT_POINT_"+ toupper(m_vname1);
        string visit_point_vehicle2 = "VISIT_POINT_"+ toupper(m_vname2);

 
     Notify("POINT_ASSIGN",str_active_utimerscript);

      

          
     if(m_region == "true"){   //assign by region
       if(!m_visit_point_buffer.empty()){
         string visit_point = m_visit_point_buffer.front();
         m_visit_point_buffer.pop_front();
         size_t m_found_x  = visit_point.find("x=");   //find x=
             size_t  m_found_c1 = visit_point.find(",");    //find first,
                    if(m_found_x !=string::npos && m_found_c1 != string::npos){
                         string sub_x1 = visit_point.substr(m_found_x+2,m_found_c1-m_found_x-2);
        // cout<<sub_x1<<endl;           
                                double x;
                                stringstream x_ss, y_ss;
                                x_ss<<sub_x1;
                                x_ss>>x;
                            
                                if(x>80){  //middle x
                                   m_visit_point_buff_v1.push_back(visit_point); 
                                  
                                 Notify(visit_point_vehicle1,visit_point);
                                }
                                else{
                                   m_visit_point_buff_v2.push_back(visit_point);
                                 Notify(visit_point_vehicle2,visit_point);
                                }
                     }
                    if(m_visit_point_buffer.front()=="lastpoint"){
                        
                          m_visit_point_buff_v1.push_back("lastpoint");
                          m_visit_point_buff_v2.push_back("lastpoint");
                                 Notify(visit_point_vehicle1,"lastpoint");
                                 Notify(visit_point_vehicle2,"lastpoint");
                          
                    }
        }
       }
          
     //not assign by region 
     else{
     
        if(!m_visit_point_buffer.empty()){
         string visit_point = m_visit_point_buffer.front();
         m_visit_point_buffer.pop_front();
         size_t m_found_id  = visit_point.find("id=");   //find id=
                    if(m_found_id !=string::npos){
                         string sub_id = visit_point.substr(m_found_id+3);
                    
                                int id;
                                stringstream id_ss;
                                id_ss<<sub_id;
                                id_ss>>id;
                            //odd ID to vehicle1, even ID to vehicle2
                                if((id%2)==1){
                                 m_visit_point_buff_v1.push_back(visit_point);
                                 Notify(visit_point_vehicle1,visit_point);
                                }
                                else{
                                 m_visit_point_buff_v2.push_back(visit_point);
                                 Notify(visit_point_vehicle2,visit_point);
                                }
                     }
                    if(m_visit_point_buffer.front()=="lastpoint"){
                   
                        m_visit_point_buff_v1.push_back("lastpoint");
                        m_visit_point_buff_v2.push_back("lastpoint");     
                                 Notify(visit_point_vehicle1,"lastpoint");
                                 Notify(visit_point_vehicle2,"lastpoint");
                        //Both vehicle need lastpoint to do the work
                    }
        }
     
     }
      
    
//        for(int i=0;i<m_visit_point_buff_v1.size();i++){
   //         sleep(1);            
//        }
        
//        for(int i=0;i<m_visit_point_buff_v2.size();i++){
    //        sleep(1);
//            Notify(visit_point_vehicle2,m_visit_point_buff_v2[i]);
//        }
    
    }

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PointAssign_Auto::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = tolower(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "foo") {
      handled = true;
    }
    if(param == "assign_by_region") {
        m_region = value;
        handled  = true;
    }
    else if(param == "bar") {
      handled = true;
    }
    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void PointAssign_Auto::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
   // Register("FOOBAR", 0);
      Register("VISIT_POINT",0);
      Register("NODE_REPORT",0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool PointAssign_Auto::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "File:                                        \n";
  m_msgs << "============================================ \n";

  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();
  return(true);
}




