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
  blk("     If you build in ubuntu, use command:                       ");
  blk("          sudo apt-get install libfftw3-dev libfftw3-doc        ");
  blk("                                                                ");
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
  blk("        sample_rate        = 96000                               ");
  blk("        iterate_data       = 0.5                                 ");
  blk("        window_type        = hanning (or rectangular)            ");
  blk("        bits               = 32                                  ");
  blk("        SNR_threshold      = 15                                  ");
  blk("        band_pass_frq_low  = 3000                                ");
  blk("        band_pass_frq_high = 10000                               ");
  blk("                                                                 ");
  blk("        channel_need = 2                                         ");
  blk("}                                                                ");
  blk("                                                                 ");
  blk(" overlap:            STFT overlap between each window            ");
  blk(" sample_rate:        how many sample per second                  ");
  blk(" iterate_data:       how many seconds data each iterate          ");
  blk(" window_type:        STFT window type, hanning or rectagular     ");
  blk(" bits:               resolution for recording                    ");
  blk(" SNR_threshold:      SNR threshold for edge detector             ");
  blk(" band_pass_frq_low:  lower  freqeuncy for band pass filter       ");
  blk(" band_pass_frq_high: higher freqeuncy for band pass filter       ");
  blk(" channel_need:       how many channel data you want to send      ");
  blk("                     out when whistle is detected                ");
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
  blk("     string value, Voltage seperate by \",\" in channel one     ");
  blk("     string value, Voltage seperate by \",\" in channel two     ");
  blk("     string value, Voltage seperate by \",\" in channel three   ");
  blk("     string value, Voltage seperate by \",\" in channel four    ");
  blk("                                                                ");
  blk(" RECORD_FRAMES:                                                 ");
  blk("     \"string\" value, variable pulished by pStoreSoundX        ");
  blk("                       How many loops per frame                 ");
  blk("                                                                ");
  blk("PUBLICATIONS:                                                   ");
  blk("------------------------------------                            ");
  blk("  WHISTLE_EXIST:                                                ");
  blk("     string value, \"true\" or \"false\"                        ");
  blk("                                                                ");
  blk("  WHISTLE_VOLTAGE_DATA_CH_ONE:                                  ");
  blk("     string value, voltage data contains whistle in  1/2 iterate");
  blk("                   seconds voltage data seperate with comma     ");
  blk("                                                                ");
  blk("  WHISTLE_VOLTAGE_DATA_CH_TWO:                                  ");
  blk("     string value, same as ch one if you need                   ");
  blk("                   Remember to check channel_need >=2           ");
  blk("  WHISTLE_VOLTAGE_DATA_CH_THREE:                                ");
  blk("     string value, same as ch one if you need                   ");
  blk("                   Remember to check channel_need >=3           ");
  blk("  WHISTLE_VOLTAGE_DATA_CH_FOUR:                                 ");
  blk("     string value, same as ch one if you need                   ");
  blk("                   Remember to check channel_need =4            ");
  blk("                                                                ");
  blk("                                                                ");
  blk("  GET_VOLTAGE_TIME:                                             ");
  blk("     time spent in parse voltage string                         ");
  blk("                                                                ");
  blk("  STFT_TIME:                                                    ");
  blk("     time spent in STFT process                                 ");
  blk("                                                                ");
  blk("  DETECTION_TIME:                                               ");
  blk("     time spent in detection process                            ");
  blk("                                                                ");
  blk("  FRQ_ANALYSIS_TIME:                                            ");
  blk("     time spent in getting exact freqeuncy                      ");
  exit(0);
}

//----------------------------------------------------------------
// Procedure: showReleaseInfoAndExit

void showReleaseInfoAndExit()
{
  showReleaseInfo("pWhistleDetector_vehicle", "gpl");
  exit(0);
}

