/************************************************************/
/*    NAME: yhhuang                                         */
/*    ORGN: MIT                                             */
/*    FILE: STFTAnalyser.h                                  */
/*    DATE: May 29th 2018                                   */
/************************************************************/

#ifndef STFTAnalyser_HEADER
#define STFTAnalyser_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <deque>
class STFTAnalyser : public AppCastingMOOSApp
{
 public:
   STFTAnalyser();
   ~STFTAnalyser();

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

    double  m_dt;                 // output time resolution
    double  m_df;                 // output frequency resolution
    int     m_sgm;                // scale gabor factor
    double  m_Bandwidth;                  // rectangular window length (seconds)
    int     m_fs;                 // sample rate
    double  m_threshold;          // threshold for detection
    std::string  m_do_dectect;         // using detection algorithm to seperate data
    double  m_iterate_data;       // "m_iterate_data" seconds per iterate loop  
    std::string  m_window_type;   // Window type: Gassian and rec
};

#endif 
