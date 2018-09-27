/************************************************************/
/*    NAME: yhhuang                                         */
/*    ORGN: NTU Taipei, Taiwan                              */
/*    FILE: SpectrogramMonitor.cpp                          */
/*    DATE: July 16th, 2018                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "SpectrogramMonitor.h"
#include "glspectrogram.h"
using namespace std;

//---------------------------------------------------------
// Constructor

SpectrogramMonitor::SpectrogramMonitor()
{
    m_bits = 16;
    m_sample_rate = 44100;
    m_input_frame = 0;
    m_get_data = false;
    m_input_per_frame_buffer.clear(); 
    m_sen = 165;
    char *myargv[1];
    int myargc=1;
    myargv[0] = strdup("pSpectrogramMonitor");
    glutInit(&myargc,myargv);
}
//---------------------------------------------------------
// Destructor

SpectrogramMonitor::~SpectrogramMonitor()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool SpectrogramMonitor::OnNewMail(MOOSMSG_LIST &NewMail)
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
     else if( key == "Sound_Data" ){
        m_get_data = true;
        m_input_per_frame_buffer.push_back(msg.GetString());
        m_input_frame++;
     }     
     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool SpectrogramMonitor::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool SpectrogramMonitor::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  
  
  if(m_get_data){

    deque<double> Pressure_buff = GetPressure(m_bits, m_sen, m_input_per_frame_buffer);
    pthread_t thread_gui;   
//    pthread_create(&thread_gui,NULL,RealTimeSpectrogram,void_pnt);
    pthread_create(&thread_gui,NULL,(void*(*)(void*))RealTimeSpectrogram,&Pressure_buff);
    cout<<"Real-time_end"<<endl;
    pthread_join(thread_gui,NULL);
    m_get_data = false;
  }

  AppCastingMOOSApp::PostReport();
  return(true);
}
// String message to voltage
deque<double> SpectrogramMonitor::GetPressure(int bits, double sen, deque<string> Input){

   deque<double> pressure;

   if(!Input.empty()){
       std::string input_str = Input.front();
       vector<string> value = parseString(input_str,',');
       for(int i=0;i<value.size();i++){
          long double num = atoi(value[i].c_str());
          double volt; 
         switch(m_bits){
            case 16:
             if(num <= 0){
              volt = num/32768;
             }
             else{
              volt = num/32767;
              cout<<"case16: num="<<num<<endl;
              cout<<"case16: volt="<<volt<<endl;
             }
              break;
            case 24:
             if(num <= 0){
              volt = num/8388608;
             }
            else{
              volt = num/8388607;
            }
            break;
            case 32:
             if(num <= 0){
              volt = num/2147483648;
             }
            else{
              volt = num/2147483647;
            }
            break;
        }
   //      volt = volt/(pow(10,(sen/20)));
        
              cout<<"voltage"<<volt<<endl;
        pressure.push_back(volt); 
   }
}

  return(pressure); 

}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool SpectrogramMonitor::OnStartUp()
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
    else if(param == "bits"){
        m_bits = atoi(value.c_str());
        handled = true;
    }
    else if(param == "sample_rate"){
        m_sample_rate = atoi(value.c_str());
        handled = true;
    }
    else if(param == "sensitivity"){
        m_sen = atof(value.c_str());
    }
        handled = true;
    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void SpectrogramMonitor::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
     Register("Sound_Data",0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool SpectrogramMonitor::buildReport() 
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




