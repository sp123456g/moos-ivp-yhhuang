/****************************************************************/
/*   NAME: yhhuang                                              */
/*   ORGN: NTU Taiwan                                           */
/*   FILE: WhistleDetector_Info.cpp                                */
/*   DATE: Aug 17th 2018                                        */
/****************************************************************/

#include <cstdlib>
#include <iostream>
#include "WhistleDetector_Info.h"
#include "ColorParse.h"
#include "ReleaseInfo.h"

using namespace std;

//----------------------------------------------------------------
// Procedure: showSynopsis

void showSynopsis()
{
  blk("SYNOPSIS:                                                       ");
  blk("------------------------------------                            ");
  blk("  The pWhistleDetector is used for analysis acoustics           ");
  blk("  data and check if there are whistle or chirp signal exist.    ");
  blk("                                                                ");
  blk("  You can use SpectrogramViewer.m file to check real-time       ");
  blk("  spectrogram and result after algorithm every time             ");
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
  blu("Usage: pWhistleDetector file.moos [OPTIONS]                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("Options:                                                        ");
  mag("  --alias","=<ProcessName>                                      ");
  blk("      Launch pWhistleDetector with the given process name         ");
  blk("      rather than pWhistleDetector.                           ");
  mag("  --example, -e                                                 ");
  blk("      Display example MOOS configuration block.                 ");
  mag("  --help, -h                                                    ");
  blk("      Display this help message.                                ");
  mag("  --interface, -i                                               ");
  blk("      Display MOOS publications and subscriptions.              ");
  mag("  --version,-v                                                  ");
  blk("      Display the release version of pWhistleDetector.        ");
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
  blu("pWhistleDetector Example MOOS Configuration                      ");
  blu("===============================================================  ");
  blk("                                                                 ");
  blk("ProcessConfig = pWhistleDetector                                 ");
  blk("{                                                                ");
  blk("        AppTick            = 4                                   ");
  blk("        CommsTick          = 4                                   ");
  blk("                                                                 ");
  blk("        overlap            = 0.9                                 ");
  blk("        window_length      = 2048                                ");
  blk("        sample_rate        = 96000                               ");
  blk("        iterate_data       = 1                                   ");
  blk("        window_type        = hanning (or rectangular)            ");
  blk("        bits               = 16                                  ");
  blk("        SNR_threshold      = 10                                  ");
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
  blu("pWhistleDetector INTERFACE                                      ");
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
  blk("  SOUND_VOLTAGE_DATA_WITH_WHISTLE:                              ");
  blk("     string value, Voltage data seperate by comma               ");
  blk("                                                                ");
  exit(0);
}

//----------------------------------------------------------------
// Procedure: showReleaseInfoAndExit

void showReleaseInfoAndExit()
{
  showReleaseInfo("pWhistleDetector", "gpl");
  exit(0);
}

