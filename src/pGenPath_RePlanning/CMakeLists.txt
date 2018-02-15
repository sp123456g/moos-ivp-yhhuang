#--------------------------------------------------------
# The CMakeLists.txt for:                       pGenPath_RePlanning
# Author(s):                              YHHUANG
#--------------------------------------------------------

SET(SRC
  GenPath_RePlanning.cpp
  GenPath_RePlanning_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pGenPath_RePlanning ${SRC})

TARGET_LINK_LIBRARIES(pGenPath_RePlanning
   ${MOOS_LIBRARIES}
   geometry
   apputil
   mbutil
   m
   pthread)

