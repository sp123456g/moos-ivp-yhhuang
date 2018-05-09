/************************************************************/
/*    NAME: yhh                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_SearchFront_Tony.h                                      */
/*    DATE:                                                 */
/************************************************************/

#ifndef SearchFront_Tony_HEADER
#define SearchFront_Tony_HEADER

#include <sstream>
#include <string>
#include "IvPBehavior.h"
#include <deque>
#include <ZAIC_PEAK.h>
#include "XYPoint.h"

class BHV_SearchFront_Tony : public IvPBehavior {
public:
  BHV_SearchFront_Tony(IvPDomain);
  ~BHV_SearchFront_Tony() {};
  
  bool         setParam(std::string, std::string);
  void         onSetParamComplete();
  void         onCompleteState();
  void         onIdleState();
  void         onHelmStart();
  void         postConfigStatus();
  void         onRunToIdleState();
  void         onIdleToRunState();
  IvPFunction* onRunState();
  IvPFunction* buildFunctionWithReflector();
  void         giveNextPoint();

protected: // Local Utility functions
    
protected: // Configuration parameters
  

protected: // State variables
  std::string m_report;
  std::string m_request;
  std::string m_index_str;
  std::deque<double> m_temp_dbl_buff;
  double      m_time0;
  double      m_time1;
  double      m_avg_number;
  double      m_heading;
  double      m_curr_time;
  double      m_arrive_time;

  double      m_omega; // sin wave omega
  double      m_desire_speed; 
  double      m_osx;
  double      m_osy;
  double      m_nextx;
  double      m_nexty;
  double      m_arrival_radius; 
  double      m_temperature_record; 
  double      m_temperature_now;
  double      m_temperature_thresold;

  bool        m_temperature_record_update;
  bool        m_vehicle_go_down;
  bool        m_vehicle_go_more;


  int m_index;

};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_SearchFront_Tony(domain);}
}
#endif
