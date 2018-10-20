/****************************************************************/
/*   NAME: yhhuang                                              */
/*   ORGN: NTU Taiwan                                           */
/*   FILE: WhistleDetector_vehicle_Info.cpp                                */
/*   DATE: Aug 17th 2018                                        */
/****************************************************************/

#include <cstdlib>
#include <iostream>
#include "WhistleDetector_vehicle_Info.h"
#include "ColorParse.h"
#include "ReleaseInfo.h"

using namespace std;

//----------------------------------------------------------------
// Procedure: showSynopsis

void showSynopsis()
{
  blk("SYNOPSIS:                                                       ");
  blk("------------------------------------                            ");
  blk("  The pWhistleDetector_vehicle is used for analysis acoustics   ");
  blk("  data and check if there are whistle or chirp signal exist.    ");
  blk("                                                                ");
  blk("  pWhistleDetector is used at shoreside, and                    ");
  blk("  pWhistleDetector_vehicle is used at vehicle                   ");
  blk("                                                                ");
  blk("  Remember to set up pShare to send the Sound data back         ");
  blk("                                                                ");
  blk("  Installation Package:                                         ");
  blk("     fftw3                                                      ");
}

//----------------------------------------------------------------
// Procedure: showHelpAndExit

void showHelpAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("Usage: pWhistleDetector_vehicle file.moos [OPTIONS]                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("Options:                                                        ");
  mag("  --alias","=<ProcessName>                                      ");
  blk("      Launch pWhistleDetector_vehicle with the given process name         ");
  blk("      rather than pWhistleDetector_vehicle.                           ");
  mag("  --example, -e                                                 ");
  blk("      Display example MOOS configuration block.                 ");
  mag("  --help, -h                                                    ");
  blk("      Display this help message.                                ");
  mag("  --interface, -i                                               ");
  blk("      Display MOOS publications and subscriptions.              ");
  mag("  --version,-v                                                  ");
  blk("      Display the release version of pWhistleDetector_vehicle.        ");
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
  blk("                                                                 ");
  blu("===============================================================  ");
  blu("pWhistleDetector_vehicle Example MOOS Configuration                      ");
  blu("===============================================================  ");
  blk("                                                                 ");
  blk("ProcessConfig = pWhistleDetector_vehicle                                 ");
  blk("{                                                                ");
  blk("        AppTick            = 4                                   ");
  blk("        CommsTick          = 4                                   ");
  blk("                                                                 ");
  blk("        overlap            = 0.9                                 ");
  blk("        window_length      = 1024                                ");
  blk("        sample_rate        = 48000                               ");
  blk("        iterate_data       = 1                                   ");
  blk("        window_type        = hanning (or rectangular)            ");
  blk("        bits               = 16                                  ");
  blk("        SNR_threshold      = 12                                  ");
  blk("        band_pass_frq_low  = 3000                                ");
  blk("        band_pass_frq_high = 10000                               ");
  blk("}                                                                ");
  blk("                                                                 ");
  blk(" overlap:            STFT overlap between each window            ");
  blk(" window_length:      samples, must be power of two to be faster  ");
  blk(" sample_rate:        how many sample per second                  ");
  blk(" iterate_data:       how many seconds data each iterate          ");
  blk(" window_type:        STFT window type, hanning or rectagular     ");
  blk(" bits:               resolution for recording                    ");
  blk(" SNR_threshold:      SNR threshold for edge detector             ");
  blk(" band_pass_frq_low:  lower  freqeuncy for band pass filter       ");
  blk(" band_pass_frq_high: higher freqeuncy for band pass filter       ");
  blk("                                                                 ");
  exit(0);
}


//----------------------------------------------------------------
// Procedure: showInterfaceAndExit

void showInterfaceAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("pWhistleDetector_vehicle INTERFACE                              ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("SUBSCRIPTIONS:                                                  ");
  blk("------------------------------------                            ");
  blk(" SOUND_VOLTAGE_DATA_CH_ONE:                                     ");
  blk("     string value, Voltage data seperate by comma               ");
  blk("                                                                ");
  blk("PUBLICATIONS:                                                   ");
  blk("------------------------------------                            ");
  blk("  WHISTLE_EXIST:                                                ");
  blk("     string value, \"true\" or \"false\"                        ");
  blk("                                                                ");
  blk("  WHISTLE_VOLTAGE_DATA:                                   ");
  blk("     string value, voltage data contains whistle in iterate     ");
  blk("                   seconds voltage data seperate with comma     ");
  blk("                                                                ");
  exit(0);
}

//----------------------------------------------------------------
// Procedure: showReleaseInfoAndExit

void showReleaseInfoAndExit()
{
  showReleaseInfo("pWhistleDetector_vehicle", "gpl");
  exit(0);
}

