#--------------------------------------------------------
# The CMakeLists.txt for:           uFldHazardMgrX_monica
# Author(s):                        monica
#--------------------------------------------------------

SET(SRC
  HazardMgrX.cpp
  HazardMgrX_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(uFldHazardMgrX_monica ${SRC})

TARGET_LINK_LIBRARIES(uFldHazardMgrX_monica
   ${MOOS_LIBRARIES}
   ufld_hazards
   geometry
   apputil
   mbutil
   ${SYSTEM_LIBS}
)

