/************************************************************/
/*    NAME: yhhuang                                              */
/*    ORGN: MIT                                             */
/*    FILE: Spectrogram.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "Spectrogram.h"
#include "sigpack.h"
#include "spectrogramshow.h"
using namespace std;

//---------------------------------------------------------
// Constructor

Spectrogram::Spectrogram()
{
}

//---------------------------------------------------------
// Destructor

Spectrogram::~Spectrogram()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Spectrogram::OnNewMail(MOOSMSG_LIST &NewMail)
{

  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Spectrogram::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Spectrogram::Iterate()
{

    AppCastingMOOSApp::Iterate();

    spectrogramshow();    

    AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Spectrogram::OnStartUp()
{

    AppCastingMOOSApp::OnStartUp();

  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      
      if(param == "FOO") {
        //handled
      }
      else if(param == "BAR") {
        //handled
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void Spectrogram::RegisterVariables()
{
  // Register("FOOBAR", 0);
    AppCastingMOOSApp::RegisterVariables();
}

bool Spectrogram::buildReport(){

return true;
}
