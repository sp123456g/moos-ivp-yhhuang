/************************************************************/
/*    NAME: yhhuang                                         */
/*    ORGN: NTU, Taipei                                            */
/*    FILE: BHV_SourceTracking.cpp                          */
/*    DATE: Jan 11th, 2019                                   */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include "BHV_SourceTracking.h"
#include "MBUtils.h"
#include "BuildUtils.h"
#include "AngleUtils.h"
#include "MBUtils.h"
#include "OF_Coupler.h"
#include "OF_Reflector.h"
#include <math.h>
#include <cmath>
#include <sstream>


using namespace std;

//---------------------------------------------------------------
// Constructor

BHV_SourceTracking::BHV_SourceTracking(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "defaultname");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "course,speed");

  // Add any variables this behavior needs to subscribe for
  addInfoVars("NAV_X, NAV_Y, SOURCE_ANGLE, WHISTLE_EXIST, BAND_AVG_SPL","no_warning");

    m_ipf_type          = "zaic";

    m_target_angle  = 0;
    m_check_num     = 5;

    m_left_bd       = 0;
    m_right_bd      = 100;
    m_up_bd         = 100;
    m_low_bd        = 0;
    m_band_level_thr= 100;

    m_arrival_radius= 5;
    
    m_osx           = 0;
    m_osy           = 0;
    m_osheading     = 0;

    m_fast_speed    = 2.5;
    m_slow_speed    = 1;
    m_desired_speed = 2.5;

    m_gen_ipf       = false;
    m_arrive        = false;

    m_no_dolphin_time = 0;
    m_no_wh_time    = 20;

    m_whistle_exist = false;
    m_first_time    = true;
    m_over_thr      = false;
}

// Procedure: postViewPoint
//---------------------------------------------------
void BHV_SourceTracking::postViewPoint(bool viewable)
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

bool BHV_SourceTracking::setParam(string param, string val)
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
  else if ((param == "slow_speed") && (double_val > 0) && (isNumber(val))){
        m_slow_speed = double_val;
        return(true);
  }
  else if ((param == "fast_speed") && (double_val > 0) && (isNumber(val))){
        m_fast_speed = double_val;
        return(true);
  }

  else if (param == "median_check_size"){
        m_check_num = double_val;
        return(true);
  }
  else if (param == "left_bound" && (isNumber(val))){
        m_left_bd = double_val;
        return(true);
  }
  else if (param == "right_bound" && (isNumber(val))){
        m_right_bd = double_val;
        return(true);
  }
  else if (param == "up_bound" && (isNumber(val))){
        m_up_bd = double_val;
        return(true);
  }
  else if (param == "low_bound" && (isNumber(val))){
        m_low_bd = double_val;
        return(true);
  }


  else if (param == "arrive_radius" && (double_val > 0) && (isNumber(val))){
    m_arrival_radius = double_val;
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

void BHV_SourceTracking::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_SourceTracking::onHelmStart()
{
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_SourceTracking::onIdleState()
{
    
    bool ok;
    string str_in = getBufferStringVal("WHISTLE_EXIST",ok);
    if(str_in == "true")
        m_whistle_exist = true;
    if(m_whistle_exist && ok)
        postMessage("TRACK","true");
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_SourceTracking::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_SourceTracking::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_SourceTracking::onIdleToRunState()
{
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_SourceTracking::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_SourceTracking::onRunState()
{
// Part 1: Build the IvP function
  IvPFunction *ipf = 0;
// get current position and heading
  bool ok1,ok2,ok3,ok4,ok5;
  m_osx = getBufferDoubleVal("NAV_X",ok1);
  m_osy = getBufferDoubleVal("NAV_Y",ok2);
  m_osheading = getBufferDoubleVal("NAV_HEADING",ok3);

  if(!ok1 || !ok2)
    postWMessage("No ownship X/Y info in info_buffer");
//  check whistle exist 
    string str_whistle_exist = getBufferStringVal("WHISTLE_EXIST",ok4);
    if(str_whistle_exist == "true"){
        m_whistle_exist = true;
        m_gen_ipf = true;
    }
    
//median filter with angle
    if(m_whistle_exist){
        double measure_angle = getBufferDoubleVal("SOURCE_ANGLE",ok5);
        if(ok5 && measure_angle !=400)
            m_source_angle_buff.push_back(measure_angle);
        
        if(m_source_angle_buff.size() >= m_check_num){
            std::vector<double> check_buff(m_check_num,0);
            for(int i=0;i<m_check_num;i++)
                check_buff[i] = m_source_angle_buff[i];

            m_target_angle = getMedian(check_buff);
        }              
//Calculate next point and check if it's possible
        CheckNextSpeed();
        CheckBound();

        postMessage("TARGET_ANGLE",m_target_angle);
        postMessage("TARGET_SPEED",m_desired_speed);
        if(m_gen_ipf)
          ipf = buildFunctionWithZAIC();
        
#ifdef WIN32
        double dist = _hypot((m_nextpt.x()-m_osx),(m_nextpt.y()-m_osy));
#else
        double dist = hypot((m_nextpt.x()-m_osx),(m_nextpt.y()-m_osy));
#endif
        if(dist <=m_arrival_radius)
            m_arrive = true;
    }
    else{
        postMessage("WAITING_FOR_DOLPHIN",true);    
        m_no_dolphin_time +=1;

        if(m_no_dolphin_time >= m_no_wh_time){
            m_gen_ipf           = false;
            m_no_dolphin_time   = 0;
        }
    }

    
  // Part N: Prior to returning the IvP function, apply the priority wt
  // Actual weight applied may be some value different than the configured
  // m_priority_wt, depending on the behavior author's insite.
  if(ipf)
    ipf->setPWT(m_priority_wt);

  return(ipf);
}

bool BHV_SourceTracking::ShowLine(double angle, double x,double y, string label){

    stringstream ss;
    ss<<"x="<<x<<",y="<<y<<",mag=10,ang="<<angle<<",label="<<label;
    string msg = ss.str();
    postMessage("VIEW_VECTOR",msg);
}

double BHV_SourceTracking::getMedian(vector<double> input){

    int size = input.size();

    for (unsigned int m = 0; m < (size+1)/2; ++m){
        unsigned int min = m;
        for (unsigned int n = m + 1; n < size; ++n)
            if (input[n] < input[min])
                min = n;
        //   Put found minimum element in its place
        float temp = input[m];
        input[m] = input[min];
        input[min] = temp;
    }
    double angle = input[(size-1)/2];

    return(angle);
}

bool BHV_SourceTracking::CheckNextSpeed(){

    bool ok;
    double band_avg = getBufferDoubleVal("BAND_AVG_SPL",ok);

    if(m_first_time){
        m_over_thr = band_avg;
        m_first_time = false;
        m_desired_speed = m_fast_speed;
        m_over_thr = true;
    }
    else if(ok){    
        if(band_avg > m_band_level_thr)
            m_over_thr = true;
    }
    else 
        m_desired_speed = m_fast_speed;
    
    if(m_over_thr)
        m_desired_speed = m_fast_speed;
    else 
        m_desired_speed = m_slow_speed;
}

bool BHV_SourceTracking::CheckBound(){

}

IvPFunction *BHV_SourceTracking::buildFunctionWithZAIC()
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
  crs_zaic.setSummit(m_osheading + m_target_angle);
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
