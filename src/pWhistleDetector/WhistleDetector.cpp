/************************************************************/
/*    NAME: yhhuang                                         */
/*    ORGN: NTU, Taiwan                                     */
/*    FILE: WhistleDetector.cpp                                */
/*    DATE: Aug 17th 2018                                   */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "WhistleDetector.h"
#include <armadillo>
#include "STFT.h"
using namespace std;
using namespace arma;

//---------------------------------------------------------
// Constructor

WhistleDetector::WhistleDetector()
{
    m_input_data.clear();

    m_overlap       = 0.9;
    m_window_length = 2048;
    m_fs            = 96000;
    m_window_length = (double)m_fs/46.875;
    m_bits          = 32;
    m_iterate_data  = 1;   // 1 second
    m_update_percent= 0.5;
    m_window_type   = "hanning";
    m_whistle_exist = false;
}


//---------------------------------------------------------
// Destructor

WhistleDetector::~WhistleDetector()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool WhistleDetector::OnNewMail(MOOSMSG_LIST &NewMail)
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
//     else if(key == "SOUND_VOLTAGE_DATA_CH_ONE"){
     else if(key == "SOUND_VOLTAGE_DATA_CH_ONE"){
      m_input_data.push_back(msg.GetString());   
     }
     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool WhistleDetector::OnConnectToServer()
{
   registerVariables();
   return(true);
}

bool WhistleDetector::GetVoltageData(std::string input)
{

    vector<string> voltages = parseString(input,',');

    for(int i=0; i<voltages.size(); i++){
      float volt = atoi(voltages[i].c_str());
      
//      if(m_voltage_data.size() >= sample_number)
//        m_voltage_data.pop_front();

      m_voltage_data.push_back(volt);
    }
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool WhistleDetector::Iterate()
{
  AppCastingMOOSApp::Iterate();

//Access data -> "m_iterate_data" seconds data per Iterate loop
  std::string   input_str_voltage;
  int           access_data_number = round(m_fs*m_iterate_data);


  vector<float> input_x(access_data_number,0);
  mat           P; 
//Get String data and transfer to voltage buffer
  if(!m_input_data.empty()){
    input_str_voltage = m_input_data.front();
    GetVoltageData(input_str_voltage); 
    m_input_data.pop_front();
  }

//set up window
    int win_number;
    if(m_window_type == "rectagular")
        win_number = 0;
    else 
        win_number = 1; 

    if(!m_voltage_data.empty()){
        stringstream ss;
        ss<<m_voltage_data.size();
        reportEvent("m_voltage_data size:" + ss.str());
//fft   
            if(m_voltage_data.size() >= access_data_number){
                for(int i=0;i<access_data_number;i++)
                    input_x[i] = m_voltage_data[i];
            
               
            P = STFT_with_FFTW3f(input_x,m_fs,m_window_length,m_overlap,win_number);
// detect_whistle algorithm
            detect_whistle(P);
//check if there is whistle in the matrix
            m_whistle_exist = any(vectorise(P));
            stringstream ss_exist;
            ss_exist<<m_whistle_exist;
            reportEvent("m_whistle_exist:" + ss_exist.str());

//remove calculated data 
            m_voltage_data.erase(m_voltage_data.begin(),m_voltage_data.begin()+round(access_data_number*1));
           
            }         
    }
        if(m_whistle_exist){
            Notify("WHISTLE_EXIST","true");
            Notify("SOUND_VOLTAGE_DATA_WITH_WHISTLE",input_str_voltage);
        }
        else 
            Notify("WHISTLE_EXIST","false");
                 
    AppCastingMOOSApp::PostReport();
    return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool WhistleDetector::OnStartUp()
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
    else if(param == "overlap"){
      
      stringstream ss;
      ss<<value;
      ss>>m_overlap;

      handled = true;
    }
    else if(param == "bits"){
      stringstream ss;
      ss<<value;
      ss>>m_bits;

      handled = true;
    }
    else if(param == "sample_rate"){
      stringstream ss;
      ss<<value;
      ss>>m_fs;
      
      m_window_length = (double)m_fs/46.875;

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
    else if(param == "data_update_second"){
        stringstream ss;
        ss<<value;
        ss>>m_update_percent;
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

void WhistleDetector::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
   Register("ACOUSTIC_DATA", 0);
   Register("SOUND_VOLTAGE_DATA_CH_ONE", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool WhistleDetector::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "File:                                        \n";
  m_msgs << "============================================ \n";

  stringstream ss;
  string    iterate_data_second;
  ss<<m_iterate_data;
  ss>>iterate_data_second;

  ACTable actab(4);

  if(m_whistle_exist)
    m_msgs << "Whistle exist!!";
  else
    m_msgs << "No whistle in this "<<iterate_data_second<<" seconds\n";

    m_msgs << actab.getFormattedString();

  return(true);
}




