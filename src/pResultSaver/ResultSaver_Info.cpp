/****************************************************************/
/*   NAME: yhhuang                                              */
/*   ORGN: National Taiwan University                           */
/*   FILE: ResultSaver_Info.cpp                                 */
/*   DATE: Oct 20th 2018                                        */
/****************************************************************/

#include <cstdlib>
#include <iostream>
#include "ResultSaver_Info.h"
#include "ColorParse.h"
#include "ReleaseInfo.h"

using namespace std;

//----------------------------------------------------------------
// Procedure: showSynopsis

void showSynopsis()
{
  blk("SYNOPSIS:                                                       ");
  blk("------------------------------------                            ");
  blk("  The pResultSaver application is used for save the variable data");
  blk("    you need to use pDataSender to send to other machine        ");
  blk("                                                                ");
  blk("  If data is ready for sending, filename will add index at the  ");
  blk("  beginning and add _pending at the end                         ");
  blk("                                                                ");
}

//----------------------------------------------------------------
// Procedure: showHelpAndExit

void showHelpAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("Usage: pResultSaver file.moos [OPTIONS]                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("Options:                                                        ");
  mag("  --alias","=<ProcessName>                                      ");
  blk("      Launch pResultSaver with the given process name         ");
  blk("      rather than pResultSaver.                           ");
  mag("  --example, -e                                                 ");
  blk("      Display example MOOS configuration block.                 ");
  mag("  --help, -h                                                    ");
  blk("      Display this help message.                                ");
  mag("  --interface, -i                                               ");
  blk("      Display MOOS publications and subscriptions.              ");
  mag("  --version,-v                                                  ");
  blk("      Display the release version of pResultSaver.        ");
  blk("                                                                ");
  blk("Note: If argv[2] does not otherwise match a known option,       ");
  blk("      then it will be interpreted as a run alias. This is       ");
  blk("      to support pAntler launching conventions.                 ");
  blk("                                                                ");
  exit(0);
}

//----------------------------------------------------------------
// Procedure: showExampleConfigAndExit

void showExampleConfigAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("pResultSaver Example MOOS Configuration                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  blk("ProcessConfig = pResultSaver                              ");
  blk("{                                                               ");
  blk("  AppTick   = 4                                                 ");
  blk("  CommsTick = 4                                                 ");
  blk("                                                                ");
  blk("  sending_variable_name = NUM_VALUE                             ");
  blk("  filepath              = ./Data_for_sending/                   ");
  blk("  filename              = data.txt                              ");
  blk("  send_by_pDataSender   = true                                  ");
  blk("  wait_number_of_line   = 10                                    ");
  blk("}                                                               ");
  blk("                                                                ");
  blk("  sending_variable_name: the MOOS variable you want to save     ");
  blk("  filepath:              the path you want to save data with / in the end");
  blk("  filename:              the file name you want                 ");
  blk("  send_by_pDataSender:   send the file by pDataSender or not, default is false");
  blk("  wait_number_of_line:   If data lines achieve the number you set ");
  blk("                          the file will become pending mode for scp");
  exit(0);
}


//----------------------------------------------------------------
// Procedure: showInterfaceAndExit

void showInterfaceAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("pResultSaver INTERFACE                                    ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("SUBSCRIPTIONS:                                                  ");
  blk("------------------------------------                            ");
  blk(" The variable you set in .moos file                             ");
  blk("                                                                ");
  blk("                                                                ");
  blk("PUBLICATIONS:                                                   ");
  blk("------------------------------------                            ");
  blk(" SEND_DATA_BY_DATASENDER = true (Only if file need pDataSender) ");
  blk("                                                                ");
  exit(0);
}

//----------------------------------------------------------------
// Procedure: showReleaseInfoAndExit

void showReleaseInfoAndExit()
{
  showReleaseInfo("pResultSaver", "gpl");
  exit(0);
}

