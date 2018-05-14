/************************************************************/
/*    NAME: yhh                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_SearchFront_yhh.h                                      */
/*    DATE:                                                 */
/************************************************************/

#ifndef SearchFront_yhh_HEADER
#define SearchFront_yhh_HEADER

#include <sstream>
#include <string>
#include <array>
#include "IvPBehavior.h"
#include <deque>
#include <ZAIC_PEAK.h>
#include "XYPoint.h"
#include <vector>
class BHV_SearchFront_yhh : public IvPBehavior {
public:
  BHV_SearchFront_yhh(IvPDomain);
  ~BHV_SearchFront_yhh() {};
  
  bool         setParam(std::string, std::string);
  void         onSetParamComplete();
  void         onCompleteState();
  void         onIdleState();
  void         onHelmStart();
  void         postConfigStatus();
  void         onRunToIdleState();
  void         onIdleToRunState();
  
  void         GenRecPoint();
  void         GenSinPoint(vector<array<double,2>>);
  vector<array<double,2>> CheckDiffPoint();
  vector<array<double,2>> GenTwoPoint(vector<array<double,2>>);

  IvPFunction* onRunState();
  IvPFunction* buildFunctionWithZAIC();
  void         postViewPoint(bool viewable); 

protected: // Local Utility functions
    
protected: // Configuration parameters
  double      m_arrival_radius;
  double      m_desired_speed;
  XYPoint     m_nextpt;
  XYPoint     m_temp_diff_point;
  std::string m_ipf_type;
protected: // State variables
  std::string m_report;
  std::string m_request;
  std::string m_index_str;
  std::string m_direction;
  std::deque<array<double,4>> m_temp_dbl_buff;
  std::deque<array<double,4>> m_diff_point_buff;
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
  double      m_middle_x;
  double      m_middle_y;
  double      m_threshold;
  double      m_width;
  double      m_length;
  double      m_amp;
  double      m_omega;
// points
  double      m_pntx_one;
  double      m_pntx_two;
  double      m_pntx_three;
  double      m_pntx_four;
  
  double      m_pnty_one;
  double      m_pnty_two;
  double      m_pnty_three;
  double      m_pnty_four;
// global index variable
  int    m_index;
  int    m_point_index;
  int    m_checking_start_index;
  double m_input_index;
//global boolin  
  bool m_generate_point;
  bool m_generate_point_sin;
  bool m_first_point;
  bool m_over_angle;
};


#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_SearchFront_yhh(domain);}
}
#endif
