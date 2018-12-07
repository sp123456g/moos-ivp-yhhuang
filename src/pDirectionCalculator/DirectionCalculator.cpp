/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: DirectionCalculator.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <sstream>
#include "MBUtils.h"
#include "ACTable.h"
#include "DirectionCalculator.h"
#include <sstream>

using namespace std;

//---------------------------------------------------------
// Constructor

DirectionCalculator::DirectionCalculator()
{
    m_fs            = 96000;
    m_c             = 1500;
    m_mic_dis       = 1;
    m_mic_num       = 2;
    m_interval      = 0.5;

    m_use_cc        = true;
    m_first_time    = true;

    m_access_data_num   = 48000;
    m_mic_num           = 2;
    m_low_fq            = 3;
    m_up_fq             = 10;
    m_filt_order        = 5;
    m_index             = 0;

    m_output_angle      = 0;

    m_ch1.clear();
    m_ch2.clear();
    m_ch3.clear();
    
    m_band_filter = new Filter(BPF,60,m_fs/1000,m_low_fq,m_up_fq);
}

//---------------------------------------------------------
// Destructor

DirectionCalculator::~DirectionCalculator()
{
    delete m_band_filter;
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool DirectionCalculator::OnNewMail(MOOSMSG_LIST &NewMail)
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
     else if(key == "WHISTLE_VOLTAGE_DATA_CH_ONE"){
         GetVoltageData_ch1(msg.GetString());
     }
     else if(key == "WHISTLE_VOLTAGE_DATA_CH_TWO"){
         GetVoltageData_ch2(msg.GetString());
     }
     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool DirectionCalculator::OnConnectToServer()
{
   registerVariables();
   return(true);
}

bool DirectionCalculator::Band_Filter(vector<float> &input){

    vector<float> output;
    double out;
    float out_data;

    for(int i =0; i<m_filt_order;i++){
        for(int j=0;j<input.size();j++){
            out = m_band_filter->do_sample((double)input[j]);
            out_data = (float) out;
            output.push_back(out_data);
        }
        input = output;
        output.clear();
    }
}

bool DirectionCalculator::GetVoltageData_ch1(string input)
{

    vector<string> voltages = parseString(input,',');

    for(int i=0; i<voltages.size(); i++){
      float volt = atof(voltages[i].c_str());

      m_ch1.push_back(volt);
    }
}

bool DirectionCalculator::GetVoltageData_ch2(string input)
{

    vector<string> voltages = parseString(input,',');

    for(int i=0; i<voltages.size(); i++){
      float volt = atof(voltages[i].c_str());

      m_ch2.push_back(volt);
    }
}

float DirectionCalculator::CalTDOA_by_cc(std::vector<float> ch1, std::vector<float> ch2){
//ch1 is left and ch2 is right
//tolerance
    unsigned int tolerance = 3;
    unsigned int    correlation_factor = tolerance+round((m_mic_dis/m_c)*m_fs);
//add "correlation_factor" zeros at front and behind 
    unsigned int    output_size = 1+correlation_factor*2;
//initialize array
    vector<float>   output(output_size,0);
    vector<float>   addzero_ch1(ch1.size()+correlation_factor*2,0); 
//add zero at front and behind
        for(unsigned int i=0;i<ch1.size();i++)
            addzero_ch1[i+correlation_factor] = ch1[i]; 
        

//correlation for only 2*correlation_factor+1 times to save time 
    unsigned int i=0;
    while(1){     //i for ch1
        float  temp = 0;
        unsigned int i1=i;
        for(unsigned int j=0;j<ch2.size();j++){      //j for ch2
            temp = temp + (addzero_ch1[i1]*ch2[j]);             
            i1++;
        }
            output[i]=temp;
            i++;
            if(i1==addzero_ch1.size())
                break;
    }
//find the maximun index after correlation
    vector<float>::iterator maxPosition = max_element(output.begin(), output.end());
    cout << *maxPosition << " at the postion of " << maxPosition - output.begin() <<endl;
    int max_index = maxPosition - output.begin();

    int     digi_TDOA = max_index - correlation_factor;
    float   true_TDOA;
// check digital TDOA digital TDOA is samples different and trueTDOA is in seconds

    if(digi_TDOA ==correlation_factor-tolerance)
     true_TDOA = m_mic_dis/m_c;
    
    else if(-digi_TDOA ==correlation_factor-tolerance)
     true_TDOA = -m_mic_dis/m_c;
    
    else if(digi_TDOA == 0)
        true_TDOA = 0;
    else 
        true_TDOA = digi_TDOA*(1/m_fs);

    return(true_TDOA);
}
float DirectionCalculator::CalTDOA_by_peak(std::vector<float> ch1, std::vector<float> ch2){
}

bool DirectionCalculator::Save_data(string filename, vector<float> in, string filepath){
    
    stringstream ss;
    string filepath_name;
    ss<<filepath<<filename;
    ss>>filepath_name;

    FILE *fp = fopen(filepath_name.c_str(),"w");

    float value;
    for(unsigned int i=0;i<in.size();i++)
            fprintf(fp,"%f %s",in[i],"\n");

    fclose(fp);
}
//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second
bool DirectionCalculator::Iterate()
{
    AppCastingMOOSApp::Iterate();

// Step 1. check data is over interval or not 
    if(m_first_time){
        m_access_data_num = round(m_fs*m_interval);
        m_first_time = false;
    } 
    float TDOA;
    vector<float> ch1_data(m_access_data_num,0);
    vector<float> ch2_data(m_access_data_num,0);
    
    stringstream ss;
    switch(m_mic_num){
        case 2:
            ss<<m_ch1.size()<<","<<m_ch2.size();
            reportEvent("size of ch1 and ch2:" +ss.str());
            if(m_ch1.size() >=m_access_data_num && m_ch2.size() >= m_access_data_num){
// Step 2. Calculate TDOA
// cross-correlation
//copy_data: 
                for(int i=0;i<m_access_data_num;i++)
                    ch1_data[i] = m_ch1[i];
                for(int j=0;j<m_access_data_num;j++)
                    ch2_data[j] = m_ch2[j];
// save data
                stringstream ss_file1, ss_file2, ss_file3,ss_file4;
                ss_file1<<m_index<<"before_filter_ch1.csv"; 
                Save_data(ss_file1.str(),ch1_data,"./csv_file/");
                
                ss_file2<<m_index<<"before_filter_ch2.csv"; 
                Save_data(ss_file2.str(),ch2_data,"./csv_file/");

//filtering
               Band_Filter(ch1_data);
               Band_Filter(ch2_data);

// save data
                ss_file3<<m_index<<"after_filter_ch1.csv"; 
                Save_data(ss_file3.str(),ch1_data,"./csv_file/");

                ss_file4<<m_index<<"after_filter_ch2.csv"; 
                Save_data(ss_file4.str(),ch2_data,"./csv_file/");


                m_index ++;

                if(m_use_cc)
                    TDOA = CalTDOA_by_cc(ch1_data,ch2_data);
                
                // find peak
                else
                    TDOA = CalTDOA_by_peak(ch1_data,ch2_data);

                m_ch1.erase(m_ch1.begin(),m_ch1.begin()+round(m_access_data_num*1));
                m_ch2.erase(m_ch2.begin(),m_ch2.begin()+round(m_access_data_num*1));

// Step 3. check angle
                stringstream ss1;
                ss1<<TDOA;
                reportEvent("TDOA="+ss1.str());

                float before_asin = TDOA*m_c/m_mic_dis;
                if(before_asin > 1){
                    cout<< "Invalid calculation"<<endl;
                    Notify("SOURCE_ANGLE","NAN");
                }
                else{
                    m_output_angle = asin(before_asin)*(180/PI);
                    Notify("SOURCE_ANGLE",m_output_angle);
                }
            }  
            break;
        case 3:
            if(m_ch1.size() >=m_access_data_num && m_ch2.size() >= m_access_data_num && m_ch3.size()>= m_access_data_num){
            }
            break;
    }
    
    AppCastingMOOSApp::PostReport();
    return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool DirectionCalculator::OnStartUp()
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
    else if(param =="sample_rate"){
        m_fs = atof(value.c_str()); 
        handled = true;
    }
    else if(param == "sound_speed"){
        m_c = atof(value.c_str());
        handled = true;
    }
    else if(param == "hyd_dis"){
        m_mic_dis = atof(value.c_str());
        handled = true;
    }
    else if(param == "hyd_number"){
        m_mic_num = atoi(value.c_str());
        handled = true;
    }
    else if(param == "interval"){
        m_interval = atof(value.c_str());
        handled = true;
    }
    else if(param =="use_cc"){
        if(value == "true")
            m_use_cc = true;
        else 
            m_use_cc = false;

        handled = true;
    }
    else if(param == "band_pass"){
        
        string low_fq_str   = biteStringX(value,':');
        string up_fq_str    = value;
        m_up_fq  = atoi(up_fq_str.c_str());
        m_low_fq = atoi(low_fq_str.c_str());

        m_band_filter = new Filter(BPF,60,m_fs/1000,m_low_fq,m_up_fq);
        
        if(m_band_filter->get_error_flag() < 0){
            reportConfigWarning("Filter can't be set up");
            handled = false;
        }
        else
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

void DirectionCalculator::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
   Register("WHISTLE_VOLTAGE_DATA_CH_ONE", 0);
   Register("WHISTLE_VOLTAGE_DATA_CH_TWO", 0);
   Register("WHISTLE_VOLTAGE_DATA_CH_THREE", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool DirectionCalculator::buildReport() 
{
  m_msgs << "============================================ \n";

  ACTable actab(4);
  m_msgs << "Band Pass filter:\n";
  m_msgs << "   Lower Frequency:"<<intToString(m_low_fq)<<"k Hz\n";
  m_msgs << "   Upper Frequency:"<<intToString(m_up_fq)<<"k Hz\n";
  m_msgs << "============================================ \n";
  m_msgs << "Direction Angle: "<<m_output_angle<<"\n";
  m_msgs << actab.getFormattedString();

  return(true);
}




