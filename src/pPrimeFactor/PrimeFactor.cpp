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
        m_input_index     = 0;
        m_calculate_index = 0;
        m_input_string    = "";
        m_output_string   = "";
        
        m_prime_vector.clear();
        m_input_string_buffer.clear();
        m_not_done_buffer.clear();
        m_output_buffer.clear();
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
//     reportEvent("Get " + msg.GetString());
    
     //   cout<<m_input_string<<endl;
   //     m_input_uint64_t = StrToUint64_t(m_input_string);
        m_input_string_buffer.push_back(msg.GetString());
        m_input_index+=1;
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

     if(key == "FOO") 
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

vector<uint64_t> PrimeFactor::PrimeCalculate(uint64_t input){    //prime calculation   put the prime in the return vector,the first element is origin number,if calculation>50000000, the last value willbe putted in the last and calculate next time 
    
    vector<uint64_t> vector_prime;
    vector_prime.push_back(input); 
    uint64_t i    = 2;
    bool done = true;

    while(i<=sqrt(input)){
//        reportEvent("While loop is running!!!!!!!!!");
        int re = input%i;
            if(re==0){
                vector_prime.push_back(i);
                input /= i;
            }
              else{
                i++;
                 if(i>50000000000000){
                  done = false;
                   break;
                 }
              }
    }
     vector_prime.push_back(input);
        if(done)
            vector_prime.push_back(1);//The last element is bool of done or not
        else
            vector_prime.push_back(0);


        return vector_prime; //output is (orig|prime|done)
}
//---------------------------------------------------------
// Procedure: OnConnectToServer

void PrimeFactor::Done_or_not(vector<uint64_t> input_vector){

    if(input_vector.back()==1){
      
        string Notify_result = vector_to_result(input_vector);
        m_output_buffer.push_back(Notify_result);

        m_calculate_index +=1;
    }

    else if(input_vector.back()==0){
        
        m_not_done_buffer.push_back(input_vector);
    }
};

std::string PrimeFactor::vector_to_result(std::vector<uint64_t> input_vector){

    string primelist="";
    for(int j=1;j<input_vector.size()-1;j++)
         {
            if(j!=1)
                primelist += ":";
                
                std::stringstream a;
                a<<input_vector[j];
                string prime = a.str();
     
                primelist += prime;
    }
                  
         std::stringstream org;
         org<<input_vector[0];
         string orig = org.str();

   string result  ="";
          result +="PRIME_RESULT=\"orig=";
          result +=orig;
          result +=",received="; 
          result += int_to_str(m_input_index);
          result += ",calculated=";
          result += int_to_str(m_calculate_index);
          result += ",primes=";
          result += primelist;
          result += ",username = YH_HUANG";

          return result;
};

void PrimeFactor::re_calculate(std::vector<uint64_t> input){

    uint64_t  i    = 50000000000001;
    bool      done = true;
   
    uint64_t input_value=input[input.size()-2];
    
    input.pop_back();
    input.pop_back();
    //remove the done and the last value(because the last value may noy be prime)
    while(i<=sqrt(input_value)){
//        reportEvent("While loop is running!!!!!!!!!");
        int re = input_value%i;
            if(re==0){
                input.push_back(i);
                input_value /= i;
            }
              else{
                i++;
              }
    }
     input.push_back(input_value);
     input.push_back(1);
      
     string recalculate_result = vector_to_result(input);
     m_output_buffer.push_back(recalculate_result);
     m_calculate_index +=1;
     
     m_not_done_buffer.pop_front();
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
//reportEvent("Iterate!!!!!!!!!!!!!!!!!");

  AppCastingMOOSApp::PostReport();

  if(!m_input_string_buffer.empty()){
    
    m_input_string    =  m_input_string_buffer.front();
    m_input_string_buffer.pop_front();
    m_input_uint64_t  =  StrToUint64_t(m_input_string);
  
        m_prime_vector = PrimeCalculate(m_input_uint64_t);
        Done_or_not(m_prime_vector);
        m_prime_vector.clear();
  if(!m_input_string_buffer.empty()){
    m_input_string    =  m_input_string_buffer.front();
    m_input_string_buffer.pop_front();
    m_input_uint64_t  =  StrToUint64_t(m_input_string);
  
        m_prime_vector = PrimeCalculate(m_input_uint64_t);
        Done_or_not(m_prime_vector);
        m_prime_vector.clear();
  
    if(!m_not_done_buffer.empty())
      re_calculate(m_not_done_buffer.front());

  //output loop  
while(!m_output_buffer.empty()){
    
        m_output_string = m_output_buffer.front();
        m_output_buffer.pop_front();

            Notify("PRIME_RESULT",m_output_string);
            reportEvent("PRIME_RESULT: "+ m_output_string);
            
    //        cout<<"Deque size:"<<m_input_string_buffer.size()<<endl;
}
}
}
  return(true);
  
}

string PrimeFactor::int_to_str(int in){

    stringstream ss;
    ss<<in;
    string int_str = ss.str();

    return int_str;
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
    Register("PRIME_RESULT_VALID",0);
    AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool PrimeFactor::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "Input  :"         << m_input_string  << "\n";
  m_msgs << "Input_index:"     << m_input_index   << "\n";
  m_msgs << "PRIME_RESULT:"    << m_output_string << "\n"; 
  m_msgs << "============================================ \n";
 // m_msgs << "Result:"          << m_output_result<<endl;
  return(true);
}




