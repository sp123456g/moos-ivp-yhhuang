/************************************************************/
/*    NAME: yhhuang                                         */
/*    ORGN: National Taiwan University                      */
/*    FILE: DataSender.cpp                                  */
/*    DATE: Oct 20th 2018                                   */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "DataSender.h"
#include <sstream>
#include <stdlib.h>

using namespace std;

//---------------------------------------------------------
// Constructor

DataSender::DataSender()
{
     m_sender_filepath = "";
     m_reciever_user = "";
     m_reciever_machine = "";
     m_reciever_filepath = "";
     m_variable = "";
}

//---------------------------------------------------------
// Destructor

DataSender::~DataSender()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool DataSender::OnNewMail(MOOSMSG_LIST &NewMail)
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

     else if(key == "SEND_DATA_BY_DATASENDER"){
        m_send_data = msg.GetString();
     }
     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool DataSender::OnConnectToServer()
{
   registerVariables();
   return(true);
}

string DataSender::scp_command(){

    string output;
    stringstream ss_out;

    ss_out<<"scp -r "<<m_sender_filepath<<"*_sending ";
    ss_out<<m_reciever_user<<"@"<<m_reciever_machine;
    ss_out<<":"<<m_reciever_filepath;

    output = ss_out.str();
    
    return(output);
    
};

string DataSender::rename_command(string before,string after){

    string output;
    stringstream ss_out;
// cd to the folder and then use rename to change pending to sending 
    ss_out<<"cd "<<m_sender_filepath;
    ss_out<<";rename -v s/"<<before<<"/"<<after<<"/ *;cd -";
    
    output = ss_out.str();

    return(output);
}
//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool DataSender::Iterate()
{
    AppCastingMOOSApp::Iterate();

    if(m_send_data == "true"){
    
        cout<<"\nChecking data now......."<<endl;

        string rename_command_str = rename_command("pending","sending");
        string scp_command_str = scp_command();

        int j = system(rename_command_str.c_str());
        if(j==0){
            cout<<"\nSending data now......."<<endl;
            int i = system(scp_command().c_str());

            stringstream ss;
            ss<<i;
    
            if(i==0){
                reportEvent("Send Data Success");
                rename_command_str = rename_command("sending","done");
                system(rename_command_str.c_str());
            }
            else{ 
                reportConfigWarning("Send Data fail, check if all things are setted up, and check if all the folders and files exist.");
                reportConfigWarning("Check ssh key by typing pDataSender -h for help");
            }
        }
        else{
            reportConfigWarning("check if file need to be send is end up with _pending");
            reportConfigWarning("check if rename and scp command installed");
        }
          
      
    m_send_data = "false";
  }
  
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool DataSender::OnStartUp()
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
    else if(param == "sender_filepath"){
        m_sender_filepath = value;
        handled = true;
    }
    else if(param == "reciever_user"){
        m_reciever_user = value;
        handled = true;
    }
    else if(param == "reciever_machine"){
        m_reciever_machine = value;
        handled = true;
    }
    else if(param == "reciever_filepath"){
        m_reciever_filepath = value;
        handled = true;
    }
    else if(param == "variable"){
        m_variable = value; 
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

void DataSender::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
   Register("SEND_DATA_BY_DATASENDER", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool DataSender::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "File:                    \n";
  m_msgs << "============================================ \n";
  m_msgs << "            \n";
  m_msgs << "Source folder:         "<<m_sender_filepath<<"      \n";
  m_msgs << "Destination folder:    "<<m_reciever_filepath<<"      \n";
  m_msgs << "Reciever:              "<<m_reciever_user<<"@"<<m_reciever_machine<<"      \n";
  m_msgs << "Destination file name: "<<"file in "<<m_reciever_filepath<<" end up with \"_sending\""<<"      \n";
  ACTable actab(4);
  m_msgs << actab.getFormattedString();

  return(true);
}




