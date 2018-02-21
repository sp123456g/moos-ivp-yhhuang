/************************************************************/
/*    NAME: YH_Huang                                              */
/*    ORGN: MIT/NTU                                             */
/*    FILE: PrimeFactor.cpp                                        */
/*    DATE: FEB.21/2018                                                 */
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
        m_input_index     = 1;     //Initialize the index to be 1
        m_calculate_index = 1; 
        m_output_result   = "";    
        m_input_list.clear();
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
            input_node.mP_orin       = input_prime_str;   // Set up the member of node
            input_node.mP_orin_value = input_prime_num;
            input_node.mP_recei_ind = m_input_index;
            input_node.mP_prime_list.push_back(input_prime_num);
            m_input_list.push_back(input_node);   // put the node into the buffer
            m_input_index+=1;    //After put it in, index up 
            
    
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

void PrimeFactor::PrimeCalculate(Prime_Node &input){    //Using call by reference for the prime calculation

    Prime_Node out;
    double time1 = MOOSTime();     //Get first time
    double time2; 
    double total_time;
    bool done=true;                //Set up done=true here
    size_t prime_list_size=(input.mP_prime_list).size();     //Get the prime size now
    uint64_t i;                                              
    uint64_t Calculate_number;
    uint64_t temp_now = input.mP_temp;                    //Get the number calculated from now one
             i=temp_now+1;   //Get the prime calculating now if it is 1 start at 2, else start at the number+1
             Calculate_number = (input.mP_prime_list).back(); //Get the last number in the prime list.
             (input.mP_prime_list).pop_back();   
    
    while(i<=sqrt(Calculate_number)){      //Must smaller or equal sqrt(calculate_number), if number come with square number, equal will be used
        
                
        
        int re = Calculate_number%i;
            if(re==0){
                (input.mP_prime_list).push_back(i);    //If the residual equal zero, this is the prime factor
                Calculate_number /= i;                 // New calculate number become Calculate_number/i
                                
            }
            
              else{
                  i++;                                 //If residual is not zero, add i
                 if(i>=temp_now+1000000){              //If i is greater than 1000000,stop calculate
                  input.mP_temp = i;                   //Get the number now to the temp as a next calculation starting number
                  done = false;                        //set done to false
                   break;                               //Break the loop
                 }
              }
    }
             (input.mP_prime_list).push_back(Calculate_number);    //Add the last prime that don't have any else factor, if it's prime itself, push back it self


        input.mP_done = done;        //Set the bool to the member of prime node
        time2 = MOOSTime();          //Get the time finished
        total_time = time2-time1;    //Calculation of time
        input.mP_time += total_time; //Set to total member of prime node
}
//---------------------------------------------------------
// Procedure: OnConnectToServer

std::string PrimeFactor::Node_to_result(Prime_Node input){
//Change the node to output string result 
    string primelist="";
    vector<uint64_t> input_vector = input.mP_prime_list;
    for(int j=0;j<input_vector.size();j++)
         {
            if(j!=0)
                primelist += ":";    //Every prime is seperate by :
                
                std::stringstream a; 
                a<<input_vector[j];
                string prime = a.str();
     
                primelist += prime;    //Get the string of prime list
    }
                  
   string result  ="";     //formating the string
          result +="PRIME_RESULT=\"orig=";
          result +=input.mP_orin;
          result +=",received="; 
          result += double_to_str(input.mP_recei_ind);
          result += ",calculated=";
          result += double_to_str(input.mP_cal_ind);
          result += ",solve_time=";
          result += double_to_str(input.mP_time);               //Number must change to string
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
       
       if(!m_input_list.empty())    //If buffer is empty, just skip this loop
       {
           std::list<Prime_Node>::iterator p;        //pointer new to the Prime_Node
                     for(p=m_input_list.begin(); p!=m_input_list.end();) {   //check out the list, using pointer
           
                         PrimeCalculate(*p);    //calculate the prime_node which was point by p now 
           
                             if(p->mP_done){    //Change to result form, Notify and then remove from the list

                                     p->mP_cal_ind=m_calculate_index;     //finished so that add calculated index
                                     m_calculate_index+=1;              
                                     m_output_result = Node_to_result(*p); //Change the node to result string
                                     Notify("PRIME_RESULT",m_output_result); //Notify it
                                     m_input_list.erase(p++);               //delete it from the list and then p get to next. (Must write this format! Can't seperate p++ to the next line)
                             }
                             else
                                     p++;   //If not done, don't delete from the list, just point to next prime node to check, 
                    }

         
       }
     AppCastingMOOSApp::PostReport();

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
  m_msgs << "PRIME_RESULT:"    <<m_output_result<< "\n"; 
  m_msgs << "============================================ \n";
 // m_msgs << "Result:"          << m_output_result<<endl;
  return(true);
}




