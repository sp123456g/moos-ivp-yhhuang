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
#include <array>
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
  std::deque< array<double, 3> > m_temp_dbl_buff;
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

  //V1 Wave doesn't move and follow the wave
  //------------------------------------
  /*
  double      m_temperature_record; 
  double      m_temperature_now;
  double      m_temperature_thresold;

  bool        m_temperature_record_update;
  bool        m_vehicle_go_down;
  bool        m_vehicle_go_more;
  */
  //------------------------------------

  //V2 Wave moving, ship move around the region
  //------------------------------------
  /*

  XYPoint     m_first_round_points[7];
  int         m_first_round_index;
  XYPoint     m_wave_start_point;
  XYPoint     m_wave_end_point;  
  */
  //-------------------------------------

  //V3 Wave moving, ship vertial moving
  //------------------------------------
  /*
  XYPoint     m_first_round_points[7];
  int         m_first_round_index;
  bool        m_temperature_is_change[5];
  double      m_local_tmp_max;
  double      m_local_tmp_min;
  int         m_round2_line;
  bool        m_turn_direction;
  bool        m_local_max_min_need_initial;
  double      m_fix_pos;
  double      m_move_max;
  double      m_move_min;
  */
  //------------------------------------


  //V4, two veh squre with clock and couter-clock
  //------------------------------------
  XYPoint     m_first_round_points[4];
  int         m_first_round_index;
  bool        m_clockwise;
  //------------------------------------



  int m_index;

};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_SearchFront_Tony(domain);}
}
#endif
