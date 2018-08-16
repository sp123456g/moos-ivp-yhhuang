/************************************************************/
/*    NAME: yhhuang                                         */
/*    ORGN: MIT                                             */
/*    FILE: WhistleDetector.h                                  */
/*    DATE: May 29th 2018                                   */
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

 protected:
   void registerVariables();

 private: // Configuration variables

 private: // State variables

   std::deque<double> m_input_data;

 private: // STFT variables

    int         m_fs;               // sample rate
    int         m_window_length;    // window length
    double      m_iterate_data;   // "m_iterate_data" seconds per iterate loop  
    double      m_overlap;
    std::string m_window_type;   // Window type: "hanning" or "rectangular" window
    std::string m_do_dectect;   // using detection algorithm to seperate data
};

#endif 
