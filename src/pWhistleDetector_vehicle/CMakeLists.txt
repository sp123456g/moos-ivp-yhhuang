#--------------------------------------------------------
# The CMakeLists.txt for:                       pWhistleDetector_vehicle
# Author(s):                              yhhuang
#--------------------------------------------------------

SET(SRC
  WhistleDetector_vehicle.cpp
  WhistleDetector_vehicle_Info.cpp
  detection_algorithm.cpp
  main.cpp
)

ADD_EXECUTABLE(pWhistleDetector_vehicle ${SRC})

TARGET_LINK_LIBRARIES(pWhistleDetector_vehicle
   ${MOOS_LIBRARIES}
   apputil
   fftw3f
   mbutil
   m
   pthread)

