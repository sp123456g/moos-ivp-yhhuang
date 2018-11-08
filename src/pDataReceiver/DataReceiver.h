/************************************************************/
/*    NAME: yhhuang                                         */
/*    ORGN: NTU                                             */
/*    FILE: DataReceiver.h                                  */
/*    DATE: Nov 8th, 2018                                   */
/************************************************************/

#ifndef DataReceiver_HEADER
#define DataReceiver_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <deque>

class DataReceiver : public AppCastingMOOSApp
{
 public:
   DataReceiver();
   ~DataReceiver();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

   std::string formFileName();
   std::string TestCommand(std::string);

   bool readfile();
   bool NotifyResult();

 private: // Configuration variables

    std::string m_file_path;
    std::string m_file_name_main;
    std::string m_file_index_need;
    std::string m_from_data_sender;
    std::string m_full_filename;
    
    unsigned int m_index;

    std::deque<std::string> m_variable_buff;
    std::deque<std::string> m_value_buff;

    bool m_start;

 private: // State variables
};

#endif 
