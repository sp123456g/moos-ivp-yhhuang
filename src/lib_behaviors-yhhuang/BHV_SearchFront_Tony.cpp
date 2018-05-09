/************************************************************/
/*    NAME: yhh                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_SearchFront_Tony.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <iostream>
#include <cstdlib>
#include <math.h>
#include "MBUtils.h"
#include "BuildUtils.h"
#include "BHV_SearchFront_Tony.h"
#include "AOF_SimpleWaypoint.h"
#include "OF_Reflector.h"

//#include <armadillo>
using namespace std;
//using namespace arma;
//---------------------------------------------------------------
// Constructor

BHV_SearchFront_Tony::BHV_SearchFront_Tony(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "search_front");

  //V1 Wave doesn't move and follow the wave
  //------------------------------------
  /*
  m_omega = 0;
  m_desire_speed = 0;
  m_osx = 0;
  m_osy = 0;
  m_nextx = -70;
  m_nexty = -30;
  m_arrival_radius = 5;
  m_temperature_record = 0;
  m_temperature_now   = 0;
  m_temperature_thresold = 6.0;
  m_temperature_record_update = true;
  m_vehicle_go_down = true;
  m_vehicle_go_more = true;
  */
  //------------------------------------

  //V2 Wave moving
  //------------------------------------  
  /*
  m_omega = 0;
  m_desire_speed = 0;
  m_osx = 0;
  m_osy = 0;
  m_nextx = 0;
  m_nexty = 0;
  m_arrival_radius = 5;
  m_first_round_index = 0;
  //initial first round
  m_first_round_points[0].set_vertex(-40, -50);
  m_first_round_points[1].set_vertex(-80, -75);
  m_first_round_points[2].set_vertex(-40, -185);
  m_first_round_points[3].set_vertex(165, -185);
  m_first_round_points[4].set_vertex(170, 5);
  m_first_round_points[5].set_vertex(110, 10);
  m_first_round_points[6].set_vertex(-40, -50);
  */
  //------------------------------------  


  //V3 Wave moving, ship vertial moving
  //------------------------------------
  m_omega = 0;
  m_desire_speed = 0;
  m_osx = 0;
  m_osy = 0;
  m_nextx = 0;
  m_nexty = 0;
  m_arrival_radius = 5;
  //------------------------------------  


  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "course,speed");

  // Add any variables this behavior needs to subscribe for
  addInfoVars("NAV_X, NAV_Y, UCTD_MSMNT_REPORT, DESIRED_HEADING","no_warning");
    
  m_report = "";
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_SearchFront_Tony::setParam(string param, string val)
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  // Get the numerical value of the param argument for convenience once
  double double_val = atof(val.c_str());
  
  if((param == "foo") && isNumber(val)) {
    // Set local member variables here
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
  else if(param == "omega"){
    m_omega = double_val;
    
    return(true);
  }
  else if(param == "speed"){
    m_desire_speed = double_val;
    
    return(true);
  }
  else if (param == "bar") {
    // return(setBooleanOnString(m_my_bool, val));
  }

  // If not handled above, then just return false;
  return(false);
}

//---------------------------------------------------------------
// Procedure: onSetParamComplete()
//   Purpose: Invoked once after all parameters have been handled.
//            Good place to ensure all required params have are set.
//            Or any inter-param relationships like a<b.

void BHV_SearchFront_Tony::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_SearchFront_Tony::onHelmStart()
{
    m_time0 = getBufferCurrTime(); 
        postMessage("TIME_ZERO",m_time0);
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_SearchFront_Tony::onIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_SearchFront_Tony::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_SearchFront_Tony::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_SearchFront_Tony::onIdleToRunState()
{
    
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_SearchFront_Tony::onRunToIdleState()
{
}

//-----------------------------------------------------------------
void BHV_SearchFront_Tony::giveNextPoint()
{
  //V1 Wave doesn't move and follow the wave
  //------------------------------------
  /*
  postMessage("TEMP_DIS",m_temperature_record - m_temperature_now);
  double move = 10;
  if(fabs(m_temperature_record - m_temperature_now) > m_temperature_thresold)
  {    
    if(m_vehicle_go_more){
      m_vehicle_go_more = false;
    }
    else{
      m_vehicle_go_down = !m_vehicle_go_down;
      m_nextx += 20;
      m_temperature_record_update = true;
      m_vehicle_go_more = true;   
    }

  }
  if(m_vehicle_go_down)
    m_nexty -= move;
  else
    m_nexty += move;
    */
  //------------------------------------

  //V2 Wave moving
  //------------------------------------
  //first round to find two points
  /*
  if(m_first_round_index<7)
  {
    m_nextx = m_first_round_points[m_first_round_index].x();
    m_nexty = m_first_round_points[m_first_round_index].y(); 
    m_first_round_index += 1;
  }
  else if(m_first_round_index == 7)
  {
    m_first_round_index += 1;
    //find the start point
    double tmp_max=-50, tmp_min=100;
    double index_max, index_min;
    double mid_x, mid_y;
    for(int i = 0 ; i< m_temp_dbl_buff.size()/2;i++){
        if(m_temp_dbl_buff[i][2] > tmp_max){
          tmp_max = m_temp_dbl_buff[i][2];
          index_max = i;
        }
        else if(m_temp_dbl_buff[i][2] < tmp_min){
          tmp_min = m_temp_dbl_buff[i][2];
          index_min = i;
        }
        else if(tmp_max -tmp_min >=8)
          break;
        else;
    }
    mid_x = m_temp_dbl_buff[index_max][0]+m_temp_dbl_buff[index_min][0];
    mid_x /= 2;
    mid_y = m_temp_dbl_buff[index_max][1]+m_temp_dbl_buff[index_min][1];
    mid_y /= 2;    
    m_wave_start_point.set_vertex(mid_x, mid_y);
    m_wave_start_point.set_label("start_point");
    m_wave_start_point.set_vertex_size(3);
    postMessage("VIEW_POINT", m_wave_start_point.get_spec());

    //find the end point
    tmp_max=-50, tmp_min=100;
    for(int i = m_temp_dbl_buff.size()/2 ; i< m_temp_dbl_buff.size();i++){
        if(m_temp_dbl_buff[i][2] > tmp_max){
          tmp_max = m_temp_dbl_buff[i][2];
          index_max = i;
        }
        else if(m_temp_dbl_buff[i][2] < tmp_min){
          tmp_min = m_temp_dbl_buff[i][2];
          index_min = i;
        }
        else if(tmp_max -tmp_min >=8)
          break;
        else;
    }
    mid_x = m_temp_dbl_buff[index_max][0]+m_temp_dbl_buff[index_min][0];
    mid_x /= 2;
    mid_y = m_temp_dbl_buff[index_max][1]+m_temp_dbl_buff[index_min][1];
    mid_y /= 2;    
    m_wave_end_point.set_vertex(mid_x, mid_y);
    m_wave_end_point.set_label("end_point");
    m_wave_end_point.set_vertex_size(3);
    postMessage("VIEW_POINT", m_wave_end_point.get_spec());
    
   
  }
  //Second round in rectangle
  else;
  */
  //-------------------------------------  


  
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_SearchFront_Tony::onRunState()
{
  // Part 1: Build the IvP function
  IvPFunction *ipf = 0;
  bool ok3, ok4;
  m_osx = getBufferDoubleVal("NAV_X", ok3);
  m_osy = getBufferDoubleVal("NAV_Y", ok4);
  if(!ok3 || !ok4) {
    postWMessage("No ownship X/Y info in info_buffer.");
    return(0);
  }

  bool ok , ok2;
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
        
        double temperature_dbl; 
        string temperature_str = tokStringParse(m_report, "temp", ',', '=');
        postMessage("TEMPERATURE_REPORT",temperature_str);
        stringstream ss;
        ss<<temperature_str;
        ss>>temperature_dbl;
        postMessage("TEMPERATURE_DBL",temperature_dbl);
        //X, Y, Temperature
        array<double, 3> data = {m_osx, m_osy, temperature_dbl};
        m_temp_dbl_buff.push_back(data);

        //V1 Wave doesn't move and follow the wave
        //------------------------------------
        /*
        m_temperature_now = temperature_dbl;
        */
        //------------------------------------

      }


  //V1 Wave doesn't move and follow the wave
  //------------------------------------
  /*
  if(m_temperature_record_update){
    m_temperature_record = m_temperature_now;
    m_temperature_record_update = false;
  }
  */
  //------------------------------------

  //V2 Wave moving
  //------------------------------------
  
  //------------------------------------


  double dist = sqrt((m_nextx-m_osx)*(m_nextx-m_osx) + (m_nexty-m_osy)*(m_nexty-m_osy) );
  if(dist <= m_arrival_radius) {
    giveNextPoint();
    return(0);
  }
  
  ipf = buildFunctionWithReflector();

  if(ipf)
    ipf->setPWT(m_priority_wt);

  return(ipf);
}
//------------------------------------------------------
//-----------------------------------------------------------
// Procedure: buildFunctionWithReflector

IvPFunction *BHV_SearchFront_Tony::buildFunctionWithReflector() 
{
  IvPFunction *ivp_function;

  bool ok = true;
  AOF_SimpleWaypoint aof_wpt(m_domain);
  ok = ok && aof_wpt.setParam("desired_speed", m_desire_speed);
  ok = ok && aof_wpt.setParam("osx", m_osx);
  ok = ok && aof_wpt.setParam("osy", m_osy);
  ok = ok && aof_wpt.setParam("ptx", m_nextx);
  ok = ok && aof_wpt.setParam("pty", m_nexty);
  ok = ok && aof_wpt.initialize();
  if(ok) {
    OF_Reflector reflector(&aof_wpt);
    reflector.create(600, 500);
    ivp_function = reflector.extractIvPFunction();
  }

  return(ivp_function);
}

