/************************************************************/
/*    NAME: YHHuang                                              */
/*    ORGN: MIT                                             */
/*    FILE: AISdecoder.h                                          */
/*    DATE: MAR 19th 2018                             */
/************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
class AisNode
{

    public: 

        AisNode();
        ~AisNode();
    
    
    protected:
        std::string m_original_inform;       // input message
               
        std::string m_packet_type;           // ownship inform or other ship 
        std::string m_number_of_sen;         // number of sentence
        std::string m_sen_num;               // sentence number
        std::string m_seq_mess_id;           // sequence message id
        std::string m_channel;               // input channel
        std::string m_main_message;          // position and others
        

        std::vector<int> m_main_bin_message;
        std::vector<std::string> m_parse_information;

        std::string m_user_id;               // ship ID
        std::string m_navigation_status;     // ship status
       
       
        double m_sog;                        // speed over ground
        double m_cog;                        // course over ground
        double m_lon;                        // lontitude
        double m_lat;                        // latitude
        double m_true_heading;
   
    public:
    
        void   setOrin(std::string input);
        void   analysis();
        
       
        std::string getPacketType() const;            
        std::string getNumberOfSen() const;         
        std::string getSenNum() const;               
        std::string getSeqMessID() const;           
        std::string getChannel() const;               
        std::string getMainMessage() const; 


        std::string getUserID() const;
        std::string getNavigationStatus() const;

        double getSog();
        double getCog();
        double getLon();
        double getLat();
        double getTrueHeading();

    protected:  //tool function use in class

        std::string CharToAscii(char input); //input is char 
        std::vector<int> AsciiToBinVec(std::vector<std::string> input);
};
