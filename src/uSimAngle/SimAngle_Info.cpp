/****************************************************************/
/*   NAME: yhhuang                                              */
/*   ORGN: NTU, Taipei                                          */
/*   FILE: SimAngle_Info.cpp                                    */
/*   DATE: Jan 16th 2019                                        */
/****************************************************************/

#include <cstdlib>
#include <iostream>
#include "SimAngle_Info.h"
#include "ColorParse.h"
#include "ReleaseInfo.h"

using namespace std;

//----------------------------------------------------------------
// Procedure: showSynopsis

void showSynopsis()
{
  blk("SYNOPSIS:                                                       ");
  blk("------------------------------------                            ");
  blk("  The uSimAngle application is used for simulating acoustic source");
  blk("  direction if the source location is set up                    ");
  blk("                                                                ");
  blk("  The output is the angle between y axis and the line of        ");
  blk("  simulating source and vehicle                                 ");
}

//----------------------------------------------------------------
// Procedure: showHelpAndExit

void showHelpAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("Usage: uSimAngle file.moos [OPTIONS]                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("Options:                                                        ");
  mag("  --alias","=<ProcessName>                                      ");
  blk("      Launch uSimAngle with the given process name         ");
  blk("      rather than uSimAngle.                           ");
  mag("  --example, -e                                                 ");
  blk("      Display example MOOS configuration block.                 ");
  mag("  --help, -h                                                    ");
  blk("      Display this help message.                                ");
  mag("  --interface, -i                                               ");
  blk("      Display MOOS publications and subscriptions.              ");
  mag("  --version,-v                                                  ");
  blk("      Display the release version of uSimAngle.        ");
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
  blu("uSimAngle Example MOOS Configuration                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  blk("ProcessConfig = uSimAngle                              ");
  blk("{                                                               ");
  blk("  AppTick   = 4                                                 ");
  blk("  CommsTick = 4                                                 ");
  blk("                                                                ");
  blk("  source_x = 0                                                  ");
  blk("  source_y = 0                                                  ");
  blk("                                                                ");
  blk("  edge_color = blue                                             ");
  blk("  fill_color = pink                                             ");
  blk("                                                                ");
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
  blu("uSimAngle INTERFACE                                    ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("SUBSCRIPTIONS:                                                  ");
  blk("------------------------------------                            ");
  blk(" NAV_X , NAV_Y, NAV_HEADING, SIM_SOURCE_X, SIM_SOURCE_Y         ");
  blk("     double value                                               ");
  blk(" WHISTLE_EXIST_SIM                                              ");
  blk("                                                                ");
  blk("                                                                ");
  blk("PUBLICATIONS:                                                   ");
  blk("------------------------------------                            ");
  blk(" SOURCE_ANGLE, double value                                     ");
  blk(" VIEW_RANGE_PULSE, str value                                    ");
  blk("                                                                ");
  blk("                                                                ");
  exit(0);
}

//----------------------------------------------------------------
// Procedure: showReleaseInfoAndExit

void showReleaseInfoAndExit()
{
  showReleaseInfo("uSimAngle", "gpl");
  exit(0);
}

