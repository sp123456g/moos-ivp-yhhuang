/************************************************************/
/*    NAME: yhh                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_SearchFront.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include "MBUtils.h"
#include "BuildUtils.h"
#include "BHV_SearchFront.h"
//#include <armadillo>
using namespace std;
//using namespace arma;
//---------------------------------------------------------------
// Constructor

BHV_SearchFront::BHV_SearchFront(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "defaultname");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "course,speed");

  // Add any variables this behavior needs to subscribe for
  addInfoVars("NAV_X, NAV_Y, UCTD_MSMNT_REPORT, DESIRED_HEADING","no_warning");
    
  m_report = "";
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_SearchFront::setParam(string param, string val)
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

void BHV_SearchFront::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_SearchFront::onHelmStart()
{
    m_time0 = getBufferCurrTime(); 
        postMessage("TIME_ZERO",m_time0);
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_SearchFront::onIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_SearchFront::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_SearchFront::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_SearchFront::onIdleToRunState()
{
    
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_SearchFront::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_SearchFront::onRunState()
{
  // Part 1: Build the IvP function
  IvPFunction *ipf = 0;
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
        m_temp_dbl_buff.push_back(temperature_dbl);
      }
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
IvPFunction* BHV_SearchFront::Gen_Zaic_peak(double heading){
    //step1 Generate domain, but there is a m_domain can use
    //step2 build zaic_peak tool ,course is heading
    ZAIC_PEAK crs_zaic(m_domain,"course");
    //step3 set up the zaic_peak
    crs_zaic.setSummit(heading);  //current heading+angle you want, because the heading here is the absolute value
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
