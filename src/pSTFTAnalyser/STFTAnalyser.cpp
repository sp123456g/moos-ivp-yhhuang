/************************************************************/
/*    NAME: yhhuang                                         */
/*    ORGN: MIT                                             */
/*    FILE: STFTAnalyser.cpp                                */
/*    DATE: May 29th 2018                                   */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "STFTAnalyser.h"
#include <armadillo>
#include "STFT.h"
using namespace std;

//---------------------------------------------------------
// Constructor

STFTAnalyser::STFTAnalyser()
{
    m_input_data.clear();

    m_dt            = 0.01;
    m_df            = 20;
    m_sgm           = 6000;
    m_Bandwidth     = 0.02;
    m_fs            = 44100;
    m_threshold     = 30;
    m_do_dectect    = true;
    m_iterate_data  = 1;   // 1 second
    m_window_type   = "rec";
}

//---------------------------------------------------------
// Destructor

STFTAnalyser::~STFTAnalyser()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool STFTAnalyser::OnNewMail(MOOSMSG_LIST &NewMail)
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
     else if(key == "ACOUSTIC_DATA"){
      m_input_data.push_back(msg.GetDouble());   
     }
     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool STFTAnalyser::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool STFTAnalyser::Iterate()
{
  AppCastingMOOSApp::Iterate();

//step1: Check the average SPL of ambient noise 
//step2: Check if the SPL of the point is between AVG_SPL+2db ~ AVG_SPL+10db
//or over than AVG_SPL+10
//step3: if over AVG_SPL+10, check SEL, if between, go to step 4  
//step4: Access data -> "m_iterate_data" seconds data per Iterate loop

//x is the data need to analysis
    int access_data_number = round(m_fs*m_iterate_data);
//    
    vec x = zeros<vec>(access_data_number);
//
//   //for single channel: using i++
//   //for stereo channel: using i+=2
    if(m_input_data.size()>=access_data_number){
        for(int i=0;i<access_data_number;i++){
            x(i) = m_input_data[i];
        }
//erase the data which already used
  
        m_input_data.erase(m_input_data.begin(),m_input_data.begin()+access_data_number); 
    
        mat P;

//using Gabor 
        if(m_window_type == "Gassian")
            P   = Gabor(x,m_fs,m_sgm,m_dt,m_df);   // P: row is time and column is frequency
//Using recSTFT if want
        else if(m_window_type == "rec")
            P   = recSTFT_new(x,m_fs,m_Bandwidth,m_dt,m_df);   // P: row is time and column is frequency                     
//put detection algorithm in 
        if(m_do_dectect == "true"){
//set a threshold to catch the chirp data
            detect_whistle(P,m_threshold); 
        }
    }
    AppCastingMOOSApp::PostReport();
    return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool STFTAnalyser::OnStartUp()
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
    else if(param == "dt"){
      
      stringstream ss;
      ss<<value;
      ss>>m_dt;

      handled = true;
    }
    else if(param == "df"){
      stringstream ss;
      ss<<value;
      ss>>m_df;

      handled = true;
    }
    else if(param == "sigma"){
      stringstream ss;
      ss<<value;
      ss>>m_sgm;

      handled = true;
    }
    else if(param == "bandwidth"){
      stringstream ss;
      ss<<value;
      ss>>m_Bandwidth;

      handled = true;
    }
    else if(param == "sample_rate"){
      stringstream ss;
      ss<<value;
      ss>>m_fs;

      handled = true;
    }
    else if(param == "detection_threshold"){
      stringstream ss;
      ss<<value;
      ss>>m_threshold;

      handled = true;
    }
    else if(param == "do_detect"){
      m_do_dectect = value;
      handled = true;
    }
    else if(param == "iterate_data"){
      stringstream ss;
      ss<<value;
      ss>>m_iterate_data;

      handled = true;
    }
    else if(param == "window_type"){
      m_window_type = value;
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

void STFTAnalyser::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
   Register("ACOUSTIC_DATA", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool STFTAnalyser::buildReport() 
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




