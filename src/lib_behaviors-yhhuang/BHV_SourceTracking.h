/************************************************************/
/*    NAME: yhhuang                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_SourceTracking.h                                      */
/*    DATE:                                                 */
/************************************************************/

#ifndef SourceTracking_HEADER
#define SourceTracking_HEADER

#include <string>
#include "IvPBehavior.h"

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

protected: // Local Utility functions

protected: // Configuration parameters

protected: // State variables
};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_SourceTracking(domain);}
}
#endif
