#--------------------------------------------------------
# The CMakeLists.txt for:              lib_behaviors-yhhuang
# Author(s):                                
#--------------------------------------------------------

# Set System Specific Libraries
if (${WIN32})
  # Windows Libraries
  SET(SYSTEM_LIBS
      )
else (${WIN32})
  # Linux and Apple Libraries
  SET(SYSTEM_LIBS
      m )
endif (${WIN32})

SET(CMAKE_CXX_FLAGS "-std=c++0x")
MACRO(ADD_BHV BHV_NAME)
  ADD_LIBRARY(${BHV_NAME} SHARED ${BHV_NAME}.cpp)
  TARGET_LINK_LIBRARIES(${BHV_NAME} 
    helmivp
    behaviors 
    ivpbuild 
    logic 
    ivpcore 
    bhvutil 
    mbutil 
    geometry 
    ${SYSTEM_LIBS} )
ENDMACRO(ADD_BHV)


#--------------------------------------------------------
#                                      BHV_SimpleWaypoint
#--------------------------------------------------------
ADD_LIBRARY(BHV_Pulse SHARED 
   BHV_Pulse.cpp)
TARGET_LINK_LIBRARIES(BHV_Pulse
   helmivp
   behaviors 
   ivpbuild 
   logic 
   ivpcore 
   bhvutil 
   mbutil 
   geometry 
   ${SYSTEM_LIBS} )

ADD_LIBRARY(BHV_ZigLeg SHARED 
    BHV_ZigLeg.cpp)
TARGET_LINK_LIBRARIES(BHV_ZigLeg
   helmivp
   behaviors 
   ivpbuild 
   logic 
   ivpcore 
   bhvutil 
   mbutil 
   geometry 
   ${SYSTEM_LIBS} )

ADD_LIBRARY(BHV_SearchFront SHARED 
   BHV_SearchFront.cpp)
TARGET_LINK_LIBRARIES(BHV_SearchFront
   helmivp
   behaviors 
   ivpbuild 
   logic 
   ivpcore 
   bhvutil 
   mbutil
   geometry 
   ${SYSTEM_LIBS} )


ADD_LIBRARY(BHV_SearchFront_Tony SHARED 
   BHV_SearchFront_Tony.cpp AOF_SimpleWaypoint)
TARGET_LINK_LIBRARIES(BHV_SearchFront_Tony
   helmivp
   behaviors 
   ivpbuild 
   logic 
   ivpcore 
   bhvutil 
   mbutil
   geometry 
   ${SYSTEM_LIBS} )

ADD_LIBRARY(BHV_SearchFront_yhh SHARED 
   BHV_SearchFront_yhh.cpp AOF_SimpleWaypoint)
TARGET_LINK_LIBRARIES(BHV_SearchFront_yhh
   helmivp
   behaviors 
   ivpbuild 
   logic 
   ivpcore 
   bhvutil 
   mbutil
   geometry 
   ${SYSTEM_LIBS} )

ADD_LIBRARY(BHV_TrackChirp SHARED 
    BHV_TrackChirp.cpp AOF_SimpleWaypoint)
TARGET_LINK_LIBRARIES(BHV_TrackChirp
   helmivp
   behaviors 
   ivpbuild 
   logic 
   ivpcore 
   bhvutil 
   mbutil
   geometry 
   ${SYSTEM_LIBS} )
