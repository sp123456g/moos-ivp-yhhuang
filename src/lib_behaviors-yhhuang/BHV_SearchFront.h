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
#include "XYPoint.h"
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
  void         GenCirclePoint();
  IvPFunction* onRunState();
  IvPFunction* buildFunctionWithZAIC();
  void         postViewPoint(bool viewable); 

protected: // Local Utility functions
    
protected: // Configuration parameters
  double      m_arrival_radius;
  double      m_desired_speed;
  XYPoint     m_nextpt;
  std::string m_ipf_type;
protected: // State variables
  std::string m_report;
  std::string m_request;
  std::string m_index_str;
  std::string m_direction;
  std::deque<double> m_temp_dbl_buff;
  std::deque<double> m_next_pntx;
  std::deque<double> m_next_pnty;
  double      m_time0;
  double      m_time1;
  double      m_avg_number;
  double      m_heading;
  double      m_curr_time;
  double      m_arrive_time;
  double      m_osx;
  double      m_osy;
  double      m_omega;
  double      m_middle_x;
  double      m_middle_y;
  double      m_circle_radius;
  
  int m_index;
  int m_point_index;

  bool m_generate_point;
};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_SearchFront(domain);}
}
#endif
