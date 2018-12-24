/****************************************************************/
/*   NAME: yhhuang                                             */
/*   ORGN: MIT Cambridge MA                                     */
/*   FILE: AmbientCalculate_Info.cpp                               */
/*   DATE: Dec 29th 1963                                        */
/****************************************************************/

#include <cstdlib>
#include <iostream>
#include "AmbientCalculate_Info.h"
#include "ColorParse.h"
#include "ReleaseInfo.h"

using namespace std;

//----------------------------------------------------------------
// Procedure: showSynopsis

void showSynopsis()
{
  blk("SYNOPSIS:                                                       ");
  blk("------------------------------------                            ");
  blk("  The pAmbientCalculate application is used for                 ");
  blk("  calculating average voltage and spl                           ");
  blk("                                                                ");
  blk("                                                                ");
  blk("                                                                ");
}

//----------------------------------------------------------------
// Procedure: showHelpAndExit

void showHelpAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("Usage: pAmbientCalculate file.moos [OPTIONS]                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("Options:                                                        ");
  mag("  --alias","=<ProcessName>                                      ");
  blk("      Launch pAmbientCalculate with the given process name         ");
  blk("      rather than pAmbientCalculate.                           ");
  mag("  --example, -e                                                 ");
  blk("      Display example MOOS configuration block.                 ");
  mag("  --help, -h                                                    ");
  blk("      Display this help message.                                ");
  mag("  --interface, -i                                               ");
  blk("      Display MOOS publications and subscriptions.              ");
  mag("  --version,-v                                                  ");
  blk("      Display the release version of pAmbientCalculate.        ");
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
  blu("pAmbientCalculate Example MOOS Configuration                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  blk("ProcessConfig = pAmbientCalculate                              ");
  blk("{                                                               ");
  blk("  AppTick   = 4                                                 ");
  blk("  CommsTick = 4                                                 ");
  blk("                                                                ");
  blk("  duration = 2                                                  ");
  blk("  sen      = -204                                               ");
  blk("  gain     = 0                                                  ");
  blk("  fs       = 96000                                              ");
  blk("                                                                ");
  blk("}                                                               ");
  blk("                                                                ");
  blk("  duration:  time duration of average                           ");
  blk("  sen:       hydrophone sensitivity                             ");
  blk("  gain:      input gain                                         ");
  blk("  fs:        sample rate                                        ");
  blk("                                                                ");
  exit(0);
}


//----------------------------------------------------------------
// Procedure: showInterfaceAndExit

void showInterfaceAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("pAmbientCalculate INTERFACE                                    ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("SUBSCRIPTIONS:                                                  ");
  blk("------------------------------------                            ");
  blk("       string value: SOUND_VOLTAGE_DATA_CH_ONE                  ");
  blk("                 ch1 voltage data seperate by comma             ");
  blk("                                                                ");
  blk("       string value: SOUND_VOLTAGE_DATA_CH_TWO                  ");
  blk("                 ch2 voltage data seperate by comma             ");
  blk("                                                                ");
  blk("PUBLICATIONS:                                                   ");
  blk("------------------------------------                            ");
  blk("   double value:  AVG_SPL_CH_ONE                                ");
  blk("   double value:  AVG_VOLTAGE_CH_ONE                            ");
  blk("                                                                ");
  exit(0);
}

//----------------------------------------------------------------
// Procedure: showReleaseInfoAndExit

void showReleaseInfoAndExit()
{
  showReleaseInfo("pAmbientCalculate", "gpl");
  exit(0);
}

