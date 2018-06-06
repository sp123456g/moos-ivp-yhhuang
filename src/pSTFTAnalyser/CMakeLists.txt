#--------------------------------------------------------
# The CMakeLists.txt for:                       pSTFTAnalyser
# Author(s):                              yhhuang
#--------------------------------------------------------

SET(SRC
  STFTAnalyser.cpp
  STFTAnalyser_Info.cpp
  STFT.cpp
  main.cpp
)

ADD_EXECUTABLE(pSTFTAnalyser ${SRC})

TARGET_LINK_LIBRARIES(pSTFTAnalyser
   ${MOOS_LIBRARIES}
   apputil
   armadillo
   mbutil
   m
   pthread)

