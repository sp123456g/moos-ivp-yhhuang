#--------------------------------------------------------
# The CMakeLists.txt for:                       pDataReceiver
# Author(s):                              
#--------------------------------------------------------

SET(SRC
  DataReceiver.cpp
  DataReceiver_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pDataReceiver ${SRC})

TARGET_LINK_LIBRARIES(pDataReceiver
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

