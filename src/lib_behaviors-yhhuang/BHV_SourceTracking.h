/************************************************************/
/*    NAME: yhhuang                                         */
/*    ORGN: NTU, Taipei                                     */
/*    FILE: BHV_SourceTracking.h                            */
/*    DATE: Jan 11, 2019                                    */
/************************************************************/

#ifndef SourceTracking_HEADER
#define SourceTracking_HEADER

#include <string>
#include "IvPBehavior.h"
#include <ZAIC_PEAK.h>
#include "XYPoint.h"
#include <vector>
#include <XYSegList.h>

class BHV_SourceTracking : public IvPBehavior {
public:
  BHV_SourceTracking(IvPDomain);
  ~BHV_SourceTracking() {};
  
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

protected: // State variables

  XYPoint   m_nextpt;

  double    m_target_angle;
  double    m_desired_speed;
  double    m_check_num;
  double    m_left_bd;
  double    m_right_bd;
  double    m_arrival_radius;

  double    m_osx;
  double    m_osy;
  double    m_osheading;
  

  bool      m_start_gen_next_pt;
  bool      m_gen_ipf;
  bool      m_arrive;

  int       m_no_dolphin_time;
  int       m_no_wh_time;       //no whistle over this variable time stop generate ipf

  std::vector<double> m_source_angle_buff;


  bool      m_whistle_exist;
};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_SourceTracking(domain);}
}
#endif
