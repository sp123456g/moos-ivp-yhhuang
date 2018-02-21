/************************************************************/
/*    NAME: YH_Huang                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactor.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "PrimeFactor.h"

using namespace std;

//---------------------------------------------------------
// Constructor

PrimeFactor::PrimeFactor()
{
        m_input_index     = 1;
        m_calculate_index = 1;
}

//---------------------------------------------------------
// Destructor

PrimeFactor::~PrimeFactor()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PrimeFactor::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

      MOOSMSG_LIST::iterator p;
        for(p=NewMail.begin(); p!=NewMail.end(); p++) {
         CMOOSMsg &msg = *p;
         string key    = msg.GetKey();

    if(key=="NUM_VALUE"){
            string input_prime_str = msg.GetString();    // Get the string value
            uint64_t input_prime_num = StrToUint64_t(input_prime_str); // str to uint64_t

            Prime_Node input_node;     //delare the node here
            input_node.mP_orin      = input_prime_str;   // Set up the member of node
            input_node.mP_recei_ind = m_input_index;
            input_node.mP_temp      = 1;
            input_node.mP_prime_list.push_back(input_prime_num);
            input_node.mP_done      = false;
            input_node.mP_time      = 0; 
            m_input_list.push_back(input_node);   // put the node into the buffer
            m_input_index+=1;    //After put it in, index up 
            
    
//        m_input_string_buffer.push_back(msg.GetString());
//        m_input_index+=1;
       //        m_result = "";
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

     else if(key == "FOO") 
       cout << "great!";

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

uint64_t PrimeFactor::StrToUint64_t(string input){     //string to uint64_t

        stringstream stream(input);
        uint64_t result;
        stream>>result;

        return result;
}

void PrimeFactor::PrimeCalculate(Prime_Node &input){    

    double time1 = MOOSTime();
    double time2; 
    double total_time;
    bool done=true; 
    size_t prime_list_size=(input.mP_prime_list).size();
    uint64_t i;
    uint64_t Calculate_number;
    if(prime_list_size>=1){
             uint64_t temp_now = input.mP_temp;
             i=temp_now+1;   //Get the prime calculating now if it is 1 start at 2, else start at the number+1
             Calculate_number = (input.mP_prime_list).back();
             (input.mP_prime_list).pop_back(); 
    
    while(i<=sqrt(Calculate_number)){
        int re = Calculate_number%i;
            if(re==0){
                (input.mP_prime_list).push_back(i);
                Calculate_number /= i;
                                
            }
            
              else{
                  i++;                  
                 if(i>temp_now+10000){
                  input.mP_temp= i;
                  done = false;
                   break;
                 }
              }
    }
             (input.mP_prime_list).push_back(Calculate_number);



        input.mP_done = done; 
        time2 = MOOSTime();
        total_time = time2-time1;
        input.mP_time += total_time;
    }
}
//---------------------------------------------------------
// Procedure: OnConnectToServer

std::string PrimeFactor::Node_to_result(Prime_Node input){

    string primelist="";
    vector<uint64_t> input_vector = input.mP_prime_list;
    for(int j=0;j<input_vector.size();j++)
         {
            if(j!=0)
                primelist += ":";
                
                std::stringstream a;
                a<<input_vector[j];
                string prime = a.str();
     
                primelist += prime;
    }
                  
   string result  ="";
          result +="PRIME_RESULT=\"orig=";
          result +=input.mP_orin;
          result +=",received index="; 
          result += double_to_str(input.mP_recei_ind);
          result += ",calculated=";
          result += double_to_str(input.mP_cal_ind);
          result += ",solve_time=";
          result += double_to_str(input.mP_time);
          result += ",primes=";
          result += primelist;
          result += ",username = YH_HUANG";

          return result;
};

bool PrimeFactor::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PrimeFactor::Iterate()
{

    AppCastingMOOSApp::Iterate();
       // Do your thing here!
       //
       if(!m_input_list.empty())
       {
           std::list<Prime_Node>::iterator p;
        for(p=m_input_list.begin(); p!=m_input_list.end();) {   //check out the list, using pointer
         Prime_Node  pn;    //pn is the prime_node now is going to be calculated
           pn= *p; 
           
           PrimeCalculate(pn); 

           if(pn.mP_done){    //Change to result form, Notify and then remove from the list

              pn.mP_cal_ind=m_calculate_index;
              m_calculate_index+=1;
              string output_result;

              
              
              output_result = Node_to_result(pn);
              Notify("PRIME_RESULT",output_result);
              m_input_list.erase(p++);
           }
           else
               p++;
        }
           Notify("SIZE",m_input_list.size()); 

         
       }
     AppCastingMOOSApp::PostReport();

//  if(!m_input_string_buffer.empty()){  
   
// }
 
  return(true);
  
}

string PrimeFactor::double_to_str(double in){

    stringstream ss;
    ss<<in;
    string double_str = ss.str();

    return double_str;
}
//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PrimeFactor::OnStartUp()
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

void PrimeFactor::registerVariables()
{
    Register("NUM_VALUE",0);
   // Register("PRIME_RESULT_VALID",0);
    AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool PrimeFactor::buildReport() 
{
  m_msgs << "============================================ \n";
//  m_msgs << "Input  :"         <<m_output<< "\n";
  m_msgs << "Input_index:"     << "\n";
  m_msgs << "PRIME_RESULT:"    << "\n"; 
  m_msgs << "============================================ \n";
 // m_msgs << "Result:"          << m_output_result<<endl;
  return(true);
}




