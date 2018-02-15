/************************************************************/
/*    NAME: YH_Huang                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactorTester.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "PrimeFactorTester.h"
using namespace std;

//---------------------------------------------------------
// Constructor

PrimeFactorTester::PrimeFactorTester()
{
    m_input_str = "";
    m_result      = 1;
    m_correct     = true;
    m_input_value = 0;
    m_output_str  = "";
}

//---------------------------------------------------------
// Destructor

PrimeFactorTester::~PrimeFactorTester()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PrimeFactorTester::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    
        string key = msg.GetKey(); 

        if(key=="PRIME_RESULT"){
            m_input_str = msg.GetString();
        }
#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PrimeFactorTester::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PrimeFactorTester::Iterate()
{
        //example input: orig=100,received=16,calculated=15,primes=2:2:5:5,username = YH_HUANG   
    size_t found = m_input_str.find("orig");
    if(found==string::npos)
     m_correct = false;
    
    else{
            size_t found2 = m_input_str.find(",");
            string input_num_str = m_input_str.substr(found+5,found2-(found+5));
           stringstream ss(input_num_str);
            ss>>m_input_value;

            size_t found3 = m_input_str.find("primes=");
            string PrimeToEnd = m_input_str.substr(found3+7);
            size_t found4 = PrimeToEnd.find(",");
                
            string PrimeList = PrimeToEnd.substr(0,found4);
            size_t found_colon;
            while(found_colon!=string::npos){
                found_colon = PrimeList.find(":"); 
                string prime_str = PrimeList.substr(0,found_colon);
  //              cout<<"prime:"<<prime_str<<endl;
                uint64_t prime=0;

                stringstream prime_ss(prime_str);
                prime_ss>>prime;

                m_input_prime.push_back(prime);
                
                PrimeList = PrimeList.substr(found_colon+1);
            }

            
            for(int i=0;i<m_input_prime.size();i++){
              //  cout<<m_input_prime.size()<<endl;
                m_result *=m_input_prime[i];
            }
            if(m_result==m_input_value)
                m_correct = true;
            else
                m_correct = false;
            
            
    }
        m_output_str = m_input_str;
        m_output_str += ",valid=";
        if(m_correct){
            m_output_str +="true";
        
        }
        else
            m_output_str +="false";     
            
     //   cout<<"m_result="<<m_result<<"\n"<<"m_input_value="<<m_input_value<<"\n";
          
        Notify("PRIME_RESULT_VALID",m_output_str);
        //reportEvent("PRIME_RESULT_VALID="+m_output_str);
        m_result = 1;
        m_input_prime.clear();
    //    std::cout<<"iterate"<<std::endl;
   //     cout<<m_correct<<endl;
        return(true);
}


//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PrimeFactorTester::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      
      if(param == "FOO") {
        //handled
      }
      else if(param == "BAR") {
        //handled
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void PrimeFactorTester::RegisterVariables()
{
    Register("PRIME_RESULT",0);
  // Register("FOOBAR", 0);
}

