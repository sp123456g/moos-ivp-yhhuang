#--------------------------------------------------------
# The CMakeLists.txt for:                       pPrimeFactor
# Author(s):                              YH_Huang
#--------------------------------------------------------

SET(SRC
  PrimeFactor.cpp
  PrimeFactor_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pPrimeFactor ${SRC})

TARGET_LINK_LIBRARIES(pPrimeFactor
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

