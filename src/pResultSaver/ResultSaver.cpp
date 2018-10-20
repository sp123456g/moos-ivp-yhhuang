/************************************************************/
/*    NAME: yhhuang                                         */
/*    ORGN: National Taiwan University                      */
/*    FILE: ResultSaver.cpp                                 */
/*    DATE: Oct 20th 2018                                   */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "ResultSaver.h"
#include <sys/stat.h>
#include <stdlib.h>

using namespace std;

//---------------------------------------------------------
// Constructor

ResultSaver::ResultSaver()
{
    m_filepath = "./";
    m_filename = "";
    m_variable_name = "";
    m_sending = "false";
    m_sending_numbers = 10;
    m_lines = 0;
    m_sending_index = 0;
}

//---------------------------------------------------------
// Destructor

ResultSaver::~ResultSaver()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool ResultSaver::OnNewMail(MOOSMSG_LIST &NewMail)
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

     else if(key == toupper(m_variable_name)){
        m_input_msg.push_back(msg.GetString());
     }
     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool ResultSaver::OnConnectToServer()
{
   registerVariables();
   return(true);
}
//--------------------------------------------------------
//save_file function
void ResultSaver::save_file(string input_str){

    stringstream ss;
    string filepath_name;
    ss<<m_filepath<<m_filename;
    ss>>filepath_name;

    mkdir(m_filepath.c_str(),0777);
    m_fp = fopen(filepath_name.c_str(),"a");
     
    fprintf(m_fp,"%s",m_variable_name.c_str()); 
    fprintf(m_fp,"%s","=");
    fprintf(m_fp,"%s %s",input_str.c_str(),"\n"); 

    fclose(m_fp);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second
bool ResultSaver::Iterate()
{
  AppCastingMOOSApp::Iterate();

  string sending_msg;

// save file
  while(!m_input_msg.empty()){

    sending_msg = m_input_msg.front();
    save_file(sending_msg);
    m_lines++;
    m_input_msg.pop_front();

//send data from scp if needed  
    if(m_sending == "true" && m_sending_numbers == m_lines){
        
        Notify("SEND_DATA_BY_DATASENDER","true");
        
        stringstream ss_command;
        ss_command<<"mv "<<m_filepath<<m_filename<<"  ";
        ss_command<<m_filepath<<m_sending_index<<"_"<<m_filename<<"_pending";
        
        reportEvent("File: "+ m_filename + " is ready for sending");

        system((ss_command.str()).c_str());
        m_sending_index++;
        m_lines = 0;
    }

  }
  
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool ResultSaver::OnStartUp()
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
    else if(param == "sending_variable_name"){
        m_variable_name = value;
        handled = true;
    }
    else if(param == "filepath"){
        m_filepath = value;
        handled = true;
    }
    else if(param == "filename"){
        m_filename = value;
        handled = true;
    }
    else if(param == "send_by_pdatasender"){
        m_sending = value;
        handled = true;
    }
    else if(param == "wait_number_of_line"){
        m_sending_numbers = atoi(value.c_str());
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

void ResultSaver::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
     Register(toupper(m_variable_name),0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool ResultSaver::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "Variable pending:      "<<toupper(m_variable_name)<<" \n";
  m_msgs << "File name:             "<<m_filename<<"\n";
  m_msgs << "Path:                  "<<m_filepath<<"\n";
  m_msgs << "Sent by scp:           "<<m_sending<<"\n";
  m_msgs << "Sending goal number:   "<<m_sending_numbers<<"\n";
  m_msgs << "Number of line now:    "<<m_lines<<"\n";
  m_msgs << "============================================ \n";

  ACTable actab(4);
  m_msgs << actab.getFormattedString();

  return(true);
}




