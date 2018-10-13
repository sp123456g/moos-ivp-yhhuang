/************************************************************/
/*    NAME: yhhuang                                         */
/*    ORGN: NTU, Taiwan                                             */
/*    FILE: WhistleDetector_vehicle.h                                  */
/*    DATE: Aug 17th 2018                                   */
/************************************************************/

#ifndef WhistleDetector_vehicle_HEADER
#define WhistleDetector_vehicle_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <deque>
class WhistleDetector_vehicle : public AppCastingMOOSApp
{
 public:
   WhistleDetector_vehicle();
   ~WhistleDetector_vehicle();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();
   bool GetVoltageData(std::string);
   bool Analysis(std::vector<float>);
   bool SendData_back(std::vector<float>);

 protected:
   void registerVariables();

 private: // Configuration variables

 private: // State variables

   std::deque<float>        m_voltage_data;
   std::deque<std::string>  m_input_data;

 private: // STFT variables

    int         m_fs;               // sample rate
    int         m_window_length;    // window length
    int         m_bits;
    int         m_access_data_number;
    double      m_iterate_data;   // "m_iterate_data" seconds per iterate loop  
    double      m_overlap;
    double      m_update_percent;
    bool        m_whistle_exist;
    bool        m_first_time;

    float       m_SNR_threshold;
    float       m_frq_low;
    float       m_frq_high;
    std::string m_window_type;   // Window type: "hanning" or "rectangular" window
    std::string m_testing_message;
};

#endif 
