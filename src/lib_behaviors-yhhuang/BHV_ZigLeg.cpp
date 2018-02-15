/************************************************************/
/*    NAME: yhhuang                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_ZigLeg.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include "MBUtils.h"
#include "BuildUtils.h"
#include "BHV_ZigLeg.h"
#include <ZAIC_PEAK.h>
#include "math.h"
using namespace std;

//---------------------------------------------------------------
// Constructor

BHV_ZigLeg::BHV_ZigLeg(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "defaultname");
  m_domain = subDomain(m_domain,"course,speed");
  // Declare the behavior decision space

  // Add any variables this behavior needs to subscribe for
  addInfoVars("NAV_X, NAV_Y, DESIRED_HEADING");
  addInfoVars("WPT_INDEX","nowarning");
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_ZigLeg::setParam(string param, string val)
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  // Get the numerical value of the param argument for convenience once
  double double_val = atof(val.c_str());
  
  if((param == "foo") && isNumber(val)) {
    // Set local member variables here
  }
  else if (param == "bar") {
    // return(setBooleanOnString(m_my_bool, val));
  }
  else if (param == "pwt"){
      m_priority_wt = double_val;
    return(true);
  }
  else if (param == "zig_duration"){
      m_duration = double_val;
    return(true);
  }
  else if (param == "angle"){
      m_angle = double_val;
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

void BHV_ZigLeg::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_ZigLeg::onHelmStart()
{
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_ZigLeg::onIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_ZigLeg::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_ZigLeg::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_ZigLeg::onIdleToRunState()
{
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_ZigLeg::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_ZigLeg::onRunState()
{
  // Part 1: Check if WPT_INDEX will has changed or not
  IvPFunction *ipf=0;    //Initialize the Ivpfunction
  bool ok,ok2,ok3;
        
        m_index = getBufferDoubleVal("WPT_INDEX",ok2);    
        m_wpt_index_buff=getBufferDoubleVector("WPT_INDEX",ok3);
        size_t buff_size = m_wpt_index_buff.size();
           postMessage("BUFFER_SIZE",buff_size); 
          m_get_index = getBufferTimeVal("WPT_INDEX"); //if it got the variable in the beginning, the value will equal 0
    if(buff_size==1){     //If get to wpt, the buffer will have two variable, because of history data
        m_arrive_time = getBufferCurrTime();
        postMessage("ARRIVE_TIME",floor(m_arrive_time));
        m_heading = getBufferDoubleVal("DESIRED_HEADING",ok);
    }
    m_curr_time = getBufferCurrTime();
   
    if(m_curr_time>=m_arrive_time+5 && m_curr_time<=m_arrive_time+m_duration){  //If current_time is between 5~duration time will generate the IvPFunction

            postMessage("CURRENT_TIME",floor(m_curr_time));   //Just use to check the time variable
        if(m_index!=0){        //avoid the wpt_index=0 case
                    ipf = Gen_Zaic_peak();
                    postMessage("GENERATE","true");
        }
    }
    
    m_curr_time = getBufferCurrTime();   //count the time
  
  // Part N: Prior to returning the IvP function, apply the priority wt
  // Actual weight applied may be some value different than the configured
  // m_priority_wt, depending on the behavior author's insite.
  if(ipf)                         //Set up the pwt
    ipf->setPWT(m_priority_wt);

  return(ipf);
}

IvPFunction* BHV_ZigLeg::Gen_Zaic_peak(){

    //step1 Generate domain, but there is a m_domain can use
    //step2 build zaic_peak tool ,course is heading
    ZAIC_PEAK crs_zaic(m_domain,"course");
    //step3 set up the zaic_peak
    crs_zaic.setSummit(m_heading+m_angle);  //current heading+angle you want, because the heading here is the absolute value
    crs_zaic.setPeakWidth(0);
    crs_zaic.setBaseWidth(180.0);
    crs_zaic.setSummitDelta(0);
    crs_zaic.setValueWrap(true);
    //make warning if the course zaic have problems
    if(crs_zaic.stateOK()==false){
        string warnings = "Course ZAIC problems" + crs_zaic.getWarnings();
        postWMessage(warnings);
        return(0);    
    }
    //step4 Extract the ipf function
    IvPFunction *crs_ipf = crs_zaic.extractIvPFunction();
    
    return(crs_ipf);
};
