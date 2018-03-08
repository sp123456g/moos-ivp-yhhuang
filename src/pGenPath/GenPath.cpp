/************************************************************/
/*    NAME: YHHUANG                                         */
/*    ORGN: MIT/NTU                                             */
/*    FILE: GenPath.cpp                                        */
/*    DATE: MAR.8/2018                                                */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "GenPath.h"
using namespace std;

//---------------------------------------------------------
// Constructor

GenPath::GenPath()
{
    m_input_buffer.clear();
    m_xyid_buff.clear();
    m_out_node.clear();
    sub_x1 = "";
    sub_y1 ="";
    m_update_str = "";
    m_id="";
    x=0;
    y=0;
    id=0;
    m_calculated_index = 0;
    m_input_index = 0;
    m_input_mail_or_not = true;
}

//---------------------------------------------------------
// Destructor

GenPath::~GenPath()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool GenPath::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);
  
    if(m_input_mail_or_not) 
    m_OnNewMail_index += 1;
    
  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();
    

    if(key == "VISIT_POINT"){      //Has Brige:visit_point_gilda,if not:+gilda or henry
        


        string visit_point = msg.GetString();
      m_input_buffer.push_back(visit_point);
      m_input_index+=1;
     
 //     reportEvent("VISIT_POINT="+visit_point); 
    }



#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif


     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool GenPath::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second
//
std::deque<Node> GenPath::Calculate_min(deque<Node> input){    //TSP algorithm

    double x_now = 0;
    double y_now = 0;
    double dis_min;
    deque<Node> output_buffer;
    Node near_point;    

        while(!input.empty()){
           
            dis_min = 3000; //initialize the min distance (a number which is big enough, Here I choose 3000)
            
            for(int i=0;i<input.size();i++){
            
                double dis_now = sqrt(pow((input[i].xyid[0]-x_now),2)+pow(input[i].xyid[1],2));
                //The distance between present point and all the others
                    if(dis_now < dis_min){
                        dis_min = dis_now;
                        
                        near_point = input[i];          // near_point : x|y|index of the node
                        near_point.xyid[2] = i;
                    }
            }
             output_buffer.push_back(near_point);    //Put in output buffer
             x_now = near_point.xyid[0];             //Reset the position (x,y) now
             y_now = near_point.xyid[1];             

                int erase_index = near_point.xyid[2];   //After throw in buffer, delete that node from the input_buffer
                input.erase(input.begin()+erase_index);   
        }
        
        return output_buffer;   //output the sorted deque
}

bool GenPath::Iterate()
{
  AppCastingMOOSApp::Iterate();
   
  if(m_input_buffer.size()!=0){

      m_calculated_index+=1;    
        string input = m_input_buffer.front();
          
          if(input=="firstpoint")  //pop the firstpoint out, or it will stay there all the time
               m_input_buffer.pop_front();
          
        m_found_id = input.find("id=");   //find id 
                m_id = input.substr(m_found_id+3);
                
        m_found_x  = input.find("x=");   //find x=
        m_found_c1 = input.find(",");    //find first,
        if(m_found_x !=string::npos && m_found_c1 != string::npos){ 
             sub_x1 = input.substr(m_found_x+2,m_found_c1-m_found_x-2); //string of x
        m_found_y  = input.find("y=");  
        
         if(m_found_y !=string::npos){       
             string sub_no_x = input.substr(m_found_y);  // get rid of x
                m_found_c2 = sub_no_x.find(",");    //find second , 
                    if(m_found_c2 != string::npos){ 
                        sub_y1 = sub_no_x.substr(2,m_found_c2-2); //string of y

                    }
         }
        }

  //string to double
    stringstream x_ss, y_ss, id_ss;
    x_ss<<sub_x1;
    x_ss>>x;
    
    y_ss<<sub_y1;
    y_ss>>y;

    id_ss<<m_id;
    id_ss>>id;
                node.xyid[0] = x;
                node.xyid[1] = y;
                node.xyid[2] = id;    //make the Node x|y|id

           m_xyid_buff.push_back(node);   //make a deque of node
           m_input_buffer.pop_front();   //delete from input if it has push back to new buffer 

  
  if(input=="lastpoint"){
  //m_input_mail_or_not = false;

  m_out_node = Calculate_min(m_xyid_buff);  //Get the sorted deque of x|y|id
  while(!m_out_node.empty()){    //Get the element of sorted deque
  
    
    Node xyid_node = m_out_node.front();
    my_seglist.add_vertex(xyid_node.xyid[0],xyid_node.xyid[1]);   //xyid[0] is x, xyid[1] is y
    m_update_str = "points = ";
    m_update_str   += my_seglist.get_spec();
    m_out_node.pop_front();
   }
           //to the form of seglist and Notift to waypoint update
      
        Notify("UPDATES_VAR", m_update_str);  // UPDATES_VAR depends on your config
  
  }
  }
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool GenPath::OnStartUp()
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
    string param = toupper(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "FOO") {
      handled = true;
    }
    else if(param == "BAR") {
      handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void GenPath::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
    Register("VISIT_POINT",0);
  // Register("FOOBAR", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool GenPath::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "PGenPath                                     \n";
  m_msgs << "============================================ \n";
 // m_msgs << "Buffer Size:"<<m_input_buffer.size()<<"\n";
 // m_msgs << "X="<<sub_x1<<"\n";
 // m_msgs << "Y="<<sub_y1<<"\n";
 // m_msgs << "m_id="<<m_id<<"\n";
  m_msgs << "m_input_index="<<m_input_index<<"\n";
  m_msgs << "m_calculated_index="<<m_calculated_index<<"\n";
  return(true);
}




