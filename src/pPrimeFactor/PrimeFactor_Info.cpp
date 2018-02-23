/****************************************************************/
/*   NAME: YH_Huang                                             */
/*   ORGN: MIT/NTU                                     */
/*   FILE: PrimeFactor_Info.cpp                               */
/*   DATE: FEB.21/2018                                        */
/****************************************************************/

#include <cstdlib>
#include <iostream>
#include "PrimeFactor_Info.h"
#include "ColorParse.h"
#include "ReleaseInfo.h"

using namespace std;

//----------------------------------------------------------------
// Procedure: showSynopsis

void showSynopsis()
{
  blk("SYNOPSIS:                                                       ");
  blk("------------------------------------                            ");
  blk("  The pPrimeFactor application is used for calculating the prime list of the input number");
  blk("  This app use one double-linked-list to manage the prime, create a class \"PrimeNode\" to  ");
  blk("  manage the prime is done or not and store the information of the prime calculating process ");
  blk("  ");
  blk("  Using STL list to finish it, in the Iterate loop, pointer will check out the element inside,");
  blk("  and using prime calculate function(call by reference) to get the node information, and if it");
  blk("  is done, it will be format to the output style and then Notify. After Notify, erase it from ");
  blk("  the loop. If it's not done, just not erase it and go to the next point");
}

//----------------------------------------------------------------
// Procedure: showHelpAndExit

void showHelpAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("Usage: pPrimeFactor file.moos [OPTIONS]                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("Options:                                                        ");
  mag("  --alias","=<ProcessName>                                      ");
  blk("      Launch pPrimeFactor with the given process name         ");
  blk("      rather than pPrimeFactor.                           ");
  mag("  --example, -e                                                 ");
  blk("      Display example MOOS configuration block.                 ");
  mag("  --help, -h                                                    ");
  blk("      Display this help message.                                ");
  mag("  --interface, -i                                               ");
  blk("      Display MOOS publications and subscriptions.              ");
  mag("  --version,-v                                                  ");
  blk("      Display the release version of pPrimeFactor.        ");
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
  blu("pPrimeFactor Example MOOS Configuration                   ");
  blu("=============================================================== ");
  blk("Need to use the uTimerScript to Notify prime, and if uTimer and pPrime");
  blk("are set to equal or uTimer be greater, the easier calculation will be finished first");
  blk("");
  blk("ProcessConfig = pPrimeFactor                              ");
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
  blu("pPrimeFactor INTERFACE                                    ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("SUBSCRIPTIONS: NUM_VALUE                                        ");
  blk("------------------------------------                            ");
  blk("Using uTimerScript or uPokeDB to give the number to test it,");
  blk("(NUM_VALUE need to be in type of \"string\")");
  blk("                                                                ");
  blk("PUBLICATIONS: PRIME_RESULT                                      ");
  blk("------------------------------------                            ");
  blk("PRIME_RESULT contains informations including  original number, ");
  blk("receive index, calculate index, calculate time, prime list and user name ");
  blk("                                                                ");
  exit(0);
}

//----------------------------------------------------------------
// Procedure: showReleaseInfoAndExit

void showReleaseInfoAndExit()
{
  showReleaseInfo("pPrimeFactor", "gpl");
  exit(0);
}

