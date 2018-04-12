/************************************************************/
/*    NAME: yhhuang                                              */
/*    ORGN: MIT                                             */
/*    FILE: HazardPath.cpp                                        */
/*    DATE: Apr.10/2018                                                */
/************************************************************/

#include <iterator>
#include <stdlib.h>
#include <sstream>
#include "MBUtils.h"
#include "HazardPath.h"
#include "ACTable.h"
using namespace std;

//---------------------------------------------------------
// Constructor

HazardPath::HazardPath()
{
  //m_xypoint_list.clear();
  m_history_detect_size = -1;
  m_middle_y_offset = 0.0;
  m_second_lane_width = 20.0;
}

//---------------------------------------------------------
// Destructor

HazardPath::~HazardPath()
{
    m_lane_width = 30;
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool HazardPath::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
    string sval = msg.GetString();
    double dval  = msg.GetDouble();
     if(key == "FOO") 
       cout << "great!";
     else if(key == "UHZ_MISSION_PARAMS")
      handleMailMissionParams(sval);
     else if (key == "HISTORY_DETEC_LIST")
      handleMailDetectList(sval);
     else if (key == "NAV_X")
      m_nav_x = dval;
     else if (key == "NAV_Y")
      m_nav_y = dval;
     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}


//---------------------------------------------------------
// Procedure: handleMailMissionParams
//   Example: UHZ_MISSION_PARAMS = penalty_missed_hazard=100,               
//                       penalty_nonopt_hazard=55,                
//                       penalty_false_alarm=35,                  
//                       penalty_max_time_over=200,               
//                       penalty_max_time_rate=0.45,              
//                       transit_path_width=25,                           
//                       search_region = pts={-150,-75:-150,-50:40,-50:40,-75}
//    goal: get the operating region and use Calculateregion function to handle it

void HazardPath::handleMailMissionParams(string str)
{
  vector<string> svector = parseStringZ(str, ',', "{");
  unsigned int i, vsize = svector.size();
  for(i=0; i<vsize; i++) {
    string param = biteStringX(svector[i], '=');
    string value = svector[i];
    // This needs to be handled by the developer. Just a placeholder.
  }

    m_path = svector[7];    //m_path: pts={.......}
    
    size_t found_bra_one, found_bra_two;
           found_bra_one = m_path.find("{");
           found_bra_two = m_path.find("}");
    string region = m_path.substr(found_bra_one+1,found_bra_two-found_bra_one-1);
   m_region = region;

   CalculateRegion(region, m_lane_width, 0.0);

    Notify("WA_UPDATE_ONE",m_s_path_one);
    Notify("WA_UPDATE_TWO",m_s_path_two);
}

void HazardPath::handleMailDetectList(string str)
{
  vector<string> contenor = parseString(str, '#');  // split by ,
  m_history_detect_size = contenor.size();
  for(int i = 0; i < contenor.size(); i++) {
    m_history_detect_buff.push_back(contenor[i]);
  }
  string2XYPoint(m_history_detect_buff);
  stringstream ss;
  ss << m_history_detect_size;
  reportEvent(ss.str());
  if(m_history_detect_size > 10) {
    CalculateRegion(m_region, m_second_lane_width, m_middle_y_offset); 
    Notify("REDETECT_MODE", "s_path");
    Notify("UPDATES_REDECT_PATH_ONE", m_s_path_one);
    Notify("UPDATES_REDECT_PATH_TWO", m_s_path_two);
  }
  else {
    greedy_path(m_nav_x, m_nav_y);
    Notify("REDETECT_MODE", "greedy_path");
    Notify("UPDATES_REDECT_PATH", XYPoint2string());
    //Notify("UPDATES_REDECT_PATH_TWO", XYPoint2string());
  }
}

//---------------------------------------------------------
// Procedure: OnConnectToServer
bool HazardPath::OnConnectToServer()
{
   registerVariables();
   return(true);
}
//---------------------------------------------------------
//CalculateRegion: Change region info to way-point update variable
//1. Calculate height and width
//2. Seperate the area to left and right
//3. find the mid point of two area
void HazardPath::CalculateRegion(string region, double lane_width, double y_middle_offset)
{

vector<string> range_vector = parseString(region, ':');
//for range index:
//0  3

//1  2
    double height;
    double width;
    //double lane_width = m_lane_width;
    double middle_pnt1_x;
    double middle_pnt2_x;
    double middle_pnt1_y;
    double middle_pnt2_y;
    double middle_offset=10;
    
    vector<double> x_pnts;
    vector<double> y_pnts;
   
    if(!range_vector.empty()){
     for(int i=0;i<range_vector.size();i++){  
      stringstream ss_x,ss_y;
      string position = range_vector[i];
      double x,y;

        ss_x<<biteStringX(position,',');
        ss_y<<position;

        ss_x>>x;
        ss_y>>y;

        x_pnts.push_back(x);
        y_pnts.push_back(y);
    }


    height  = abs(y_pnts[0]-y_pnts[1]);
    width   = abs((x_pnts[1]-x_pnts[2])/2);  
    
    middle_pnt1_x = x_pnts[1]+(width/2)-middle_offset;
    middle_pnt2_x = x_pnts[2]-(width/2)+middle_offset;

    //middle_pnt1_y = (y_pnts[0]-height/2)+middle_offset;
    middle_pnt1_y = (y_pnts[0]-height/2) + y_middle_offset;
    middle_pnt2_y = (y_pnts[0]-height/2) + y_middle_offset; 

    // Output for appcasting    
    stringstream ss_one,ss_two;
    ss_one<<height;
    ss_two<<width;
    ss_one>>m_height;
    ss_two>>m_width;
     
//Generate update variable for wapoint behavior
//points = format=lawnmower,label=jakesearch,x=-15,y=-237.5,height=325,width=240,lane_width=30,rows=east-west,startx=0,starty=0
//WA_UPDATE_ONE for VNAME1, WA_UPDATE_TWO for VNAME2

        string str_output_one;
        string str_output_two;
        stringstream ss_output_one;
        stringstream ss_output_two;

        ss_output_one<<"points=format=lawnmower,label=west_region,x=";
        ss_output_one<<middle_pnt1_x<<",y="<<middle_pnt1_y;
        ss_output_one<<",height="<<height<<",width="<<width;
        ss_output_one<<",lane_width="<<lane_width;
        ss_output_one<<",rows=east-west,startx=0,starty=0";
        
        ss_output_one>>str_output_one;
        
        ss_output_two<<"points=format=lawnmower,label=west_region,x=";
        ss_output_two<<middle_pnt2_x<<",y="<<middle_pnt2_y;
        ss_output_two<<",height="<<height<<",width="<<width;
        ss_output_two<<",lane_width="<<lane_width;
        ss_output_two<<",rows=east-west,startx=0,starty=0";

        ss_output_two>>str_output_two;

        //for debug if needed 
        //  m_output_one = str_output_one;
        //  m_output_two = str_output_two;

        m_s_path_one = str_output_one;
        m_s_path_two = str_output_two;
            // Notify("WA_UPDATE_ONE",str_output_one);
            // Notify("WA_UPDATE_TWO",str_output_two);

    } 
}


// -------------- string2XYPoint -----------------
// string to XYPoint
// msg from string_list
//          example: x=14,y=3,label=35 | x=-34,y=-34,label=46
// XYPoint is save in class value m_xypoint_list
void HazardPath::string2XYPoint(vector<string> string_list)
{
  vector<string>::iterator it;
  if(!string_list.empty()) {
    for(it=string_list.begin(); it!=string_list.end(); ++it)
    {
      string &visit_point = *it;
      vector<string> contenor = parseString(visit_point, ',');  // split by ,
      double x, y;
      int param_cnt = 0;
      for(int i = 0; i < contenor.size(); i++) {
        string param = biteStringX(contenor[i], '=');
        string value = contenor[i];
        param = tolower(param);
        if(param == "x") {
          setDoubleOnString(x, value);
          param_cnt++;
        }
        else if (param == "y") {
          setDoubleOnString(y, value);
          param_cnt++;
        }
        else if (param == "label") {
          param_cnt++;
          break;
        }
      }
      if(param_cnt == 3) {
        XYPoint new_point(x,y);
        m_xypoint_list.push_back(new_point);
      }
    }
  }
}

// --------------- greedy_path ----------------
// 0. Set a starting point as previous point
// 1. Calculate distance between previous point and other point
// 2. Get the point that has mininum distance between previous point and set it as next point
// 3. set next point as previous point
// 4. go back to step 1.
void HazardPath::greedy_path(double start_x, double start_y)
{
  XYPoint point_pre(start_x, start_y);
  vector<XYPoint> path_list;
  while(!m_xypoint_list.empty()){
    vector<XYPoint>::iterator p;
    vector<XYPoint>::iterator p_next;
    double min_dis = 9999;
    // find local mininum
    for(p=m_xypoint_list.begin(); p!=m_xypoint_list.end(); ++p) {
      XYPoint &point_now = *p;
      double d = (pow((point_now.get_vx()-point_pre.get_vx()),2) + pow((point_now.get_vy()-point_pre.get_vy()),2));
      d = sqrt(d);
      if(d < min_dis) {
        min_dis = d;
        p_next = p;
      }
    }
    XYPoint &point_next = *p_next;
    point_next.set_vx(point_next.get_vx());
    point_next.set_vy(point_next.get_vy());
    path_list.push_back(*p_next);
    point_pre = *p_next;
    m_xypoint_list.erase(p_next);
  }
  m_xypoint_list = path_list;
}

void HazardPath::i_path (double start_x, double start_y)
{
  XYPoint point_pre(start_x, start_y);
  vector<XYPoint> path_list;
  while(!m_xypoint_list.empty()){
    vector<XYPoint>::iterator p;
    vector<XYPoint>::iterator p_next;
    double min_dis = 9999;
    // find local mininum
    for(p=m_xypoint_list.begin(); p!=m_xypoint_list.end(); ++p) {
      XYPoint &point_now = *p;
      double d = (pow((point_now.get_vx()-point_pre.get_vx()),2) + pow((point_now.get_vy()-point_pre.get_vy()),2));
      d = sqrt(d);
      if(d < min_dis) {
        min_dis = d;
        p_next = p;
      }
    }
    XYPoint &point_next = *p_next;
    point_pre = *p_next;
    point_next.set_vx(point_next.get_vx()+20);
    point_next.set_vy(point_next.get_vy());
    path_list.push_back(*p_next);
    point_next.set_vx(point_next.get_vx()-40);
    point_next.set_vy(point_next.get_vy());
    path_list.push_back(*p_next);
    m_xypoint_list.erase(p_next);
  }
  m_xypoint_list = path_list;
}

// --------------- XYPoint2string ----------------
// XYPoint to string
// msg from XYPoint vector
// return strin msg
//          example: points = 14,3:34,-34:15,20:30,6
string HazardPath::XYPoint2string()
{
  stringstream ss;
  ss << "points = ";
  vector<XYPoint>::iterator iter;
  for (iter=m_xypoint_list.begin(); iter!=m_xypoint_list.end(); ++iter) {
    XYPoint &point_msg = *iter;
    if(iter != m_xypoint_list.begin())
      ss << ":";
    ss << point_msg.get_vx() << "," << point_msg.get_vy();
  }
  return ss.str();
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second
bool HazardPath::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool HazardPath::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = tolower(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "foo") {
      handled = true;
    }
    else if(param == "bar") {
      handled = true;
    }
    else if(param == "lane_width"){
        stringstream ss;
        ss<<value;
        ss>>m_lane_width;
        handled = true;
    
    }
    else if(param == "second_lane_width"){
        stringstream ss;
        ss<<value;
        ss>>m_second_lane_width;
        handled = true;
    
    }
    else if(param == "y_offset"){
        stringstream ss;
        ss<<value;
        ss>>m_middle_y_offset;
        handled = true;
    
    }
  //  else if(param == "opregion"){
    //    CalculateRegion(value);
    //    m_region = value; 
    //    handled  = true;
  //  }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void HazardPath::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  Register("UHZ_MISSION_PARAMS",0);
  //Register("SURVEY",0);
  Register("HISTORY_DETEC_LIST",0);
  Register("NAV_X",0);
  Register("NAV_Y",0);
     
}


//------------------------------------------------------------
// Procedure: buildReport()

bool HazardPath::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "File:                                        \n";
  m_msgs << "============================================ \n";

  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  m_msgs << actab.getFormattedString();
  m_msgs <<endl<< "Searching Height:"<<m_height<<endl;
  m_msgs << "Searching Width:" <<m_width<<endl;
  m_msgs << "Searching Region:"<<m_region<<endl;
//  m_msgs << "PATH:"<<m_path<<endl;
//  m_msgs << "m_path_to_region"<<m_path_to_region<<endl;
//  m_msgs <<"WA_UPDATE_ONE:"<<m_output_one<<endl;  
//  m_msgs << "WA_UPDATE_TWO:"<<m_output_two<<endl;
  return(true);
}




