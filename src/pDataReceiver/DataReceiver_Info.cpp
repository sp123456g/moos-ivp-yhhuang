/****************************************************************/
/*   NAME: yhhuang                                              */
/*   ORGN: NTU                                                  */
/*   FILE: DataReceiver_Info.cpp                                */
/*   DATE: Nov 9th, 2018                                        */
/****************************************************************/

#include <cstdlib>
#include <iostream>
#include "DataReceiver_Info.h"
#include "ColorParse.h"
#include "ReleaseInfo.h"

using namespace std;

//----------------------------------------------------------------
// Procedure: showSynopsis

void showSynopsis()
{
  blk("SYNOPSIS:                                                       ");
  blk("------------------------------------                            ");
  blk("  The pDataReceiver application is used for read the file       ");
  blk("  which contain MOOS variable inside it and Notify to MOOSDB    ");
  blk("                                                                ");
  blk("                                                                ");
}

//----------------------------------------------------------------
// Procedure: showHelpAndExit

void showHelpAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("Usage: pDataReceiver file.moos [OPTIONS]                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("Options:                                                        ");
  mag("  --alias","=<ProcessName>                                      ");
  blk("      Launch pDataReceiver with the given process name         ");
  blk("      rather than pDataReceiver.                           ");
  mag("  --example, -e                                                 ");
  blk("      Display example MOOS configuration block.                 ");
  mag("  --help, -h                                                    ");
  blk("      Display this help message.                                ");
  mag("  --interface, -i                                               ");
  blk("      Display MOOS publications and subscriptions.              ");
  mag("  --version,-v                                                  ");
  blk("      Display the release version of pDataReceiver.        ");
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
  blu("pDataReceiver Example MOOS Configuration                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  blk("ProcessConfig = pDataReceiver                              ");
  blk("{                                                               ");
  blk("  AppTick   = 4                                                 ");
  blk("  CommsTick = 4                                                 ");
  blk("                                                                ");
  blk("  file_path        = ./                                         ");
  blk("  file_name        = variable.txt                               ");
  blk("  file_order       = true                                       ");
  blk("  from_pDataSender = true                                       ");
  blk("}                                                               ");
  blk("                                                                ");
  blk("  file_path: path of the file you want with \"/\" in the last   ");
  blk("                                                                ");
  blk("  file_name: file name, if it sent by pDataSender, here just    ");
  blk("   need main part, ex: 0_test.txt_sending, just need test.txt   ");
  blk("                                                                ");
  blk("  file_order: if you have more than one file and you want to    ");
  blk("  have correct order, try to add index before file name,        ");
  blk("        filename ex: 0_test.txt, 1_test.txt 2_test.txt ...      ");
  blk("        string value: true or false                             ");
  blk("                                                                ");
  blk("  from_pDataSender: If your data is from pDataSender,           ");
  blk("      and your filename will be all set by pDataSender          ");
  blk("        string value: true or false                             ");
  exit(0);
}


//----------------------------------------------------------------
// Procedure: showInterfaceAndExit

void showInterfaceAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("pDataReceiver INTERFACE                                    ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("SUBSCRIPTIONS:                                                  ");
  blk("------------------------------------                            ");
  blk("  NODE_MESSAGE = src_node=alpha,dest_node=bravo,var_name=FOO,   ");
  blk("                 string_val=BAR                                 ");
  blk("                                                                ");
  blk("PUBLICATIONS:                                                   ");
  blk("------------------------------------                            ");
  blk("  Notify string value of the variable in the file               ");
  blk("                                                                ");
  blk("  File format: one line contain variable and value without space");
  blk("     ex:  FACING_ANGLE=40,20,70,                                ");
  exit(0);
}

//----------------------------------------------------------------
// Procedure: showReleaseInfoAndExit

void showReleaseInfoAndExit()
{
  showReleaseInfo("pDataReceiver", "gpl");
  exit(0);
}

