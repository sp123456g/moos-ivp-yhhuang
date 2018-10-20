#--------------------------------------------------------
# The CMakeLists.txt for:                       pResultSaver
# Author(s):                              yhhuang
#--------------------------------------------------------

SET(SRC
  ResultSaver.cpp
  ResultSaver_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pResultSaver ${SRC})

TARGET_LINK_LIBRARIES(pResultSaver
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

