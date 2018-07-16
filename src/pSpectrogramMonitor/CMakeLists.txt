#--------------------------------------------------------
# The CMakeLists.txt for:                       pSpectrogramMonitor
# Author(s):                              yhhuang
#--------------------------------------------------------

SET(SRC
  SpectrogramMonitor.cpp
  SpectrogramMonitor_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pSpectrogramMonitor ${SRC})

TARGET_LINK_LIBRARIES(pSpectrogramMonitor
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   asound
   fftw3f
   glut
   GL
   m
   pthread)

