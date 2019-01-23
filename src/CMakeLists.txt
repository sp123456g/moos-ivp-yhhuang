##############################################################################
# FILE:        moos-ivp-extend/src/CMakeLists.txt
# DATE:        2010/09/07
# DESCRIPTION: CMakeLists.txt file for the moos-ivp-extend source directory
##############################################################################


#============================================================================
# Add the libraries in the current directory to the include path
#============================================================================
FILE(GLOB LOCAL_LIBRARY_DIRS ./lib_*)
INCLUDE_DIRECTORIES(${LOCAL_LIBRARY_DIRS})

#============================================================================
# List the subdirectories to build...
#============================================================================
ADD_SUBDIRECTORY(lib_behaviors-test)
ADD_SUBDIRECTORY(lib_behaviors-yhhuang)
ADD_SUBDIRECTORY(pXRelayTest)
ADD_SUBDIRECTORY(pExampleApp)
ADD_SUBDIRECTORY(pOdometry)
ADD_SUBDIRECTORY(pPrimeFactor)
ADD_SUBDIRECTORY(pPrimeFactorTester)
ADD_SUBDIRECTORY(pPointAssign)
ADD_SUBDIRECTORY(pGenPath)
ADD_SUBDIRECTORY(pGenPath_RePlanning)
ADD_SUBDIRECTORY(pAISdecoder)
ADD_SUBDIRECTORY(pCommunicationAngle_yhhuang)
ADD_SUBDIRECTORY(pPointAssign_Auto)
ADD_SUBDIRECTORY(uFldHazardMgrX)
ADD_SUBDIRECTORY(pHazardPath)
ADD_SUBDIRECTORY(pFrontEstimateX)
ADD_SUBDIRECTORY(pWhistleDetector)
ADD_SUBDIRECTORY(pWhistleDetector_vehicle)
ADD_SUBDIRECTORY(pStoreSoundX)
ADD_SUBDIRECTORY(pResultSaver)
ADD_SUBDIRECTORY(pDataSender)
ADD_SUBDIRECTORY(pDataReceiver)
ADD_SUBDIRECTORY(pDirectionCalculator)
ADD_SUBDIRECTORY(pAmbientCalculate)
ADD_SUBDIRECTORY(pPoseKeep)
#ADD_SUBDIRECTORY(uSimAngle)
ADD_SUBDIRECTORY(pStoreSoundX_TX1)
##############################################################################
#                           END of CMakeLists.txt
##############################################################################

