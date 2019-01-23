#--------------------------------------------------------
# The CMakeLists.txt for:                       pStoreSoundX_TX1
# Author(s):                              yhhuang
#--------------------------------------------------------

SET(SRC
  StoreSoundX_TX1.cpp
  StoreSoundX_TX1_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pStoreSoundX_TX1 ${SRC})

TARGET_LINK_LIBRARIES(pStoreSoundX_TX1
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread
   asound)

