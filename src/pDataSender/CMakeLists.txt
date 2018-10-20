#--------------------------------------------------------
# The CMakeLists.txt for:                       pDataSender
# Author(s):                              yhhuang
#--------------------------------------------------------

SET(SRC
  DataSender.cpp
  DataSender_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pDataSender ${SRC})

TARGET_LINK_LIBRARIES(pDataSender
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

