#--------------------------------------------------------
# The CMakeLists.txt for:                       uSimAngle
# Author(s):                             yhhuang 
#--------------------------------------------------------

SET(SRC
  SimAngle.cpp
  SimAngle_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(uSimAngle ${SRC})

TARGET_LINK_LIBRARIES(uSimAngle
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

