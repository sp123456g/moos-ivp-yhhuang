/************************************************************/
/*    NAME: yhhuang                                         */
/*    ORGN: NTU, Taipei                                            */
/*    FILE: BHV_FixSourceFinding.cpp                          */
/*    DATE: Jan 11th, 2019                                   */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include "MBUtils.h"
#include "BuildUtils.h"
#include "AngleUtils.h"
#include "MBUtils.h"
#include "OF_Coupler.h"
#include "OF_Reflector.h"
#include <math.h>
#include <cmath>
#include <sstream>
#include "BHV_FixSourceFinding.h"


using namespace std;

//---------------------------------------------------------------
// Constructor

BHV_FixSourceFinding::BHV_FixSourceFinding(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "defaultname");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "course,speed");

  // Add any variables this behavior needs to subscribe for
  addInfoVars("NAV_X, NAV_Y, SOURCE_ANGLE, WHISTLE_EXIST","no_warning");

  m_ipf_type    =   "zaic";
  
  m_desired_speed = 2;
    m_osx         = 0;
    m_osy         = 0;
    m_center_x    = 0;
    m_center_y    = 0;

  m_whistle_exist = false;

}

// Procedure: postViewPoint
//---------------------------------------------------
void BHV_FixSourceFinding::postViewPoint(bool viewable)
{
  m_nextpt.set_label(" tracking navigation next waypoint");

  string point_spec;
  if(viewable)
    point_spec = m_nextpt.get_spec("active=true");
  else
    point_spec = m_nextpt.get_spec("active=false");
  postMessage("VIEW_POINT", point_spec);
}


//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_FixSourceFinding::setParam(string param, string val)
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  // Get the numerical value of the param argument for convenience once
  double double_val = atof(val.c_str());
  
  if((param == "foo") && isNumber(val)) {
    // Set local member variables here
    return(true);
  }
  else if (param == "bar") {
    // return(setBooleanOnString(m_my_bool, val));
  }
  else if (param == "loiter_center"){
    
        m_center_x = atof(biteStringX(val,',').c_str());
        m_center_y = atof(val.c_str());

        return(true);
  }

  else if ((param == "speed") && (double_val > 0) && (isNumber(val))){
        m_desired_speed = double_val;
        return(true);
  
  }
  // If not handled above, then just return false;
  return(false);
}

//---------------------------------------------------------------
// Procedure: onSetParamComplete()
//   Purpose: Invoked once after all parameters have been handled.
//            Good place to ensure all required params have are set.
//            Or any inter-param relationships like a<b.

void BHV_FixSourceFinding::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_FixSourceFinding::onHelmStart()
{
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_FixSourceFinding::onIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_FixSourceFinding::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_FixSourceFinding::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_FixSourceFinding::onIdleToRunState()
{
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_FixSourceFinding::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_FixSourceFinding::onRunState()
{
  // Part 1: Build the IvP function
  IvPFunction *ipf = 0;


  bool ok1,ok2,ok3;
  m_osx = getBufferDoubleVal("NAV_X",ok1);
  m_osy = getBufferDoubleVal("NAV_Y",ok2);

  if(!ok1 || !ok2)
    postWMessage("No ownship X/Y info in info_buffer");
  
    string str_whistle_exist = getBufferStringVal("WHISTLE_EXIST",ok3);
    if(str_whistle_exist == "true")
        m_whistle_exist = true;
    else 
        m_whistle_exist = false;
      
    if(m_whistle_exist){
        m_nextpt.set_vx(m_center_x);
        m_nextpt.set_vy(m_center_y);
        ipf = buildFunctionWithZAIC();
        if(ipf == 0)
            postWMessage("Problem Creating the IvP Function");
        postViewPoint(true);
    }
  // Part N: Prior to returning the IvP function, apply the priority wt
  // Actual weight applied may be some value different than the configured
  // m_priority_wt, depending on the behavior author's insite.
  if(ipf)
    ipf->setPWT(m_priority_wt);

  return(ipf);
}

IvPFunction *BHV_FixSourceFinding::buildFunctionWithZAIC()
{
  ZAIC_PEAK spd_zaic(m_domain, "speed");
  spd_zaic.setSummit(m_desired_speed);
  spd_zaic.setPeakWidth(0.5);
  spd_zaic.setBaseWidth(1.0);
  spd_zaic.setSummitDelta(0.8);
  if(spd_zaic.stateOK() == false) {
    string warnings = "Speed ZAIC problems " + spd_zaic.getWarnings();
    postWMessage(warnings);
    return(0);
  }

  double rel_ang_to_wpt = relAng(m_osx, m_osy, m_nextpt.x(), m_nextpt.y());
  ZAIC_PEAK crs_zaic(m_domain, "course");
  crs_zaic.setSummit(rel_ang_to_wpt);
  crs_zaic.setPeakWidth(0);
  crs_zaic.setBaseWidth(180.0);
  crs_zaic.setSummitDelta(0);
  crs_zaic.setValueWrap(true);
  if(crs_zaic.stateOK() == false) {
    string warnings = "Course ZAIC problems " + crs_zaic.getWarnings();
    postWMessage(warnings);
    return(0);
  }

  IvPFunction *spd_ipf = spd_zaic.extractIvPFunction();
  IvPFunction *crs_ipf = crs_zaic.extractIvPFunction();

  OF_Coupler coupler;
  IvPFunction *ivp_function = coupler.couple(crs_ipf, spd_ipf, 50, 50);

  return(ivp_function);
}
