/************************************************************/
/*    NAME: YHHuang                                              */
/*    ORGN: MIT                                             */
/*    FILE: AISdecoder.cpp                                        */
/*    DATE: MAR 19th 2018                             */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "AISdecoder.h"
#include <sstream>
using namespace std;

//---------------------------------------------------------
// Constructor

AISdecoder::AISdecoder()
{
    m_input_buff.clear();
}

//---------------------------------------------------------
// Destructor

AISdecoder::~AISdecoder()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool AISdecoder::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

    if(key == "AIS_DATA"){
        
        string origin = msg.GetString();
        AisNode input;
        input.setOrin(origin);

        m_input_buff.push_back(input);
    }

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

     else if(key == "FOO") 
       cout << "great!";

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool AISdecoder::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool AISdecoder::Iterate()
{
  AppCastingMOOSApp::Iterate();
  
  if(!m_input_buff.empty()){
  
    
      AisNode input_node = m_input_buff.front();
      input_node.analysis();
      input_node.getLon();
  
  }
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool AISdecoder::OnStartUp()
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

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void AISdecoder::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
     Register("AIS_DATA",0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool AISdecoder::buildReport() 
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




