/************************************************************/
/*    NAME: yhhuang                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_Pulse.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include "MBUtils.h"
#include "BuildUtils.h"
#include "BHV_Pulse.h"

using namespace std;

//---------------------------------------------------------------
// Constructor

BHV_Pulse::BHV_Pulse(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "defaultname");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "course,speed");

  // Add any variables this behavior needs to subscribe for
  addInfoVars("NAV_X","nowarning");
  addInfoVars("NAV_Y","nowarning");
  addInfoVars("WPT_INDEX","nowarning");
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_Pulse::setParam(string param, string val)
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  // Get the numerical value of the param argument for convenience once
  double double_val = atof(val.c_str());
  
  if((param == "name") && isNumber(val)) {
    // Set local member variables here
    return(true);
  }
  else if (param == "pulse_range") {

      m_range = double_val;
    // return(setBooleanOnString(m_my_bool, val));
    return(true);
  }
  else if (param == "pulse_duration"){
    m_pulse_duration = double_val;
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

void BHV_Pulse::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_Pulse::onHelmStart()
{
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_Pulse::onIdleState()
{
       
    
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_Pulse::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_Pulse::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_Pulse::onIdleToRunState()
{
    
    getBufferCurrTime();
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_Pulse::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_Pulse::onRunState()
{
  // Part 1: Build the IvP function
  IvPFunction *ipf = 0;
//check the wpt_index 
    bool ok1,ok2,ok3;
     m_index =  getBufferDoubleVal("WPT_INDEX",ok1);
     m_osx   =  getBufferDoubleVal("NAV_X",ok2);
     m_osy   =  getBufferDoubleVal("NAV_Y",ok3);
    if(!ok1){
        if(m_index>0)
        postWMessage("No Waypoint index found!\n");
    }
    else if(!ok2)
        postWMessage("No NAV_X\n");
    else if(!ok3)
        postWMessage("No NAV_Y\n");
   //If the WPT_INDEX changed, m_alter will be zero,else it will be -1
    m_alter = getBufferTimeVal("WPT_INDEX");
    postMessage("M_ALTER",m_alter);
    //check if index has increase or not
    if(m_alter==0){
         m_arrive_time = getBufferCurrTime(); //Get the exact altering time
    }
    m_curr_time = getBufferCurrTime();    //Get the current time
//if index increase, calculate 5 secs and then output the pulse
        if(m_curr_time=(m_arrive_time+5)){ //Calculate 5 seconds
           pulse.set_x(m_osx);
           pulse.set_y(m_osy);
           pulse.set_label("bhv_pulse");
           pulse.set_rad(m_range);
           pulse.set_duration(m_pulse_duration);
           pulse.set_time(m_curr_time);
           pulse.set_color("edge", "pink");
           pulse.set_color("fill", "pink");
        
           string spec = pulse.get_spec();
           postMessage("VIEW_RANGE_PULSE", spec);    //Output the VIEW_RANGE_PULSE 
        }
    
            
  // Part N: Prior to returning the IvP function, apply the priority wt
  // Actual weight applied may be some value different than the configured
  // m_priority_wt, depending on the behavior author's insite.
  if(ipf)
    ipf->setPWT(m_priority_wt);

  return(ipf);
}

