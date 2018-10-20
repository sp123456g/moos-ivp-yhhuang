/****************************************************************/
/*   NAME: yhhuang                                              */
/*   ORGN: Naational Taiwan University                          */
/*   FILE: DataSender_Info.cpp                                  */
/*   DATE: Oct 20th 2018                                        */
/****************************************************************/

#include <cstdlib>
#include <iostream>
#include "DataSender_Info.h"
#include "ColorParse.h"
#include "ReleaseInfo.h"

using namespace std;

//----------------------------------------------------------------
// Procedure: showSynopsis

void showSynopsis()
{
  blk("SYNOPSIS:                                                       ");
  blk("------------------------------------                            ");
  blk("  The pDataSender application is used for sending the file      ");
  blk("  between the machine and user by using scp command             ");
  blk("                                                                ");
  blk("  If data is sent successfully, filename will add _done at the end");
  blk("                                                                ");
  blk("  Make sure you have already put your public ssh key into       ");
  blk("  destination machine                                           ");
  blk("                                                                ");
  blk("  If you can ssh your machine without typing password,          ");
  blk("    you are all setted                                          ");
}

//----------------------------------------------------------------
// Procedure: showHelpAndExit

void showHelpAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("Usage: pDataSender file.moos [OPTIONS]                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("Options:                                                        ");
  mag("  --alias","=<ProcessName>                                      ");
  blk("      Launch pDataSender with the given process name         ");
  blk("      rather than pDataSender.                           ");
  mag("  --example, -e                                                 ");
  blk("      Display example MOOS configuration block.                 ");
  mag("  --help, -h                                                    ");
  blk("      Display this help message.                                ");
  mag("  --interface, -i                                               ");
  blk("      Display MOOS publications and subscriptions.              ");
  mag("  --version,-v                                                  ");
  blk("      Display the release version of pDataSender.        ");
  blk("                                                                ");
  blk("Note: If argv[2] does not otherwise match a known option,       ");
  blk("      then it will be interpreted as a run alias. This is       ");
  blk("      to support pAntler launching conventions.                 ");
  blk("                                                                ");
  blk("SetUp:                                                          ");
  blk(" Put your public key into the machine you want to send to:  ");
  blk("     step 1: type \"ssh-keygen\" in command line            ");
  blk("     step 2: type \"cat ~/.ssh/id_rsa.pub | ssh user@IP \"cat >> ~/.ssh/authorized_keys\"\"");
  blk("     step 3: If fail, check if the file in destination machine can be modified");
  blk("     step 4: Check if you can ssh into machine without typing password, ");
  blk("             If it work fine, you are all set, if not, type \"ssh-add\" if needed");
  blk("                                                                ");
  blk("                                                                ");
  exit(0);
}

//----------------------------------------------------------------
// Procedure: showExampleConfigAndExit

void showExampleConfigAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("pDataSender Example MOOS Configuration                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  blk("ProcessConfig = pDataSender                              ");
  blk("{                                                               ");
  blk("  AppTick   = 4                                                 ");
  blk("  CommsTick = 4                                                 ");
  blk("     sender_filepath    = ./TestApp/                        ");
  blk("     reciever_user           = yhhuang                          ");
  blk("     reciever_machine        = 140.113.34.123                   ");
  blk("     reciever_filepath  = ~/folder_name                         ");
  blk("}                                                               ");
  blk("                                                                ");
  blk("  sender_filepath:   the path of the file you want to send(remember to add / in the end)");
  blk("                     and folder must exist                      ");
  blk("  reciever_user:     the user name you want to send to          ");
  blk("  reciever_machine:  the IP address or machine of destination   ");
  blk("  reciever_filepath: the directory you want to store files      ");
  exit(0);
}


//----------------------------------------------------------------
// Procedure: showInterfaceAndExit

void showInterfaceAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("pDataSender INTERFACE                                    ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("SUBSCRIPTIONS:                                                  ");
  blk("------------------------------------                            ");
  blk("     SEND_DATA_BY_DATASENDER = true or false                    ");
  blk("         true: send data immediately                            ");
  blk("                                                                ");
  blk("PUBLICATIONS:                                                   ");
  blk("------------------------------------                            ");
  blk("  Publications are determined by the node message content.      ");
  blk("                                                                ");
  exit(0);
}

//----------------------------------------------------------------
// Procedure: showReleaseInfoAndExit

void showReleaseInfoAndExit()
{
  showReleaseInfo("pDataSender", "gpl");
  exit(0);
}

