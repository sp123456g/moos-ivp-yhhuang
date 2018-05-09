#--------------------------------------------------------
# The CMakeLists.txt for:                       pFrontEstimateX
# Author(s):                              
#--------------------------------------------------------

SET(SRC
  FrontEstimateX.cpp
  FrontEstimateX_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pFrontEstimateX ${SRC})

TARGET_LINK_LIBRARIES(pFrontEstimateX
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

