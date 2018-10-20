/************************************************************/
/*    NAME: yhhuang                                         */
/*    ORGN: National Taiwan University                      */
/*    FILE: ResultSaver.h                                   */
/*    DATE: Oct 20th 2018                                   */
/************************************************************/

#ifndef ResultSaver_HEADER
#define ResultSaver_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "deque"

class ResultSaver : public AppCastingMOOSApp
{
 public:
   ResultSaver();
   ~ResultSaver();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   bool GetVoltageData(std::string);

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();
   void save_file(std::string);

 private: // Configuration variables

 private: // State variables
   std::deque<std::string> m_input_msg;
   std::string  m_filepath;
   std::string  m_filename;
   std::string  m_variable_name;
   std::string  m_sending;

   unsigned int m_sending_numbers;
   unsigned int m_lines;
   unsigned int m_sending_index;

   FILE *m_fp;
};

#endif 
