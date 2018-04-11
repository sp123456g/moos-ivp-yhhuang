/************************************************************/
/*    NAME: yhhuang                                              */
/*    ORGN: MIT                                             */
/*    FILE: HazardPath.cpp                                        */
/*    DATE: Apr.10/2018                                                */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "HazardPath.h"
#include <sstream>
using namespace std;

//---------------------------------------------------------
// Constructor

HazardPath::HazardPath()
{
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
     if(key == "FOO") 
       cout << "great!";
     else if(key == "UHZ_MISSION_PARAMS")
      handleMailMissionParams(sval);
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

   CalculateRegion(region); 
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
void HazardPath::CalculateRegion(string region){

vector<string> range_vector = parseString(region, ':');
//for range index:
//0  3

//1  2
    double height;
    double width;
    double    lane_width = m_lane_width;
    double middle_pnt1_x;
    double middle_pnt2_x;
    double middle_pnt1_y;
    double middle_pnt2_y;

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
    width   = abs((x_pnts[1]-x_pnts[2])/2);  //set 50 to let two vehicle not be too close
    
    middle_pnt1_x = x_pnts[1]+(width/2);
    middle_pnt2_x = middle_pnt1_x + width;

    middle_pnt1_y = y_pnts[0]-height/2;
    middle_pnt2_y = middle_pnt1_y; 

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
        ss_output_one<<",height="<<height<<",width="<<width-50;
        ss_output_one<<",lane_width="<<lane_width;
        ss_output_one<<",rows=east-west,startx=0,starty=0";
        
        ss_output_one>>str_output_one;
        
        ss_output_two<<"points=format=lawnmower,label=west_region,x=";
        ss_output_two<<middle_pnt2_x<<",y="<<middle_pnt2_y;
        ss_output_two<<",height="<<height<<",width="<<width-50;
        ss_output_two<<",lane_width="<<lane_width;
        ss_output_two<<",rows=east-west,startx=0,starty=0";

        ss_output_two>>str_output_two;

        //for debug if needed 
        //  m_output_one = str_output_one;
        //  m_output_two = str_output_two;

            Notify("WA_UPDATE_ONE",str_output_one);
            Notify("WA_UPDATE_TWO",str_output_two); 

    } 
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




