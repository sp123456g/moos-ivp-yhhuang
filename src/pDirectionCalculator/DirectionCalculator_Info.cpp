/****************************************************************/
/*   NAME:                                              */
/*   ORGN: MIT Cambridge MA                                     */
/*   FILE: DirectionCalculator_Info.cpp                               */
/*   DATE: Dec 29th 1963                                        */
/****************************************************************/

#include <cstdlib>
#include <iostream>
#include "DirectionCalculator_Info.h"
#include "ColorParse.h"
#include "ReleaseInfo.h"

using namespace std;

//----------------------------------------------------------------
// Procedure: showSynopsis

void showSynopsis()
{
  blk("SYNOPSIS:                                                       ");
  blk("------------------------------------                            ");
  blk("  The pDirectionCalculator application is used for              ");
  blk("  calculating the underwater acoustic source direction          ");
  blk("  by TDOA method, support two and three hydrophones             ");
  blk("                                                                ");
  blk("                                                                ");
}

//----------------------------------------------------------------
// Procedure: showHelpAndExit

void showHelpAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("Usage: pDirectionCalculator file.moos [OPTIONS]                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("Options:                                                        ");
  mag("  --alias","=<ProcessName>                                      ");
  blk("      Launch pDirectionCalculator with the given process name         ");
  blk("      rather than pDirectionCalculator.                           ");
  mag("  --example, -e                                                 ");
  blk("      Display example MOOS configuration block.                 ");
  mag("  --help, -h                                                    ");
  blk("      Display this help message.                                ");
  mag("  --interface, -i                                               ");
  blk("      Display MOOS publications and subscriptions.              ");
  mag("  --version,-v                                                  ");
  blk("      Display the release version of pDirectionCalculator.        ");
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
  blu("pDirectionCalculator Example MOOS Configuration                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  blk("ProcessConfig = pDirectionCalculator                              ");
  blk("{                                                                ");
  blk("  AppTick   = 4                                                  ");
  blk("  CommsTick = 4                                                  ");
  blk("                                                                 ");
  blk("  sample_rate    = 96000                                         ");
  blk("  sound_speed    = 1500                                          ");
  blk("  hyd_number     = 1                                             ");
  blk("  hyd_dis        = 1                                             ");
  blk("  use_cc         = true                                          ");
  blk("  interval       = 0,5                                           ");
  blk("  band_pass      = 3:10                                          ");
  blk("}                                                               ");
  blk("                                                                ");
  blk("  sample_rate: sampling frequency in Hz                         ");
  blk("  sound_speed: sound speed in the water (m/s)                   ");
  blk("  hyd_number : how many channel use for calculating TDOA        ");
  blk("  hyd_dis    : distance between two hydrophones (meter)         ");
  blk("  use_cc     : true if you want to use cross-correlation methond");
  blk("               false for using peak finding method              ");
  blk("  interval   : set up signal duration for calculating TDOA      ");
  blk("               AppTick and CommsTick suggest to be 1/interval   ");
  blk("  band_pass  : band-pass filter from lower to higher frq (kHz)  ");
  blk("                                                                ");
  exit(0);
}


//----------------------------------------------------------------
// Procedure: showInterfaceAndExit

void showInterfaceAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("pDirectionCalculator INTERFACE                                    ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("SUBSCRIPTIONS:                                                  ");
  blk("------------------------------------                            ");
  blk(" WHISTLE_VOLTAGE_DATA_CH_ONE:                                   ");
  blk(" string variable with specific duration voltage data from ch1   ");
  blk("                                                                ");
  blk(" WHISTLE_VOLTAGE_DATA_CH_TWO:                                   ");
  blk(" string variable with specific duration voltage data from ch2   ");
  blk("                                                                ");
  blk(" WHISTLE_VOLTAGE_DATA_CH_THREE:                                 ");
  blk(" string variable with specific duration voltage data from ch3   ");
  blk("                                                                ");
  blk("PUBLICATIONS:                                                   ");
  blk("------------------------------------                            ");
  blk("  SOURCE_ANGLE:                                                 ");
  blk("      string variable, \"NAN\" if no answer                     ");
  blk("      double variable,  direction angle in degree               ");
  blk("                                                                ");
  exit(0);
}

//----------------------------------------------------------------
// Procedure: showReleaseInfoAndExit

void showReleaseInfoAndExit()
{
  showReleaseInfo("pDirectionCalculator", "gpl");
  exit(0);
}

