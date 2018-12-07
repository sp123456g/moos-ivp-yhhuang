#--------------------------------------------------------
# The CMakeLists.txt for:                       pDirectionCalculator
# Author(s):                              
#--------------------------------------------------------

SET(SRC
  DirectionCalculator.cpp
  DirectionCalculator_Info.cpp
  filt.cpp
  main.cpp
)

ADD_EXECUTABLE(pDirectionCalculator ${SRC})

TARGET_LINK_LIBRARIES(pDirectionCalculator
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

