#--------------------------------------------------------
# The CMakeLists.txt for:                       pCommunicationAngle_yhhuang
# Author(s):                              
#--------------------------------------------------------

SET(SRC
  CommunicationAngle_yhhuang.cpp
  CommunicationAngle_yhhuang_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pCommunicationAngle_yhhuang ${SRC})

TARGET_LINK_LIBRARIES(pCommunicationAngle_yhhuang
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

