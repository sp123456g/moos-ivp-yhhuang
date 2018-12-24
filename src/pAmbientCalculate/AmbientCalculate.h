/************************************************************/
/*    NAME: yhhuang                                              */
/*    ORGN: MIT                                             */
/*    FILE: AmbientCalculate.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef AmbientCalculate_HEADER
#define AmbientCalculate_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <deque>

class AmbientCalculate : public AppCastingMOOSApp
{
 public:
   AmbientCalculate();
   ~AmbientCalculate();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   bool GetVoltageData_ch1(std::string input);
   bool GetVoltageData_ch2(std::string input);

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables
   std::deque<float> m_ch1;
   std::deque<float> m_ch2;

   int      m_fs;
   int      m_access_data_num;

   float    m_gain;
   float    m_sen;
   float    m_duration;
   float    m_avg_spl1;
   float    m_avg_voltage1;
   float    m_avg_spl2;
   float    m_avg_voltage2;
   

 private: // State variables
};

#endif 
