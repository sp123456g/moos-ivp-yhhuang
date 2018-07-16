/************************************************************/
/*    NAME: yhhuang                                              */
/*    ORGN: NTU Taipei, Taiwan                                             */
/*    FILE: SpectrogramMonitor.h                                          */
/*    DATE: July 16th, 2018                             */
/************************************************************/

#ifndef SpectrogramMonitor_HEADER
#define SpectrogramMonitor_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <deque>
class SpectrogramMonitor : public AppCastingMOOSApp
{
 public:
   SpectrogramMonitor();
   ~SpectrogramMonitor();

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
   std::deque<double> GetPressure(int bits, double sen, std::deque<std::string> Input);   
 private: // State variables
    
    unsigned int    m_input_frame;
    int             m_bits;
    int             m_sample_rate;
    bool            m_get_data;
    double          m_sen;

    std::deque<std::string> m_input_per_frame_buffer;
};

#endif 
