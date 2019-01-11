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
  IvPFunction* buildFunctionWithZAIC();

protected: // Local Utility functions

protected: // Configuration parameters

protected: // State variables

  double    m_desired_speed;
  double    m_osx;
  double    m_osy;
  XYPoint   m_nextpt;

};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_SourceTracking(domain);}
}
#endif
