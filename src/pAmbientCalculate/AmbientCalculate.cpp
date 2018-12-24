/************************************************************/
/*    NAME: yhhuang                                              */
/*    ORGN: MIT                                             */
/*    FILE: AmbientCalculate.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "AmbientCalculate.h"
#include <stdlib.h>
#include <math.h>

using namespace std;

//---------------------------------------------------------
// Constructor

AmbientCalculate::AmbientCalculate()
{
    m_gain          = 0;
    m_sen           = 0;
    m_duration      = 2;
    m_avg_spl1      = 0;
    m_avg_voltage1  = 0;
    m_avg_spl2      = 0;
    m_avg_voltage2  = 0;
    m_fs            = 96000;
}

//---------------------------------------------------------
// Destructor

AmbientCalculate::~AmbientCalculate()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool AmbientCalculate::OnNewMail(MOOSMSG_LIST &NewMail)
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
        GetVoltageData_ch1(msg.GetString()); 
     }
     else if(key == "SOUND_VOLTAGE_DATA_CH_TWO"){
        GetVoltageData_ch2(msg.GetString()); 
     
     }
     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool AmbientCalculate::OnConnectToServer()
{
   registerVariables();
   return(true);
}

bool AmbientCalculate::GetVoltageData_ch1(string input)
{

    vector<string> voltages = parseString(input,',');

    for(int i=0; i<voltages.size(); i++){
      float volt = atof(voltages[i].c_str());

      m_ch1.push_back(volt);
    }
}

bool AmbientCalculate::GetVoltageData_ch2(string input)
{

    vector<string> voltages = parseString(input,',');

    for(int i=0; i<voltages.size(); i++){
      float volt = atof(voltages[i].c_str());

      m_ch2.push_back(volt);
    }
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool AmbientCalculate::Iterate()
{
  AppCastingMOOSApp::Iterate();

    float total1 = 0,total2 = 0;

    int access_data_num = round(m_fs * m_duration);
    deque<float> ch1(access_data_num,0), ch2(access_data_num,0);

// check ch1
    if(m_ch1.size() >= access_data_num){
      
        for(int i=0;i<access_data_num;i++)
            ch1[i] = m_ch1[i];

        for(int i=0;i<ch1.size();i++)
            total1 +=abs(ch1[i]);

        m_avg_voltage1 = total1/access_data_num;
        m_avg_spl1 = 20*log10(m_avg_voltage1) - m_sen + m_gain;

        Notify("AVG_SPL_CH_ONE", m_avg_spl1);
        Notify("AVG_VOLTAGE_CH_ONE", m_avg_voltage1);

        m_ch1.erase(m_ch1.begin(),m_ch1.begin()+round(access_data_num*1));
    }


// check ch2
    if(m_ch2.size() >= access_data_num){
    
       for(int i=0;i<access_data_num;i++)
           ch2[i] = m_ch2[i];
   
       for(int i=0;i<ch2.size();i++)
           total2 +=abs(ch2[i]);
 
       m_avg_voltage2 = total2/access_data_num;
       m_avg_spl2 = 20*log10(m_avg_voltage2) - m_sen + m_gain;

       Notify("AVG_SPL_CH_TWO", m_avg_spl2);
       Notify("AVG_VOLTAGE_CH_TWO", m_avg_voltage2);

       m_ch2.erase(m_ch2.begin(),m_ch2.begin()+round(access_data_num*1));
    }
    
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool AmbientCalculate::OnStartUp()
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
    else if(param == "duration"){
        m_duration = atof(value.c_str());
        handled = true;
    }
    else if(param == "sen"){
        m_sen = atoi(value.c_str());
        handled = true;
    }
    else if(param == "gain") {
        m_gain = atof(value.c_str());
        handled = true;
    }  
    else if( param == "fs"){
        m_fs = atoi(value.c_str());
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

void AmbientCalculate::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
   Register("SOUND_VOLTAGE_DATA_CH_ONE", 0);
   Register("SOUND_VOLTAGE_DATA_CH_TWO", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool AmbientCalculate::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "ch1 average SPL:"<<m_avg_spl1<<"                  \n";
  m_msgs << "ch1 average Voltage:"<<m_avg_voltage1<<"          \n";
  m_msgs << "ch2 average SPL:"<<m_avg_spl2<<"                  \n";
  m_msgs << "ch2 average Voltage:"<<m_avg_voltage2<<"          \n";
  m_msgs << "============================================ \n";

  ACTable actab(4);
  m_msgs << actab.getFormattedString();

  return(true);
}




