/************************************************************/
/*    NAME: yhhuang                                         */
/*    ORGN: National Taiwan University                      */
/*    FILE: DataSender.h                                    */
/*    DATE: Oct 20th 2018                                   */
/************************************************************/

#ifndef DataSender_HEADER
#define DataSender_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class DataSender : public AppCastingMOOSApp
{
 public:
   DataSender();
   ~DataSender();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();
   std::string scp_command(); 
   std::string rename_command(std::string,std::string);

 protected:
   void registerVariables();

 private: // Configuration variables

   std::string m_sender_filepath;
   std::string m_reciever_user;
   std::string m_reciever_machine;
   std::string m_reciever_filepath;
   std::string m_variable;
   std::string m_send_data;

 private: // State variables
};

#endif 
