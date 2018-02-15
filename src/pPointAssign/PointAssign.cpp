/************************************************************/
/*    NAME: YHHUANG                                        */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "PointAssign.h"
using namespace std;

//---------------------------------------------------------
// Constructor

PointAssign::PointAssign()
{
    visit_point_buffer.clear();
    
    m_vname1 ="";
    m_vname2 ="";
    m_region ="false";
    m_x_array.clear();
    m_middle_x = 0;
    m_input_index = 0;
    m_x_array_input_index = 0;
}

//---------------------------------------------------------
// Destructor

PointAssign::~PointAssign()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PointAssign::OnNewMail(MOOSMSG_LIST &NewMail)
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

       
     if(key == "VISIT_POINT") 
     {
       string input_str = msg.GetString();
       visit_point_buffer.push_back(input_str);  //get vistit point and push in input buffer
        m_input_index+=1; 
     }
     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PointAssign::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second


bool PointAssign::Iterate()
{
  AppCastingMOOSApp::Iterate();

     string str_active_utimerscript = "true";   //Let uTimersrcript be active
     Notify("POINT_ASSIGN",str_active_utimerscript);

     string visit_point_vehicle1 = "VISIT_POINT_"+ toupper(m_vname1);
     string visit_point_vehicle2 = "VISIT_POINT_"+ toupper(m_vname2);
     
     if(m_region == "true"){   //assign by region
       if(!visit_point_buffer.empty()){
         string visit_point = visit_point_buffer.front();
         visit_point_buffer.pop_front();
         size_t m_found_x  = visit_point.find("x=");   //find x=
             size_t  m_found_c1 = visit_point.find(",");    //find first,
                    if(m_found_x !=string::npos && m_found_c1 != string::npos){
                         string sub_x1 = visit_point.substr(m_found_x+2,m_found_c1-m_found_x-2);
         cout<<sub_x1<<endl;           
                                double x;
                                stringstream x_ss, y_ss;
                                x_ss<<sub_x1;
                                x_ss>>x;
                            
                                if(x>80){
                                 Notify(visit_point_vehicle1,visit_point);
                                }
                                else{
                                 Notify(visit_point_vehicle2,visit_point);
                                }
                     }
                    if(visit_point_buffer.front()=="lastpoint"){
                        Notify(visit_point_vehicle1,"lastpoint");
                        Notify(visit_point_vehicle2, "lastpoint");
                    }
        }
       }
          
     //not assign by region 
     else{
     
        if(!visit_point_buffer.empty()){
         string visit_point = visit_point_buffer.front();
         visit_point_buffer.pop_front();
         size_t m_found_id  = visit_point.find("id=");   //find id=
                    if(m_found_id !=string::npos){
                         string sub_id = visit_point.substr(m_found_id+3);
                    
                                int id;
                                stringstream id_ss;
                                id_ss<<sub_id;
                                id_ss>>id;
                            
                                if((id%2)==1){
                                 Notify(visit_point_vehicle1,visit_point);
                                }
                                else{
                                 Notify(visit_point_vehicle2,visit_point);
                                }
                     }
                    if(visit_point_buffer.front()=="lastpoint"){
                        Notify(visit_point_vehicle1,"lastpoint");
                        Notify(visit_point_vehicle2, "lastpoint");
                    }
        }
     
     }  
  // Do your thing here!
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PointAssign::OnStartUp()
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
    string param = toupper(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "ASSIGN_BY_REGION") {
        m_region = value;
        handled  = true;
   //     reportEvent(value);
    }
    else if(param == "VNAME1"){
        m_vname1 = value;
        handled  = true;
    }
    else if(param == "VNAME2"){
        m_vname2 = value;
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

void PointAssign::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
   
     Register("VISIT_POINT",0);

  // Register("FOOBAR", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool PointAssign::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "File:                                        \n";
  m_msgs << "============================================ \n";
  m_msgs << "m_vname1="<<m_vname1<<"\n";   
  m_msgs << "m_vname2="<<m_vname2<<"\n";   
  m_msgs << "m_region="<<m_region<<"\n";   
  m_msgs << "buffer size:"<<visit_point_buffer.size()<<"\n";
  m_msgs << "m_x_array_input_index="<<m_x_array_input_index<<"\n";
  m_msgs << "m_input_index="<<m_input_index<<"\n";
  return(true);
}




