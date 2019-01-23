/************************************************************/
/*    NAME: yhhuang                                         */
/*    ORGN: NTU, Taipei                                     */
/*    FILE: SimAngle.cpp                                    */
/*    DATE: Jan 16th, 2019                                  */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "SimAngle.h"

using namespace std;

//---------------------------------------------------------
// Constructor

SimAngle::SimAngle()
{
    m_src_x = 0;
    m_src_y = 0;
    m_current_x = 0;
    m_current_y = 0;
    m_current_heading = 0;
    m_source_angle = 0;
}

//---------------------------------------------------------
// Destructor

SimAngle::~SimAngle()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool SimAngle::OnNewMail(MOOSMSG_LIST &NewMail)
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
     
     else if(key =="NAV_X")
        m_current_x = msg.GetDouble();
     else if(key =="NAV_Y")
        m_current_y = msg.GetDouble();
     else if(key =="NAV_HEADING")
        m_current_heading = msg.GetDouble();
     else if(key =="SIM_SOURCE_X")
         m_src_x = msg.GetDouble();
     else if(key == "SIM_SOURCE_Y")
         m_src_y = msg.GetDouble();
     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool SimAngle::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool SimAngle::Iterate()
{
  AppCastingMOOSApp::Iterate();

//output pulse as source in pMarineViewer
    PulseOut();
//calculate relative coordinate

//check left or right

//check angle and output

  AppCastingMOOSApp::PostReport();
  return(true);
}

bool SimAngle::PulseOut()
{
    stringstream ss;
    ss<<"x="<<m_src_x<<",y="<<m_src_y<<"radius=15,duration=1,fill=0.5,label=";
    ss<<"sound_source,edge_color="<<m_edge_color<<",fill_color="<<m_fill_color;
    ss<<",edge_size=1";

    Notify("VIEW_RANGE_PULSE",ss.str());
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool SimAngle::OnStartUp()
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
    else if(param == "source_x"){
        m_src_x = atof(value.c_str());
        handled = true;
    }
    else if(param == "source_y"){
        m_src_y = atof(value.c_str());
        handled = true;
    }
    else if(param == "edge_color"){
        m_edge_color = value;
        handled = true;
    }
    else if(param == "fill_color"){
        m_fill_color = value;
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

void SimAngle::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
    Register("NAV_X",0);
    Register("NAV_Y",0);
    Register("NAV_HEADING",0);
    Register("SIM_SOURCE_X",0);
    Register("SIM_SOURCE_Y",0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool SimAngle::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "Source location:                             \n";
  m_msgs << "   x="<<m_src_x<<"\n";
  m_msgs << "   y="<<m_src_y<<"\n";
  m_msgs << "============================================ \n";
  m_msgs << "   source angle ="<<m_source_angle<<"\n";


  return(true);
}




