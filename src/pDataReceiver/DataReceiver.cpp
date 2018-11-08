/************************************************************/
/*    NAME: yhhuang                                         */
/*    ORGN: NTU                                             */
/*    FILE: DataReceiver.cpp                                */
/*    DATE: Nov 8th, 2018                                   */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "DataReceiver.h"
#include <sstream>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

//---------------------------------------------------------
// Constructor

DataReceiver::DataReceiver()
{
    m_file_path         = "./DataTest/";
    m_file_name_main    = "testing.txt";
    m_file_index_need   = "false";
    m_from_data_sender  = "false";

    m_start = false;
    m_index = 0;
}

//---------------------------------------------------------
// Destructor

DataReceiver::~DataReceiver()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool DataReceiver::OnNewMail(MOOSMSG_LIST &NewMail)
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

     else if(key == "START_READ"){
        if(msg.GetString() == "true")
            m_start = true;
        else 
            m_start = false;
     }
     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool DataReceiver::OnConnectToServer()
{
   registerVariables();
   return(true);
}
//---------------------------------------------------------
// Function: formfilename()
string DataReceiver::formFileName(){

    // if it's sent by data sender
    stringstream ss_file_name;
    string output_str;

    if(m_from_data_sender == "true")
       ss_file_name<<m_file_path<<m_index<<"_"<<m_file_name_main<<"_sending"; 

    //if not sent by data sender, but need order
    else if(m_file_index_need == "true")
       ss_file_name<<m_file_path<<m_index<<"_"<<m_file_name_main; 
    
    // if don't need anythings just file name
    else 
        ss_file_name<<m_file_path<<m_file_name_main;

    output_str = ss_file_name.str();
    
    return(output_str);    
}

//---------------------------------------------------------
// Function: Testcommand() 
//  form testing command
string DataReceiver::TestCommand(string input){
    stringstream ss_command;
    ss_command<<"test -e "<<input;
    return(ss_command.str());
}

bool DataReceiver::readfile(){

    fstream fp;
    char buffer[192000];
    fp.open(m_full_filename.c_str(),ios::in);
    if(!fp)
        reportConfigWarning("Warning ----> Can't open file!!!");
    else{
        while(fp.getline(buffer,sizeof(buffer))){
            string line_str;
            line_str = buffer;
            
            m_variable_buff.push_back(biteString(line_str,'='));
            m_value_buff.push_back(line_str);

        }
        reportEvent("Read "+ m_full_filename + " success!");
        fp.close();
    }
}

//---------------------------------------------------------
//Function: NotifyResult()
bool DataReceiver::NotifyResult(){

    stringstream ss, ss2;
    ss<<m_variable_buff.size()<<"  ";
    ss2<<m_value_buff.size();

    int buff_size = m_variable_buff.size();
    for(int i=0;i<buff_size;i++){
        string variable_name = m_variable_buff.front();
        string str_value = m_value_buff.front();

        Notify(variable_name,str_value);

        m_variable_buff.pop_front();
        m_value_buff.pop_front();
    }
        m_index++;
}
//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second
bool DataReceiver::Iterate()
{
  AppCastingMOOSApp::Iterate();
  if(m_start){
    //step 1: form filename:
        m_full_filename = formFileName();
    //step 2: check if file exist 
        string command = TestCommand(m_full_filename);
        int exist = !system(command.c_str());
    //step 3: read file to variable and string buffer if exist, or output waiting for file
        if(exist){
            readfile();
        //step 4: Notify to MOOSDB
            NotifyResult();
        }
        else
            reportEvent("Waiting for file: " + m_full_filename);
  } 

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool DataReceiver::OnStartUp()
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
    else if(param == "file_path"){
    
      m_file_path = value;
      handled = true;
    }
    else if(param == "file_name"){
    
      m_file_name_main = value;
      handled = true;
    }
    else if(param == "file_order"){
    
      m_file_index_need = value;
      handled = true;
    }
    else if(param == "from_pdatasender"){
    
      m_from_data_sender = value;
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

void DataReceiver::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
   Register("START_READ", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool DataReceiver::buildReport() 
{
  m_msgs << "============================================ \n";

  if(!m_start)
    m_msgs << "Status: Waiting for START_READ=true\n";
  else{ 
    m_msgs << "Status: Poking                             \n";
    m_msgs << "Target File now: "<<m_full_filename<<"     \n";
  }
  m_msgs << "============================================ \n";

  ACTable actab(4);
  m_msgs << actab.getFormattedString();

  return(true);
}




