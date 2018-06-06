/****************************************************************/
/*   NAME: yhhuang                                              */
/*   ORGN: MIT Cambridge MA                                     */
/*   FILE: STFTAnalyser_Info.cpp                                */
/*   DATE: May 29th 2018                                        */
/****************************************************************/

#include <cstdlib>
#include <iostream>
#include "STFTAnalyser_Info.h"
#include "ColorParse.h"
#include "ReleaseInfo.h"

using namespace std;

//----------------------------------------------------------------
// Procedure: showSynopsis

void showSynopsis()
{
  blk("SYNOPSIS:                                                       ");
  blk("------------------------------------                            ");
  blk("  The pSTFTAnalyser application is used for analysis acoustics  ");
  blk("  data, input the acoustic data and output the spectrogram matrix");
  blk("                                                                 ");
  blk("                                                                ");
  blk("                                                                ");
}

//----------------------------------------------------------------
// Procedure: showHelpAndExit

void showHelpAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("Usage: pSTFTAnalyser file.moos [OPTIONS]                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("Options:                                                        ");
  mag("  --alias","=<ProcessName>                                      ");
  blk("      Launch pSTFTAnalyser with the given process name         ");
  blk("      rather than pSTFTAnalyser.                           ");
  mag("  --example, -e                                                 ");
  blk("      Display example MOOS configuration block.                 ");
  mag("  --help, -h                                                    ");
  blk("      Display this help message.                                ");
  mag("  --interface, -i                                               ");
  blk("      Display MOOS publications and subscriptions.              ");
  mag("  --version,-v                                                  ");
  blk("      Display the release version of pSTFTAnalyser.        ");
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
  blu("pSTFTAnalyser Example MOOS Configuration                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  blk("ProcessConfig = pSTFTAnalyser                              ");
  blk("{                                                               ");
  blk("        AppTick     = 4                                         ");
  blk("        CommsTick   = 4                                         ");
  blk("        dt          = 0.01                                      ");
  blk("        df          = 20                                        ");
  blk("        sample_rate = 44100                                     ");
  blk("detection_threshold = 30                                        ");
  blk("        do_detect   = true                                      ");
  blk("       iterate_data = 1                                         ");
  blk("        window_type = rec (or gassian)                          ");
  blk("        sigma     = 600                                         ");
  blk("        bandwidth   = 0.02                                      ");
  blk("}                                                               ");
  blk("                                                                ");
  exit(0);
}


//----------------------------------------------------------------
// Procedure: showInterfaceAndExit

void showInterfaceAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("pSTFTAnalyser INTERFACE                                    ");
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
  blk("  Publications are determined by the node message content.      ");
  blk("                                                                ");
  exit(0);
}

//----------------------------------------------------------------
// Procedure: showReleaseInfoAndExit

void showReleaseInfoAndExit()
{
  showReleaseInfo("pSTFTAnalyser", "gpl");
  exit(0);
}

