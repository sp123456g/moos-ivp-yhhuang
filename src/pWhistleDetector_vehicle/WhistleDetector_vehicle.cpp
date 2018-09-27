/************************************************************/
/*    NAME: yhhuang                                         */
/*    ORGN: NTU, Taiwan                                     */
/*    FILE: WhistleDetector_vehicle.cpp                                */
/*    DATE: Aug 17th 2018                                   */
/************************************************************/
#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "WhistleDetector_vehicle.h"
#include "detection_algorithm.h"
using namespace std;

//---------------------------------------------------------
// Constructor

WhistleDetector_vehicle::WhistleDetector_vehicle()
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
    m_SNR_threshold = 10;
    m_frq_low       = 3000;
    m_frq_high      = 10000;
}


//---------------------------------------------------------
// Destructor

WhistleDetector_vehicle::~WhistleDetector_vehicle()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool WhistleDetector_vehicle::OnNewMail(MOOSMSG_LIST &NewMail)
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
     else if(key == "SOUND_VOLTAGE_DATA_CH_ONE"){
//Get String data and transfer to voltage buffer
        GetVoltageData(msg.GetString()); 
     }
     else if(key == "TEST_MESSAGE"){
        m_testing_message = msg.GetString();
     }
     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool WhistleDetector_vehicle::OnConnectToServer()
{
   registerVariables();
   return(true);
}

bool WhistleDetector_vehicle::GetVoltageData(std::string input)
{

    vector<string> voltages = parseString(input,',');

    for(int i=0; i<voltages.size(); i++){
      float volt = atof(voltages[i].c_str());
      
      m_voltage_data.push_back(volt);
    }
}

bool WhistleDetector_vehicle::Analysis(vector<float> input_data)
{

    vector<vector<float> > P;
//set up window
    int win_number;
    if(m_window_type == "rectagular")
        win_number = 0;
    else 
        win_number = 1; 
//STFT
    P = STFT_with_FFTW3f(input_data,m_fs,m_window_length,m_overlap,win_number);

//Whistle detection
    detect_whistle(P,m_fs,m_window_length,m_overlap,m_SNR_threshold,m_frq_low,m_frq_high);
//check if there is whistle in the matrix
        vector<whistle> whistle_list;
        whistle_list = check_result(P,m_fs,m_window_length,m_overlap);
    m_whistle_exist = !whistle_list.empty();
// Check properties of whistles
// ----------------------------------------------------------------
    if(m_whistle_exist){

        Notify("WHISTLE_EXIST","true");

        for(int i=0;i<whistle_list.size();i++){
            stringstream ss_1, ss_2,ss_3,ss_4;
            ss_1<<i;
            ss_2<<whistle_list[i].start_frq;
            ss_3<<whistle_list[i].end_frq;
            reportEvent("whistle_"+ ss_1.str());
            reportEvent("start frq = " + ss_2.str());
            reportEvent("end frq = " + ss_3.str());
        }
    }
//-----------------------------------------------------------------
    else 
        Notify("WHISTLE_EXIST","false");
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool WhistleDetector_vehicle::Iterate()
{
  AppCastingMOOSApp::Iterate();

//Access data -> "m_iterate_data" seconds data per Iterate loop
  if(m_first_time){
    m_access_data_number = round(m_fs*m_iterate_data);

    if(m_frq_high>=m_fs/2){
        m_frq_high = m_fs/2;
        reportEvent("Frequency high for band-pass filter is too high, change to fs/2");
    }
    m_first_time = false;
  }
  vector<float> input_x(m_access_data_number,0);

    if(!m_voltage_data.empty()){

// Get input 
            if(m_voltage_data.size() >= m_access_data_number){
                for(int i=0;i<m_access_data_number;i++)
                    input_x[i] = m_voltage_data[i];
// Analysis (detection algorithm) 
            Analysis(input_x);

//remove calculated data 
            m_voltage_data.erase(m_voltage_data.begin(),m_voltage_data.begin()+round(m_access_data_number*1));
           
            }         
    }

    AppCastingMOOSApp::PostReport();
    return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool WhistleDetector_vehicle::OnStartUp()
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
    else if(param == "snr_threshold"){
        stringstream ss;
        ss<<value;
        ss>>m_SNR_threshold;

        handled = true;
    }
    else if(param == "band_pass_frq_low"){
        stringstream ss;
        ss<<value;
        ss>>m_frq_low;

        handled = true;
    }
    else if(param == "band_pass_frq_high"){
    stringstream ss;
        ss<<value;
        ss>>m_frq_high;

        if(m_frq_high>=(double)m_fs/2){
            m_frq_high = (double)m_fs/2;
            reportEvent("Higher Frequency for band-pass filter is too high, set up to fs/2");
        }

        handled = true;
    }
    else if(param == "data_update_percent"){
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

void WhistleDetector_vehicle::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
   Register("ACOUSTIC_DATA", 0);
   Register("SOUND_VOLTAGE_DATA_CH_ONE", 0);
   Register("TEST_MESSAGE",0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool WhistleDetector_vehicle::buildReport() 
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
