/************************************************************/
/*    NAME: yhh                                              */
/*    ORGN: MIT                                             */
/*    FILE: FrontEstimateX.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "FrontEstimateX.h"
#include <deque>
using namespace std;

//---------------------------------------------------------
// Constructor

FrontEstimateX::FrontEstimateX()
{
    m_input_index   = 0;
    m_sending_start_index = 0;
    m_report_name   = "";
    m_output_report = "";
    m_input_buff.clear();
    m_recieve = false;
}

//---------------------------------------------------------
// Destructor

FrontEstimateX::~FrontEstimateX()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool FrontEstimateX::OnNewMail(MOOSMSG_LIST &NewMail)
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

        else if(key == "UCTD_MSMNT_REPORT"){
            m_input_buff.push_back(msg.GetString());
            m_input_index +=1;
        }
        else if(key == "COL_RESULT"){
          m_col_msg_buff.push_back(msg.GetString());
          m_recieve = true; 
        }
     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
     } 
        
         
     
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool FrontEstimateX::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool FrontEstimateX::ShareCommReport(){

   
   if(m_output_report!=""){ 
    string hostname=m_report_name;      
    string dest_name="all";     
    string moos_varname="COL_RESULT";
    string msg_contents="\"";


    msg_contents +=m_output_report;
    msg_contents +="\"";

    string msg;
    msg += "src_node="   + hostname;
    msg += ",dest_node=" + dest_name;
    msg += ",var_name="  + moos_varname;
    msg += ",string_val="+ msg_contents; 

    Notify("NODE_MESSAGE_LOCAL", msg);
    m_output_report = "";
   }
};

bool FrontEstimateX::GenCommReport(std::string report){

    if(m_output_report!="")
      m_output_report += "#";

    m_output_report +=report;
};

bool FrontEstimateX::Iterate()
{
  AppCastingMOOSApp::Iterate();
//Build and share report

  
  if(!m_input_buff.empty()){
      bool ele_less_than_ten = false; 
      int  start_index; 
      for(int i=m_sending_start_index;i<m_sending_start_index+10;i++){ 
        
        if(i>=m_input_buff.size()){
         start_index = i;
         ele_less_than_ten = true;
            break;
        }
        GenCommReport(m_input_buff[i]);
      }
        if(m_recieve){
       
          if(ele_less_than_ten)
              m_sending_start_index = start_index;
          else
              m_sending_start_index +=10;
          
          m_recieve = false;
        }

  }
      ShareCommReport();  
  
// Handle the report from other vehicle
   while(!m_col_msg_buff.empty()){

       vector<string> str_vector = parseString(m_col_msg_buff.front(),'#');
       m_col_msg_buff.pop_front();
       
       for(int i=0;i<str_vector.size();i++){
           size_t found_vname = str_vector[i].find("vname=");
           size_t found_comm  = str_vector[i].find(",");
           str_vector[i].erase(found_vname+6,found_comm-found_vname-6);
           str_vector[i].insert(found_vname+6,m_report_name);
          // Notify("UCTD_MSMNT_REPORT",str_vector[i]);
           Notify("UCTD_MSMNT_REPORT",str_vector[i]);
       }
   } 

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool FrontEstimateX::OnStartUp()
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
    else if(param == "bar") {
      handled = true;
    }
    else if(param == "report_name")
    {
        m_report_name = value;
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

void FrontEstimateX::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  Register("UCTD_MSMNT_REPORT",0);
  Register("COL_RESULT",0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool FrontEstimateX::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "File:                                        \n";
  m_msgs << "============================================ \n";

  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();
  m_msgs <<"m_sending_start_index="<<m_sending_start_index<<endl;
  return(true);
}




