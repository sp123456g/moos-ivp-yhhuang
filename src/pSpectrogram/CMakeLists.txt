#--------------------------------------------------------
# The CMakeLists.txt for:                       pSpectrogram
# Author(s):                              yhhuang
#--------------------------------------------------------

SET(SRC
  Spectrogram.cpp
  Spectrogram_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pSpectrogram ${SRC})

TARGET_LINK_LIBRARIES(pSpectrogram
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

