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
  addInfoVars("NAV_X, NAV_Y, SOURCE_ANGLE, WHISTLE_EXIST, MEASURE_START","no_warning");

    m_ipf_type          = "zaic";
    m_measure_start     = "false";
    m_source_quadrant   = "pending";  


    m_desired_speed = 2;

    m_osx         = 0;
    m_osy         = 0;
    m_center_x    = 0;
    m_center_y    = 0;
    m_line_angle1 = 0;
    m_line_angle2 = 0;


    m_angle1_get    = false;
    m_angle2_get    = false;
    m_angle3_get    = false;

    m_whistle_exist = false;

    m_first_time_change_heading = true;
    m_second_time_checking      = false;
    m_third_time_check          = true;
    m_start_gen_next_pt         = false;
    m_check_source_point        = false;
    m_gen_ipf                   = false;

    m_arrive            = false;
    m_second_poke_hold   = true;
    


    m_source_angle_buff.clear();

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
  else if (param == "method"){
        m_method = val;
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
  else if (param == "radius" && (double_val > 0) && (isNumber(val))){
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


  bool ok1,ok2,ok3,ok4,ok5;
  m_osx = getBufferDoubleVal("NAV_X",ok1);
  m_osy = getBufferDoubleVal("NAV_Y",ok2);

  if(!ok1 || !ok2)
    postWMessage("No ownship X/Y info in info_buffer");
  
    string str_whistle_exist = getBufferStringVal("WHISTLE_EXIST",ok3);
    if(str_whistle_exist == "true"){
        m_whistle_exist = true;
        postMessage("WHISTLE_ONCE_EXIST","true");
    }

    if(m_method == "region"){
            
    }

    else if(m_method == "two_point_measure"){
        string m_measure_start = getBufferStringVal("MEASURE_START",ok4);

        if(m_measure_start == "true"){
            
            double measure_angle = getBufferDoubleVal("SOURCE_ANGLE",ok5);
            if(ok5 && measure_angle !=400){
                m_source_angle_buff.push_back(measure_angle);
                postMessage("SOURCE_ANGLE",400);
            }
// change source angle to 400 means that I got the angle, won't get agian next time
        
            
            if(m_source_angle_buff.size() >= m_check_num){
                std::vector<double> check_buff(m_check_num,0);
                for(int i=0;i<m_check_num;i++)
                    check_buff[i] = m_source_angle_buff[i];
                    
                double angle_after_check = getMedian(check_buff);
                
                if(!m_angle1_get){
                    m_angle1 = angle_after_check;
                    m_angle1_get = true;
                }
                else if(!m_angle2_get){
                    m_angle2 = angle_after_check;
                    m_angle2_get = true;
                }
                else if(!m_angle3_get){
                    m_angle3 = angle_after_check;
                    m_angle3_get = true;
                
                }
                m_source_angle_buff.clear();
            }
        }

        if(m_angle1_get && m_first_time_change_heading){
            postMessage("ANGLE_ONE", m_angle1);
            if(m_angle1 >= 0 ){
                postMessage("HOLD_HEADING",90);
                postMessage("DEPLOY","hold");
                postMessage("MEASURE_START","false");
            }
            else if(m_angle1 < 0){
                postMessage("HOLD_HEADING",270);
                postMessage("DEPLOY","hold");
                postMessage("MEASURE_START","false");
            }
            m_first_time_change_heading = false;
            m_second_time_checking      = true;
        }
        else if(m_angle2_get && m_second_time_checking){
            postMessage("ANGLE_TWO", m_angle2);
            m_first_line_point_x = m_osx; 
            m_first_line_point_y = m_osy; 

            if(m_angle1 >0 && m_angle2 <0){
                m_source_quadrant = "one"; 
                m_line_angle1 = m_angle1;
            }           
            else if(m_angle1 <0 && m_angle2>0){
                m_source_quadrant = "two"; 
                m_line_angle1 = 270 + m_angle2;
            }
            else if(m_angle1 <0 && m_angle2<0){
                m_source_quadrant = "three"; 
                m_line_angle1 = 270 + m_angle2;
            }
            else if(m_angle1 >0 && m_angle2 >0){
                m_source_quadrant = "four"; 
                m_line_angle1 = 90 + m_angle1;
            }
            m_second_time_checking = false;
            m_start_gen_next_pt = true;    
            m_gen_ipf = true;
            postMessage("SOURCE_QUADRANT",m_source_quadrant);
            postMessage("MEASURE_START","false");
            postMessage("DEPLOY","true");
            ShowLine(m_line_angle1,m_first_line_point_x,m_first_line_point_y,"line1");
        }
        else if(m_angle3_get && m_third_time_check){
            
            if(m_source_quadrant == "one" || m_source_quadrant =="two")
                m_line_angle2 = m_angle3;
            else
                m_line_angle2 = m_angle3+180;
            
            m_second_line_point_x = m_osx;
            m_second_line_point_y = m_osy;

            m_third_time_check = false;
            m_check_source_point = true;
            ShowLine(m_line_angle2,m_second_line_point_x,m_second_line_point_y,"line2");
        }
        else if(m_start_gen_next_pt && m_gen_ipf){
            genNextPt(m_angle1);
            ipf = buildFunctionWithZAIC();
        }
        if(m_arrive && m_second_poke_hold){
            if(m_source_quadrant == "one" || m_source_quadrant == "two")
                postMessage("HOLD_HEADING",0);
            else
                postMessage("HOLD_HEADING",180);

            postMessage("DEPLOY","hold");
            m_second_poke_hold = false;
        }
#ifdef WIN32
        double dist = _hypot((m_nextpt.x()-m_osx),(m_nextpt.y()-m_osy));
#else
        double dist = hypot((m_nextpt.x()-m_osx),(m_nextpt.y()-m_osy));
#endif
        if(dist <=m_arrival_radius){
            m_gen_ipf = false;
            m_arrive = true;
        }
// check line equation
        if(m_check_source_point){
        
            postMessage("ANGLE_ONE",m_angle1);
            postMessage("ANGLE_TWO",m_angle2);
            postMessage("ANGLE_THREE",m_angle3);
            postMessage("LINE_ANGLE_ONE",m_line_angle1);
            postMessage("LINE_ANGLE_TWO",m_line_angle2);
            postMessage("LINE_POINT_X_ONE",m_first_line_point_x);
            postMessage("LINE_POINT_Y_ONE",m_first_line_point_y);
            postMessage("LINE_POINT_X_TWO",m_second_line_point_x);
            postMessage("LINE_POINT_Y_TWO",m_second_line_point_y);
           
            CheckSourceAngle();

            setComplete();
        }
    }
    else 
        postWMessage("Unknow configure \"method\", choose region or two_point_measure");

    
  // Part N: Prior to returning the IvP function, apply the priority wt
  // Actual weight applied may be some value different than the configured
  // m_priority_wt, depending on the behavior author's insite.
  if(ipf)
    ipf->setPWT(m_priority_wt);

  return(ipf);
}

bool BHV_FixSourceFinding::ShowLine(double angle, double x,double y, string label){

    stringstream ss;
    ss<<"x="<<x<<",y="<<y<<",mag=40,ang="<<angle<<",label="<<label;
    string msg = ss.str();
    postMessage("VIEW_VECTOR",msg);
}

bool BHV_FixSourceFinding::CheckSourceAngle(){
}

double BHV_FixSourceFinding::getMedian(vector<double> input){

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

bool BHV_FixSourceFinding::genNextPt(double angle){

    double next_x;
    double next_y;
    if( angle >=0 )
        next_x = m_right_bd -10;
    else
        next_x = m_left_bd + 10;

    m_nextpt.set_vx(next_x);
//    m_nextpt.set_vy(m_osy); 
    m_nextpt.set_vy(m_center_y); 

    postMessage("NEXT_X",next_x);
    postMessage("NEXT_Y",m_center_y);

    postViewPoint(true);
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
