#--------------------------------------------------------
# The CMakeLists.txt for:              pHazardPath_monica
# Author(s):                           monica
#--------------------------------------------------------

SET(SRC
  HazardPath.cpp
  HazardPath_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pHazardPath_monica ${SRC})

TARGET_LINK_LIBRARIES(pHazardPath_monica
   ${MOOS_LIBRARIES}
   geometry
   apputil
   mbutil
   m
   pthread)
