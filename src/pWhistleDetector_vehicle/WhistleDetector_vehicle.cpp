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
#include <sstream>
using namespace std;

//---------------------------------------------------------
// Constructor

WhistleDetector_vehicle::WhistleDetector_vehicle()
{
    m_input_data.clear();
    m_ch2_str_buff.clear();
    m_ch3_str_buff.clear();
    m_ch4_str_buff.clear();

    m_overlap       = 0.9;
    m_window_length = 2048;
    m_fs            = 96000;
    m_window_length = (double)m_fs/46.875;
    m_bits          = 32;
    m_iterate_data  = 1;   // 1 second
    m_update_percent= 0.5;
    m_window_type   = "hanning";
    m_SNR_threshold = 10;
    m_frq_low       = 3000;
    m_frq_high      = 10000;

    m_first_time    = true;
    m_get_frames    = false;
    m_whistle_exist = false;
    m_ch1_full      = false;
    m_ch2_full      = false;
    m_ch3_full      = false;
    m_ch4_full      = false;
    m_all_get       = false;
    m_active        = false;
    
    m_channel_need  = 1;
}


//---------------------------------------------------------
// Destructor

WhistleDetector_vehicle::~WhistleDetector_vehicle()
{
}

//--------------------------------------------------------
//Get current time 
std::string WhistleDetector_vehicle::fileTime(char kind)/*{{{*/
{
  time_t timep;
  struct tm *p;
  int year, mon, day, hour, min, sec;
  stringstream file_time;
  stringstream dir_date;
  time(&timep);
  p = localtime(&timep);
  year = 1900+p->tm_year;
  mon = 1+p->tm_mon;
  day = p->tm_mday;
  hour = p->tm_hour;
  min = p->tm_min;
  sec = p->tm_sec;
  if(kind == 'f'){
    file_time << year << "_" << mon << "_" << day << "_" << hour << "_" << min << "_" << sec;
    return(file_time.str());
  }else if(kind == 'd'){
    dir_date << year << "_" << mon << "_" << day;
    return(dir_date.str());
  }
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
        double tic = MOOSTime();
        m_input_data.push_back(msg.GetString());
        GetVoltageData(msg.GetString()); 
        double toc = MOOSTime();

        stringstream ss;
        ss<<(toc-tic);
        Notify("GET_VOLTAGE_TIME",ss.str());
     }
     else if(key == "SOUND_VOLTAGE_DATA_CH_TWO"){
         if(m_channel_need !=1)
            m_ch2_str_buff.push_back(msg.GetString()); 
            GetVoltageData_ch2(msg.GetString());
     }
     else if(key == "SOUND_VOLTAGE_DATA_CH_THREE"){
         if(m_channel_need ==3 || m_channel_need ==4)
            m_ch3_str_buff.push_back(msg.GetString()); 
     }
     else if(key == "SOUND_VOLTAGE_DATA_CH_FOUR" ){
         if(m_channel_need ==4)
            m_ch4_str_buff.push_back(msg.GetString()); 

     }
     else if(key == "TEST_MESSAGE"){
        m_testing_message = msg.GetString();
     }
     else if(key == "DB_UPTIME"){
            m_current_time = msg.GetDouble();
     }
     else if(key == "RECORD_FRAMES"){
         stringstream ss;
         if(msg.IsString()){
            ss<<msg.GetString();
            m_frames = atoi((msg.GetString()).c_str());
         }
         else 
            m_frames = (double)msg.GetDouble();

         m_get_frames = true;
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
      
      if(volt >0.9)
          volt = 0;

      m_voltage_data.push_back(volt);
    }

   return(true);
}

bool WhistleDetector_vehicle::GetVoltageData_ch2(std::string input)
{

    vector<string> voltages = parseString(input,',');

    for(int i=0; i<voltages.size(); i++){
      float volt = atof(voltages[i].c_str());
      
      if(volt >0.9)
          volt = 0;
      m_voltage_data_ch2.push_back(volt);
    }

   return(true);
}

bool WhistleDetector_vehicle::SendData(vector<float> input){
    
    stringstream msg1_1, msg1_2, msg2_1, msg2_2, msg3_1, msg3_2, msg4_1, msg4_2;
//devide data
//    for(int i=0,j=ceil(input.size()/2);i<floor(input.size()/2),j<input.size();i++,j++){
//        msg1_1 << input[i]<<",";
//        msg1_2 << input[j]<<",";
//    }
//    Notify("WHISTLE_VOLTAGE_DATA_CH_ONE",msg1_1.str());
//    Notify("WHISTLE_VOLTAGE_DATA_CH_ONE",msg1_2.str());

// ch1 data back
    if(!m_input_data.empty()){
    
        for(int i=0,k=ceil(m_other_ch_str_num/2);i<floor(m_other_ch_str_num/2),k<m_other_ch_str_num;i++,k++){
           msg1_1 << m_input_data[i];
           msg1_2 << m_input_data[k];
        }
        Notify("WHISTLE_VOLTAGE_DATA_CH_ONE",msg1_1.str());
        Notify("WHISTLE_VOLTAGE_DATA_CH_ONE",msg1_2.str());
    }
// ch2 data back
    if(!m_ch2_str_buff.empty()){
       for(int i=0,k=ceil(m_other_ch_str_num/2);i<floor(m_other_ch_str_num/2),k<m_other_ch_str_num;i++,k++){
           msg2_1 << m_ch2_str_buff[i];
           msg2_2 << m_ch2_str_buff[k];
       }
    Notify("WHISTLE_VOLTAGE_DATA_CH_TWO",msg2_1.str());
    Notify("WHISTLE_VOLTAGE_DATA_CH_TWO",msg2_2.str());
    }
// ch3 data back
    if(!m_ch3_str_buff.empty()){
       for(int i=0,k=ceil(m_other_ch_str_num/2);i<floor(m_other_ch_str_num/2),k<m_other_ch_str_num;i++,k++){
           msg3_1 << m_ch3_str_buff[i];
           msg3_2 << m_ch3_str_buff[k];
       }
    Notify("WHISTLE_VOLTAGE_DATA_CH_THREE",msg3_1.str());
    Notify("WHISTLE_VOLTAGE_DATA_CH_THREE",msg3_2.str());
    }
    if(!m_ch4_str_buff.empty()){
       for(int i=0,k=ceil(m_other_ch_str_num/2);i<floor(m_other_ch_str_num/2),k<m_other_ch_str_num;i++,k++){
           msg4_1 << m_ch4_str_buff[i];
           msg4_2 << m_ch4_str_buff[k];
       }
    Notify("WHISTLE_VOLTAGE_DATA_CH_FOUR",msg4_1.str());
    Notify("WHISTLE_VOLTAGE_DATA_CH_FOUR",msg4_2.str());
    }
  



    return(true);
}

bool WhistleDetector_vehicle::Analysis(vector<float> input_data)
{

    stringstream ss1, ss2, ss3;

    vector<vector<float> > P;

//set up window
    int win_number;
    if(m_window_type == "rectagular")
        win_number = 0;
    else 
        win_number = 1; 
//STFT
    double tic_1 = MOOSTime();
    P = spectrogram_yhh(input_data,m_fs,m_window_length,m_overlap,win_number);
    double toc_1 = MOOSTime();
    
    
//Whistle detection
    
    double tic_2 = MOOSTime();
    detect_whistle(P,m_fs,m_window_length,m_overlap,m_SNR_threshold,m_frq_low,m_frq_high);
    double toc_2 = MOOSTime();
//check if there is whistle in the matrix
    vector<whistle> whistle_list;
    double tic_3 = MOOSTime();
    whistle_list = check_result(P,m_fs,m_window_length,m_overlap);
    double toc_3 = MOOSTime();

    m_whistle_exist = !whistle_list.empty();

// Check properties of whistles
// ----------------------------------------------------------------
    if(m_whistle_exist){

        Notify("WHISTLE_EXIST","true");
        Notify("APEAR_TIME",fileTime('f'));
        

//send result data to MOOSDB
        SendData(input_data);        

// output result in event        
        for(int i=0;i<whistle_list.size();i++){
            stringstream ss_1, ss_2,ss_3,ss_4;
            ss_1<<i;
            ss_2<<whistle_list[i].start_frq;
            ss_3<<whistle_list[i].end_frq;
//            reportEvent("whistle_"+ ss_1.str());
            reportEvent("start frq = " + ss_2.str());
            reportEvent("end frq = " + ss_3.str());
        }
    }
//-----------------------------------------------------------------
    else 
        Notify("WHISTLE_EXIST","false");
   

//Notify time use
    ss1<<(toc_1-tic_1);
    ss2<<(toc_2-tic_2);
    ss3<<(toc_3-tic_3);
    Notify("STFT_TIME",ss1.str());
    Notify("DETECTION_TIME",ss2.str());
    Notify("FRQ_ANALYSIS_TIME",ss3.str());

    return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool WhistleDetector_vehicle::Iterate()
{
  AppCastingMOOSApp::Iterate();
    
//Access data -> "m_iterate_data" seconds data per Iterate loop
  if(m_first_time && m_get_frames){
    m_access_data_number = round(m_fs*m_iterate_data);
    m_other_ch_str_num   = round(m_fs*m_iterate_data/m_frames);
    stringstream ss;
    ss<<m_other_ch_str_num;
    reportEvent("other channel access data counting: "+ss.str());

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

            switch(m_channel_need){
            case 1:
                m_all_get = true;
                break;
            case 2:
                if(m_ch2_str_buff.size() >= m_other_ch_str_num)
                    m_all_get = true;

                break;
            case 3:
                if(m_ch2_str_buff.size() >= m_other_ch_str_num && m_ch3_str_buff.size() >= m_other_ch_str_num)
                    m_all_get = true;
                break;
            case 4:
                if(m_ch2_str_buff.size() >= m_other_ch_str_num && m_ch3_str_buff.size() >= m_other_ch_str_num && m_ch4_str_buff.size() >= m_other_ch_str_num)
                    m_all_get = true;
                break;
            }                
        }  
        
            if(m_all_get){
                m_active = true;
              //  reportEvent("Analysing!");
                for(int i=0;i<m_access_data_number;i++)
                    input_x[i] = m_voltage_data[i];
                // Analysis (detection algorithm) 
                Analysis(input_x);
                //remove calculated data 
                m_voltage_data.erase(m_voltage_data.begin(),m_voltage_data.begin()+round(m_access_data_number*1));
                m_input_data.erase(m_input_data.begin(),m_input_data.begin()+round(m_other_ch_str_num*1));

                if(m_channel_need == 2){
                    m_ch2_str_buff.erase(m_ch2_str_buff.begin(),m_ch2_str_buff.begin()+round(m_other_ch_str_num*1));
                }
                else if(m_channel_need == 3){
                    m_ch2_str_buff.erase(m_ch2_str_buff.begin(),m_ch2_str_buff.begin()+round(m_other_ch_str_num*1));
                    m_ch3_str_buff.erase(m_ch3_str_buff.begin(),m_ch3_str_buff.begin()+round(m_other_ch_str_num*1));
                }
                else if(m_channel_need == 4){
                    m_ch2_str_buff.erase(m_ch2_str_buff.begin(),m_ch2_str_buff.begin()+round(m_other_ch_str_num*1));
                    m_ch3_str_buff.erase(m_ch3_str_buff.begin(),m_ch3_str_buff.begin()+round(m_other_ch_str_num*1));
                    m_ch4_str_buff.erase(m_ch4_str_buff.begin(),m_ch4_str_buff.begin()+round(m_other_ch_str_num*1));
                }

                m_all_get = false;
            }         
            else 
                m_active = false;
    }
    else 
        m_active = false;

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
    else if(param == "channel_need"){
        m_channel_need = atoi(value.c_str());
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
   Register("SOUND_VOLTAGE_DATA_CH_ONE", 0);
   Register("SOUND_VOLTAGE_DATA_CH_TWO", 0);
   Register("SOUND_VOLTAGE_DATA_CH_THREE", 0);
   Register("SOUND_VOLTAGE_DATA_CH_FOUR", 0);
   Register("RECORD_FRAMES",0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool WhistleDetector_vehicle::buildReport() 
{
  m_msgs << "============================================   \n";
  if(m_active)
      m_msgs << "pWhistleDetecor status:    Active          \n";
  else 
      m_msgs << "pWhistleDetecor status:  Waiting for data \n";
  m_msgs << "       (Remember to poke RECORD_FRAMES in str format if use pDataReceiver to get data)\n";
  m_msgs << "=============================================  \n";

  stringstream ss;
  string    iterate_data_second;
  ss<<m_iterate_data;
  ss>>iterate_data_second;

  ACTable actab(4);

  if(m_whistle_exist){
    m_msgs << "Whistle exist!!";
    reportEvent("Whistle exist");
  }
  else
    m_msgs << "No whistle in this "<<iterate_data_second<<" seconds\n";
//    m_msgs << "Ch1 size:"<<m_voltage_data.size()<<"\n";
//    m_msgs << "Ch2 size:"<<m_voltage_data_ch2.size()<<"\n";
//    m_msgs << "Ch1 str size:"<<m_input_data.size()<<"\n";
//    m_msgs << "Ch2 str size:"<<m_ch2_str_buff.size()<<"\n";

    m_msgs << actab.getFormattedString();

  return(true);
}
