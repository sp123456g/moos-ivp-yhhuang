#--------------------------------------------------------
# The CMakeLists.txt for:                       pAmbientCalculate
# Author(s):                              yhhuang
#--------------------------------------------------------

SET(SRC
  AmbientCalculate.cpp
  AmbientCalculate_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pAmbientCalculate ${SRC})

TARGET_LINK_LIBRARIES(pAmbientCalculate
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

