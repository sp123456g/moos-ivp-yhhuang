#--------------------------------------------------------
# The CMakeLists.txt for:                       pAISdecoder
# Author(s):                                    YHHuang
# DATE: MAR 19th 2018                             */
#--------------------------------------------------------

SET(SRC
  AISdecoder.cpp
  AISdecoder_Info.cpp
  AisNode.cpp
  main.cpp
)

ADD_EXECUTABLE(pAISdecoder ${SRC})

TARGET_LINK_LIBRARIES(pAISdecoder
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

