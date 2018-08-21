/************************************************************/
/*    NAME: yhhuang                                         */
/*    ORGN: NTU, Taiwan                                             */
/*    FILE: WhistleDetector.h                                  */
/*    DATE: Aug 17th 2018                                   */
/************************************************************/

#ifndef WhistleDetector_HEADER
#define WhistleDetector_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <deque>
class WhistleDetector : public AppCastingMOOSApp
{
 public:
   WhistleDetector();
   ~WhistleDetector();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();
   bool GetVoltageData(std::string);

 protected:
   void registerVariables();

 private: // Configuration variables

 private: // State variables

   std::deque<float>       m_voltage_data;
   std::deque<std::string>  m_input_data;

 private: // STFT variables

    int         m_fs;               // sample rate
    int         m_window_length;    // window length
    int         m_bits;
    double      m_iterate_data;   // "m_iterate_data" seconds per iterate loop  
    double      m_overlap;
    double      m_update_percent;
    std::string m_window_type;   // Window type: "hanning" or "rectangular" window
    bool        m_whistle_exist;
};

#endif 
