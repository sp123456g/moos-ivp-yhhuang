/************************************************************/
/*    NAME: YHHuang                                              */
/*    ORGN: MIT                                             */
/*    FILE: AisNode.cpp                                          */
/*    DATE: MAR 19th 2018                            */
/************************************************************/


#include "AisNode.h"
#include "MBUtils.h"
#include <vector>
#include <sstream>
#include <iomanip>
AisNode::AisNode()
{
    m_original_inform   = "";
    m_packet_type       = "";
    m_number_of_sen     = "";
    m_sen_num           = "";
    m_seq_mess_id       = "";
    m_channel           = "";
    m_main_message      = "";
    
    m_parse_information.clear();
    m_main_bin_message.clear();

    m_user_id           = "";
    m_navigation_status = "";
    
    m_sog = 0;
    m_cog = 0;
    m_lon = 0;
    m_lat = 0;
    m_true_heading = 0;

    m_wrong = false;
     
};

AisNode::~AisNode()
{

};

void AisNode::setOrin(std::string input)
{

    m_original_inform = input;
};

void AisNode::analysis()
{
    if(m_original_inform.find("!AI")!=std::string::npos){
    m_parse_information = parseString(m_original_inform, ',');
//AIS DATA is seperated by ','
     m_packet_type   = m_parse_information[0]; 
     m_number_of_sen = m_parse_information[1]; 
     m_sen_num       = m_parse_information[2]; 
     m_seq_mess_id   = m_parse_information[3]; 
     m_channel       = m_parse_information[4]; 
     m_main_message  = m_parse_information[5]; 

     m_message_id    = m_main_message[0];
        
    std::vector<std::string> binary_str_buff;
 
    for(int i=0;i<m_main_message.length();i++){


      std::string ascii_str = CharToAscii(m_main_message[i]);
        
        if(ascii_str != "") 
        binary_str_buff.push_back(ascii_str);
    }

      m_main_bin_message = AsciiToBinVec(binary_str_buff);
    }
};

std::string AisNode::getPacketType() const            
{
    return(m_packet_type);

};

std::string AisNode::getNumberOfSen() const         
{
    return(m_number_of_sen);
};

std::string AisNode::getSenNum() const              
{
    return(m_sen_num);
};

std::string AisNode::getSeqMessID() const          
{
    return(m_seq_mess_id);
};

std::string AisNode::getChannel() const              
{
    return(m_channel);
};

std::string AisNode::getMainMessage() const
{
    return(m_main_message);
};

std::string AisNode::getMessageID() const
{
    return(m_message_id);
};

std::string AisNode::getUserID()
{
    std::deque<int> binary;

    for(int i=8;i<=37;i++){
  //index 8 to 37 is for UserID 
       binary.push_front(m_main_bin_message[i]);
    }

    long int decimal = UnsignBinToDec(binary);   

    std::stringstream ss;
        ss<<decimal;
        ss>>m_user_id; 
        return(m_user_id);
};

double AisNode::getSog()  //get speed over ground
{
    std::deque<int> binary;

    for(int i=50;i<=59;i++){
//index 50 to 59 is for sog 
       binary.push_front(m_main_bin_message[i]);
    }

    double decimal = UnsignBinToDec(binary);   
           m_sog = decimal/10;    // result is 10 times of real speed
      
//Check if it's in range
    if(decimal<0 || decimal>1022){
        m_wrong = true;
    }
        
    return(m_sog);
};

double AisNode::getCog()  //get course of ground
{
    std::deque<int> binary;

    for(int i=116;i<=127;i++){
//index 116 to 127 is for Cog 
       binary.push_front(m_main_bin_message[i]);
    }

    double decimal = UnsignBinToDec(binary);   
           m_cog = decimal/10;    //result is 10 times of real spedd

//check if it's in range 
    if(decimal<0 || decimal>3600){
        m_wrong = true;

    }

    

    return(m_cog);
};

double AisNode::getLon()
{
    std::deque<int> binary;

    for(int i=61;i<=88;i++){
//index 61 to 88 is for Lon 
       binary.push_front(m_main_bin_message[i]);
    }

    double decimal = BinToDec(binary);   
           m_lon = decimal/600000;    //output is 1/10000 min, change to degree
//check if it's in range
    if(abs(m_lon)>180){
        m_wrong = true;
    }

    return(m_lon);
    
};

double AisNode::getLat()
{
    std::deque<int> binary;

    for(int i=89;i<=115;i++){
  //index 89 to 115 is for Lat
       binary.push_front(m_main_bin_message[i]);
    }

    double decimal = BinToDec(binary);   
           m_lat = decimal/600000;    //output is 1/10000 min, change to degree
    
    if(abs(m_lat)>90){
        m_wrong = true;
    }



    return(m_lat);


};

double AisNode::getTrueHeading()
{
    std::deque<int> binary;

    for(int i=128;i<=136;i++){
  //index 128 to 136 is for true heading 
       binary.push_front(m_main_bin_message[i]);
    }

    double decimal = UnsignBinToDec(binary);   
    m_true_heading = decimal;    
  
    if(m_true_heading<0 || m_true_heading>360 && m_true_heading!=511.0){
        m_wrong = true;
    }



    return(m_true_heading);


};

std::string AisNode::CharToAscii(char input)
{
    std::string output="";
    if(input=='0')
        output = "000000";
    else if(input=='1')
        output = "000001";
    else if(input=='2')
        output = "000010";
    else if(input=='3')
        output = "000011";
    else if(input=='4')
        output = "000100";
    else if(input=='5')
        output = "000101";
    else if(input=='6')
        output = "000110";
    else if(input=='7')
        output = "000111";
    else if(input=='8')
        output = "001000";
    else if(input=='9')
        output = "001001";
    else if(input==':')
        output = "001010";
    else if(input==';')
        output = "001011";
    else if(input=='<')
        output = "001100";
    else if(input=='=')
        output = "001101";
    else if(input=='>')
        output = "001110";
    else if(input=='?')
        output = "001111";
    else if(input=='@')
        output = "010000";
    else if(input=='A')
        output = "010001";
    else if(input=='B')
        output = "010010";
    else if(input=='C')
        output = "010011";
    else if(input=='D')
        output = "010100";
    else if(input=='E')
        output = "010101";
    else if(input=='F')
        output = "010110";
    else if(input=='G')
        output = "010111";
    else if(input=='H')
        output = "011000";
    else if(input=='I')
        output = "011001";
    else if(input=='J')
        output = "011010";
    else if(input=='K')
        output = "011011";
    else if(input=='L')
        output = "011100";
    else if(input=='M')
        output = "011101";
    else if(input=='N')
        output = "011110";
    else if(input=='O')
        output = "011111";
    else if(input=='P')
        output = "100000";
    else if(input=='Q')
        output = "100001";
    else if(input=='R')
        output = "100010";
    else if(input=='S')
        output = "100011";
    else if(input=='T')
        output = "100100";
    else if(input=='U')
        output = "100101";
    else if(input=='V')
        output = "100110";
    else if(input=='W')
        output = "100111";
    else if(input=='`')
        output = "101000";
    else if(input=='a')
        output = "101001";
    else if(input=='b')
        output = "101010";
    else if(input=='c')
        output = "101011";
    else if(input=='d')
        output = "101100";
    else if(input=='e')
        output = "101101";
    else if(input=='f')
        output = "101110";
    else if(input=='g')
        output = "101111";
    else if(input=='h')
        output = "110000";
    else if(input=='i')
        output = "110001";
    else if(input=='j')
        output = "110010";
    else if(input=='k')
        output = "110011";
    else if(input=='l')
        output = "110100";
    else if(input=='m')
        output = "110101";
    else if(input=='n')
        output = "110110";
    else if(input=='o')
        output = "110111";
    else if(input=='p')
        output = "111000";
    else if(input=='q')
        output = "111001";
    else if(input=='r')
        output = "111010";
    else if(input=='s')
        output = "111011";
    else if(input=='t')
        output = "111100";
    else if(input=='u')
        output = "111101";
    else if(input=='v')
        output = "111110";
    else if(input=='w')
        output = "111111";

    return output;
};


std::vector<int> AisNode::AsciiToBinVec(std::vector<std::string> input)
{
    std::vector<int> output;
    
   for(int i=0;i<input.size();i++){
    
       for(int j=0;j<input[0].length();j++){
           
           std::stringstream ss;
           std::string int_str;
           int number;
           
           int_str = input[i].substr(j,1);
           
           ss<<int_str;
           ss>>number;
           output.push_back(number);  
       }
   }
        return(output);
};

double AisNode::BinToDec(std::deque<int> input)  // ex: 11110 input[0]=0 input[1]=1...
{
    int64_t sum=0;

    if(input[input.size()-1]==1){
        //negative, largest number become to nagative
         for(int i=0;i<input.size()-1;i++){
                sum+=input[i]*pow(2,i);
         }
         sum+=-1*pow(2,input.size()-1);
    }
    else{ 
        for(int i=0;i<input.size();i++){
                 sum+=input[i]*pow(2,i);
        }
    }
        return(sum);    
};

double AisNode::UnsignBinToDec(std::deque<int> input)  // ex: 11110 input[0]=0 input[1]=1...
{
    int64_t sum=0;

        for(int i=0;i<input.size();i++){
                 sum+=input[i]*pow(2,i);
        }
        return(sum);    
};

std::string AisNode::getReport()
{
    std::stringstream ss;
    std::string       output;
   
    getUserID();
    getLon();
    getLat();
    getCog();
    getSog();
    getTrueHeading(); 
    ss<<"ShipID="<<m_user_id<<",Lon="<<std::setprecision(10)<<m_lon<<",Lat="<<std::setprecision(10)<<m_lat<<",Cog="<<m_cog<<",TrueHeading="<<m_true_heading<<",Sog="<<m_sog;

    ss>>output;

    if(m_wrong)
        output = "Wrong_Message_Type";

    return(output);
};
