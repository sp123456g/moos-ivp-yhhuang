#--------------------------------------------------------
# The CMakeLists.txt for:                       pStoreSoundX
# Author(s):                              CTHung
#--------------------------------------------------------

SET(SRC
  StoreSoundX.cpp
  StoreSoundX_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pStoreSoundX ${SRC})

TARGET_LINK_LIBRARIES(pStoreSoundX
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread
   asound)

