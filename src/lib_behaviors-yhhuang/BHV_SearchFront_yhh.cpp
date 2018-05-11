/************************************************************/
/*    NAME: yhh                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_SearchFront_yhh.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include "MBUtils.h"
#include "BuildUtils.h"
#include "AngleUtils.h"
#include "BuildUtils.h"
#include "OF_Coupler.h"
#include "OF_Reflector.h"
#include "BHV_SearchFront_yhh.h"
#include <math.h>
//#include <armadillo>
using namespace std;
//using namespace arma;
//---------------------------------------------------------------
// Constructor

BHV_SearchFront_yhh::BHV_SearchFront_yhh(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "defaultname");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "course,speed");

  // Add any variables this behavior needs to subscribe for
  addInfoVars("NAV_X, NAV_Y, UCTD_MSMNT_REPORT, DESIRED_HEADING","no_warning");
  

  m_arrival_radius = 10;
  m_desired_speed  = 0;
  m_ipf_type       = "zaic";

  m_report    = "";
  m_request   = "";
  m_index_str = "";
  m_direction = "";
  m_temp_dbl_buff.clear();
  m_next_pntx.clear();
  m_next_pnty.clear();
  
  m_time0          = 0;
  m_time1          = 0;
  m_avg_number     = 0;
  m_heading        = 0;
  m_curr_time      = 0;
  m_arrive_time    = 0;
  m_osx            = 0;
  m_osy            = 0;
  m_middle_x       = 0;
  m_middle_y       = 0;
  m_width          = 0;
  m_length         = 0;
  m_threshold      = 2;

  m_index          = 0;
  m_point_index    = 0;
  m_checking_start_index = 0;
  m_generate_point = true; 
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_SearchFront_yhh::setParam(string param, string val)
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  // Get the numerical value of the param argument for convenience once
  double double_val = atof(val.c_str());
  
  if((param == "foo") && isNumber(val)) {
    // Set local member variables here
    return(true);
  }
  if((param == "middle_ptx")  && (isNumber(val))) {
    m_middle_x = double_val;
    return(true);
  }
  else if((param == "middle_pty") && (isNumber(val))) {
    m_middle_y = double_val;
    return(true);
  }
  else if((param == "width") && (double_val > 0) && (isNumber(val))) {
    m_width = double_val;
    return(true);
  }
  else if((param == "length") && (double_val > 0) && (isNumber(val))) {
    m_length = double_val;
    return(true);
  }

  else if(param == "direction"){
    m_direction = val;
    return(true);
  }
  else if(param == "threshold"){
    m_threshold = double_val;
    return(true);
  }
  else if(param == "vname"){
    string vname = val;
    m_request  = "vname=";
    m_request +=  vname;
    
    return(true);
  }
  else if(param == "average_number"){
    m_avg_number = double_val;
    
    return(true);
  }
  else if((param == "speed") && (double_val > 0) && (isNumber(val))){
    m_desired_speed = double_val;
   return(true); 
  }
  else if((param == "radius") && (double_val > 0) && (isNumber(val))){
    m_arrival_radius = double_val;
   return(true); 
  }
  // If not handled above, then just return false;
  return(false);
}
// Procedure: postViewPoint

void BHV_SearchFront_yhh::postViewPoint(bool viewable) 
{
  m_nextpt.set_label(m_us_name + "'s next waypoint");
  
  string point_spec;
  if(viewable)
    point_spec = m_nextpt.get_spec("active=true");
  else
    point_spec = m_nextpt.get_spec("active=false");
  postMessage("VIEW_POINT", point_spec);
}


//---------------------------------------------------------------
// Procedure: onSetParamComplete()
//   Purpose: Invoked once after all parameters have been handled.
//            Good place to ensure all required params have are set.
//            Or any inter-param relationships like a<b.

void BHV_SearchFront_yhh::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_SearchFront_yhh::onHelmStart()
{
    m_time0 = getBufferCurrTime(); 
        postMessage("TIME_ZERO",m_time0);
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_SearchFront_yhh::onIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_SearchFront_yhh::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_SearchFront_yhh::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_SearchFront_yhh::onIdleToRunState()
{
    
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_SearchFront_yhh::onRunToIdleState()
{
}

void BHV_SearchFront_yhh::GenCirclePoint()
{
   if(m_direction == "cclock"){ 
//x
     m_next_pntx.push_back(m_middle_x - m_width/2);
     m_next_pntx.push_back(m_middle_x - m_width/2);
     m_next_pntx.push_back(m_middle_x + m_width/2);
     m_next_pntx.push_back(m_middle_x + m_width/2);
//y     
     m_next_pnty.push_back(m_middle_y + m_length/2); 
     m_next_pnty.push_back(m_middle_y - m_length/2); 
     m_next_pnty.push_back(m_middle_y - m_length/2); 
     m_next_pnty.push_back(m_middle_y + m_length/2); 
   }
   else if(m_direction == "clock"){ 
//x
     m_next_pntx.push_back(m_middle_x + m_width/2);
     m_next_pntx.push_back(m_middle_x + m_width/2);
     m_next_pntx.push_back(m_middle_x - m_width/2);
     m_next_pntx.push_back(m_middle_x - m_width/2);
//y     
     m_next_pnty.push_back(m_middle_y + m_length/2); 
     m_next_pnty.push_back(m_middle_y - m_length/2); 
     m_next_pnty.push_back(m_middle_y - m_length/2); 
     m_next_pnty.push_back(m_middle_y + m_length/2); 
    
   
   
   }

}
//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_SearchFront_yhh::onRunState()
{
  // Part 1: Build the IvP function
  IvPFunction *ipf = 0;
  bool ok,ok2,ok3,ok4;
        m_time1 = getBufferCurrTime();
        postMessage("UCTD_SENSOR_REQUEST","CHANGE");
   //     postMessage("TIME_ONE",m_time1);
        if(m_time1 > m_time0+1){    
  //        postMessage("TIME_ZERO",m_time0);
          postMessage("UCTD_SENSOR_REQUEST",m_request);
          m_time0 = getBufferCurrTime();
        }
      
      m_report = getBufferStringVal("UCTD_MSMNT_REPORT",ok);
      if(ok){
          
        double temperature_dbl,x_dbl,y_dbl; 
        string temperature_str = tokStringParse(m_report, "temp", ',', '=');
        string x_str = tokStringParse(m_report,"x",',','=');
        string y_str = tokStringParse(m_report,"y",',','=');

          stringstream ss_temp,ss_x,ss_y;

          ss_temp<<temperature_str;
          ss_temp>>temperature_dbl;
          ss_x<<x_str;
          ss_x>>x_dbl;
          ss_y<<y_str;
          ss_y>>y_dbl;

          array<double,3> array_xytemp={x_dbl,y_dbl,temperature_dbl};

          m_temp_dbl_buff.push_back(array_xytemp);

 //       if(!m_temp_dbl_buff.empty()){
 //         postMessage("TEMPERATURE_DBL",m_temp_dbl_buff.front()[2]);
 //         postMessage("X_DBL",m_temp_dbl_buff.front()[0]);
 //         postMessage("Y_DBL",m_temp_dbl_buff.front()[1]);
 //         m_temp_dbl_buff.pop_front();
 //       }
      }

// Finding temerature vary point

      int check_interval = 10;
       if(!m_temp_dbl_buff.empty() && m_checking_start_index+check_interval < m_temp_dbl_buff.size()){
         double temp_checking,temp_checking_two;
             
         temp_checking     = m_temp_dbl_buff[m_checking_start_index][2];
         temp_checking_two = m_temp_dbl_buff[m_checking_start_index+check_interval][2];

           postMessage("DIFFERENCE",fabs(temp_checking-temp_checking_two)); 
         if(fabs(temp_checking-temp_checking_two)>m_threshold){
          
           double temp_diff_x = m_temp_dbl_buff[m_checking_start_index+check_interval/2][0];
           double temp_diff_y = m_temp_dbl_buff[m_checking_start_index+check_interval/2][1];
           m_temp_diff_point.set_vx(temp_diff_x);
           m_temp_diff_point.set_vy(temp_diff_y);
           m_temp_diff_point.set_label("difference point");
           postMessage("VIEW_POINT",m_temp_diff_point.get_spec());
           //postMessage("STARTING_POINT",);
         }

         m_temp_dbl_buff.pop_front();
       }

       // Part 1: Get vehicle position from InfoBuffer and post a 
  // warning if problem is encountered
  m_osx = getBufferDoubleVal("NAV_X", ok3);
  m_osy = getBufferDoubleVal("NAV_Y", ok4);
  if(!ok3 || !ok4) {
    postWMessage("No ownship X/Y info in info_buffer.");
    return(0);
  }


  if(m_generate_point){
    GenCirclePoint();
    m_generate_point = false;
  }

   // Part 2: Determine if the vehicle has reached the destination 
  // point and if so, declare completion.
#ifdef WIN32
  double dist = _hypot((m_nextpt.x()-m_osx), (m_nextpt.y()-m_osy));
#else
  double dist = hypot((m_nextpt.x()-m_osx), (m_nextpt.y()-m_osy));
#endif
  if(dist <= m_arrival_radius) {
    if(!m_next_pnty.empty() && !m_next_pntx.empty()){
 
      m_nextpt.set_vx(m_next_pntx.front()); 
      m_nextpt.set_vy(m_next_pnty.front());
      m_next_pntx.pop_front();
      m_next_pnty.pop_front();
      m_point_index +=1;
    }
    else if(m_point_index !=0){ 
        setComplete();
        postViewPoint(false);
        return(0);
    }
  }

  postViewPoint(true);
// Part 4: Build the IvP function with either the ZAIC coupler 
  // or the Reflector tool.
    ipf = buildFunctionWithZAIC();
  if(ipf == 0) 
    postWMessage("Problem Creating the IvP Function");
  if(ipf)
    ipf->setPWT(m_priority_wt);
  return(ipf);


//using average 
    
//        if(m_temp_dbl_buff.size()>=m_avg_number){
//          
//         vec x = zeros<vec>(m_avg_number);
//         double avg;
//         double sum;
//           
//          for(int i=0;i<m_avg_number;i++){
//              sum += m_temp_dbl_buff.front();
//              m_temp_dbl_buff.pop_front();
//          }
//          avg = sum/m_avg_number;
//          postMessage("AVG_FOR_TEN",avg);
//        }
        
//using curve fitting
//
//      postMessage("BUFF_SIZE",m_temp_dbl_buff.size());
//      if(m_temp_dbl_buff.size()>=m_avg_number){
//      
//          vec x = zeros<vec>(m_avg_number);
//          vec y = zeros<vec>(m_avg_number);
//          
//         for(int i=0;i<m_avg_number;i++){
//         
//            x(i) = i;
//            y(i) = m_temp_dbl_buff.front();
//            m_temp_dbl_buff.pop_front();
//         } 
//       postMessage("X_ONE",x(1));  
//         vec p = polyfit(x,y,1);
//
//         if(abs(p(0))>0.13){
//          m_arrive_time = getBufferCurrTime();
//          m_heading = getBufferDoubleVal("DESIRED_HEADING",ok2);
//         }   
//         m_curr_time = getBufferCurrTime();
//         
//    if(m_curr_time>=m_arrive_time+2 && m_curr_time<=m_arrive_time+180){  
//          ipf = Gen_Zaic_peak(m_heading+180);
//    }
//    else
//        ipf = Gen_Zaic_peak(0);
//             postMessage("CHANGE_HEADING","NOT_CHANGED");
//
//         postMessage("POLYFIT_PARA_ZERO",p(0));
//         postMessage("POLYFIT_PARA_ONE",p(1));
//      }
  // Part N: Prior to returning the IvP function, apply the priority wt
  // Actual weight applied may be some value different than the configured
  // m_priority_wt, depending on the behavior author's insite.
  if(ipf)
    ipf->setPWT(m_priority_wt);

  return(ipf);
}

IvPFunction *BHV_SearchFront_yhh::buildFunctionWithZAIC() 
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
