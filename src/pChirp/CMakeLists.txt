#--------------------------------------------------------
# The CMakeLists.txt for:                       pChirp
# Author(s):                              hs
#--------------------------------------------------------

SET(SRC
  Chirp.cpp
  Chirp_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pChirp ${SRC})

TARGET_LINK_LIBRARIES(pChirp
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

