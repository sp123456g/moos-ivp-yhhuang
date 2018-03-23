/****************************************************************/
/*   NAME: YHHuang                                             */
/*   ORGN: MIT Cambridge MA                                     */
/*   FILE: AISdecoder_Info.cpp                               */
/*   DATE: MAR 19th 2018                                     */
/****************************************************************/

#include <cstdlib>
#include <iostream>
#include "AISdecoder_Info.h"
#include "ColorParse.h"
#include "ReleaseInfo.h"

using namespace std;

//----------------------------------------------------------------
// Procedure: showSynopsis

void showSynopsis()
{
  blk("SYNOPSIS:                                                       ");
  blk("------------------------------------                            ");
  blk("  The pAISdecoder application is used for               ");
  blk("  decoding the AIS data which is in NMEA sentence.               ");
  blk("  The Output include Latitude(Lat), Longitude(Lon), CourseOverGround(Cog), TrueHeading, SpeedOverGround(Sog)");
  blk("                                                                ");
  blk("     Only output for type 1,2,3 message(Position message)\n     ");
  blk("     Unit:                                                      ");
  blk("         ShipID:      Maritime Mobile Service Identity (MMSI)   ");
  blk("         Longitude:   Degree");
  blk("         Latitude:    Degree");
  blk("         Cog:         Degree");
  blk("         TrueHeading: Degree");
  blk("         Sog:         Knots");
  blk("                                                                ");
}

//----------------------------------------------------------------
// Procedure: showHelpAndExit

void showHelpAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("Usage: pAISdecoder file.moos [OPTIONS]                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("Options:                                                        ");
  mag("  --alias","=<ProcessName>                                      ");
  blk("      Launch pAISdecoder with the given process name         ");
  blk("      rather than pAISdecoder.                           ");
  mag("  --example, -e                                                 ");
  blk("      Display example MOOS configuration block.                 ");
  mag("  --help, -h                                                    ");
  blk("      Display this help message.                                ");
  mag("  --interface, -i                                               ");
  blk("      Display MOOS publications and subscriptions.              ");
  mag("  --version,-v                                                  ");
  blk("      Display the release version of pAISdecoder.        ");
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
  blu("pAISdecoder Example MOOS Configuration                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  blk("ProcessConfig = pAISdecoder                              ");
  blk("{                                                               ");
  blk("  AppTick   = 4                                                 ");
  blk("  CommsTick = 4                                                 ");
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
  blu("pAISdecoder INTERFACE                                    ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("SUBSCRIPTIONS:                                                  ");
  blk("------------------------------------                            ");
  blk("  AIS_DATA   ");
  blk("     string_val");
  blk("     NMEA sentence,                               ");
  blk("     Ex:                                                        ");
  blk("         !AIVDM,1,1,,A,33P;D3E00000PkHLD;TV17w60000,0*4A\n      ");
  blk("PUBLICATIONS:                                                   ");
  blk("------------------------------------                            ");
  blk("   AIS_RESULT                                                   ");
  blk("     string_val                                                 "); 
  blk("     Ex:"                                                        );
  blk("        If input is normal NMEA sentence:");
  blk("        AIS_RESULT=\"ShipID=235066381,Lon=0.111967,Lat=49.484617,Cog=154,TrueHeading=255,Sog=0\"\n");
  blk("        If input message is wrong type:");
  blk("        AIS_RESULT=\"Wrong_Message_Type\"\n");
  blk("---------------------------------------------------------------");
  exit(0);
}

//----------------------------------------------------------------
// Procedure: showReleaseInfoAndExit

void showReleaseInfoAndExit()
{
  showReleaseInfo("pAISdecoder", "gpl");
  exit(0);
}

