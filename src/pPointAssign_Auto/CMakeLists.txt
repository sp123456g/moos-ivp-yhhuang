#--------------------------------------------------------
# The CMakeLists.txt for:                       pPointAssign_Auto
# Author(s):                              YHHuang
#--------------------------------------------------------

SET(SRC
  PointAssign_Auto.cpp
  PointAssign_Auto_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pPointAssign_Auto ${SRC})

TARGET_LINK_LIBRARIES(pPointAssign_Auto
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

