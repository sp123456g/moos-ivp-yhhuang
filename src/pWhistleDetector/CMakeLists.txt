#--------------------------------------------------------
# The CMakeLists.txt for:                       pWhistleDetector
# Author(s):                              yhhuang
#--------------------------------------------------------

SET(SRC
  WhistleDetector.cpp
  WhistleDetector_Info.cpp
  STFT.cpp
  main.cpp
)

ADD_EXECUTABLE(pWhistleDetector ${SRC})

TARGET_LINK_LIBRARIES(pWhistleDetector
   ${MOOS_LIBRARIES}
   apputil
   fftw3f
   mbutil
   m
   pthread)

