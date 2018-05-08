/************************************************************/
/*    NAME: yhh                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_SearchFront.h                                      */
/*    DATE:                                                 */
/************************************************************/

#ifndef SearchFront_HEADER
#define SearchFront_HEADER

#include <sstream>
#include <string>
#include "IvPBehavior.h"
#include <deque>
#include <ZAIC_PEAK.h>
class BHV_SearchFront : public IvPBehavior {
public:
  BHV_SearchFront(IvPDomain);
  ~BHV_SearchFront() {};
  
  bool         setParam(std::string, std::string);
  void         onSetParamComplete();
  void         onCompleteState();
  void         onIdleState();
  void         onHelmStart();
  void         postConfigStatus();
  void         onRunToIdleState();
  void         onIdleToRunState();
  IvPFunction* onRunState();
  IvPFunction* Gen_Zaic_peak(double heading);

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
  int m_index;
};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_SearchFront(domain);}
}
#endif
