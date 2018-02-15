#--------------------------------------------------------
# The CMakeLists.txt for:                       pOdometry_new
# Author(s):                              YH Huang
#--------------------------------------------------------

SET(SRC
  Odometry.cpp
  Odometry_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pOdometry ${SRC})

TARGET_LINK_LIBRARIES(pOdometry
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

