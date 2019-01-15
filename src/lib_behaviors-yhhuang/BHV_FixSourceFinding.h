/************************************************************/
/*    NAME: yhhuang                                         */
/*    ORGN: NTU, Taipei                                     */
/*    FILE: BHV_FixSourceFinding.h                            */
/*    DATE: Jan 11, 2019                                    */
/************************************************************/

#ifndef FixSourceFinding_HEADER
#define FixSourceFinding_HEADER

#include <string>
#include "IvPBehavior.h"
#include <ZAIC_PEAK.h>
#include "XYPoint.h"
#include <vector>
#include <XYSegList.h>

class BHV_FixSourceFinding : public IvPBehavior {
public:
  BHV_FixSourceFinding(IvPDomain);
  ~BHV_FixSourceFinding() {};
  
  bool         setParam(std::string, std::string);
  void         onSetParamComplete();
  void         onCompleteState();
  void         onIdleState();
  void         onHelmStart();
  void         postConfigStatus();
  void         onRunToIdleState();
  void         onIdleToRunState();
  IvPFunction* onRunState();

  IvPFunction*  buildFunctionWithZAIC();
  void          postViewPoint(bool viewable);
  double        getMedian(std::vector<double>);
  bool          genNextPt(double);
  bool          ShowLine(double,double,double,std::string);
  bool          CheckSourceAngle();  
  

protected: // Local Utility functions

protected: // Configuration parameters

  std::string m_ipf_type;
  std::string m_method;
  std::string m_measure_start;
  std::string m_source_quadrant;

  bool m_angle1_get;
  bool m_angle2_get;
  bool m_angle3_get;

protected: // State variables

  int       m_check_num;

  double    m_arrival_radius;
  double    m_desired_speed;
  double    m_osx;
  double    m_osy;
  double    m_center_x;
  double    m_center_y;
  double    m_first_line_point_x;
  double    m_first_line_point_y;
  double    m_second_line_point_x;
  double    m_second_line_point_y;
  double    m_line_angle1;
  double    m_line_angle2;
  double    m_left_bd;
  double    m_right_bd;

  double    m_angle1;
  double    m_angle2;
  double    m_angle3;

  XYPoint   m_nextpt;

  bool      m_whistle_exist;
  bool      m_first_time_change_heading;
  bool      m_second_time_checking;
  bool      m_third_time_check;
  bool      m_start_gen_next_pt;
  bool      m_check_source_point;
  bool      m_gen_ipf;
  bool      m_arrive;
  bool      m_second_poke_hold;

  std::vector<double> m_source_angle_buff;
};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_FixSourceFinding(domain);}
}
#endif
