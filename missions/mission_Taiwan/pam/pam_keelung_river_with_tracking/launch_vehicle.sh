#!/bin/bash
TIME_WARP=1
JUST_MAKE="no"
SHORE_IP="192.168.66.15"
SHORE_LISTEN="9300"

HELP="no"
JUST_BUILD="no"
VTEAM=""
VNAME=""
VMODEL="M300"

#strat point
START_POS="-18,44"
#return point
RETURN_POS="39,2"
LOITER_POS="x=100,y=-180"
GRAB_POS=""
UNTAG_POS=""
#------------
CLOCKWISE=1
#------------

#find behavior path
export IVP_BEHAVIOR_DIRS=../../../../lib
#-------------------------------------------------------
#  Part 1: Check for and handle command-line arguments
#-------------------------------------------------------
for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
        HELP="yes"
    elif [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$TIME_WARP" = 1 ]; then
        TIME_WARP=$ARGI
    elif [ "${ARGI}" = "--kirk" -o "${ARGI}" = "-k" ] ; then
        M200_IP=192.168.12.1 #luke
        VNAME="kirk"
	VMODEL="M300"
        VPORT="9012"
        SHARE_LISTEN="9312"
        echo "KIRK vehicle selected."
    elif [ "${ARGI}" = "--money" -o "${ARGI}" = "-m" ] ; then
        M200_IP=192.168.1.134 #UAL_Heron
        VNAME="UAL_Heron"
	VMODEL="M300"
        VPORT="9013"
        SHARE_LISTEN="9313"
        echo "UAL_Heron vehicle selected"
    elif [ "${ARGI}" = "--nostromo" -o "${ARGI}" = "-n" ] ; then
        VNAME="nostromo"
	VMODEL="kayak"
        VPORT="9014"
        SHARE_LISTEN="9314"
	FRONT_SEAT_IP="192.168.1.103"
	FRONT_SEAT_SHARE="9414"
        echo "Nostromo vehicle selected."
    elif [ "${ARGI}" = "--kestrel" -o "${ARGI}" = "-ke" ] ; then
        VNAME="kestrel"
	VMODEL="kayak"
        VPORT="9015"
        SHARE_LISTEN="9315"
	FRONT_SEAT_IP="192.168.1.105"
	FRONT_SEAT_SHARE="9415"
        echo "Kestrel vehicle selected."
    elif [ "${ARGI}" = "--just_build" -o "${ARGI}" = "-j" ] ; then
        JUST_BUILD="yes"
        echo "Just building files; no vehicle launch."
    elif [ "${ARGI}" = "--sim" -o "${ARGI}" = "-s" ] ; then
        SIM="SIM"
        echo "Simulation mode ON."
    elif [ "${ARGI:0:10}" = "--start-x=" ] ; then
        START_POS_X="${ARGI#--start-x=*}"
    elif [ "${ARGI:0:10}" = "--start-y=" ] ; then
        START_POS_Y="${ARGI#--start-y=*}"
    elif [ "${ARGI:0:10}" = "--start-a=" ] ; then
        START_POS_A="${ARGI#--start-a=*}"
    #----------------------------------------
    elif [ "${ARGI}" = "--first" -o "${ARGI}" = "-1" ] ; then
        BHV_FILE="meta_vehicle1.bhv"
        echo "doing first bhv file "
    elif [ "${ARGI}" = "--second" -o "${ARGI}" = "-2" ] ; then
        BHV_FILE="meta_vehicle2.bhv"
        echo "doing second bhv file"
    #----------------------------------------
    else
        echo "Undefined argument:" $ARGI
        echo "Please use -h for help."
        exit 1
    fi
done

#-------------------------------------------------------
#  Part 2: Handle Ill-formed command-line arguments
#-------------------------------------------------------

if [ "${HELP}" = "yes" ]; then
    echo "$0 [SWITCHES]"
    echo "  --UAL_Heron,  -m  : UAL_Heron vehicle."
    echo "  --sim,        -s  : Simulation mode."
    echo "  --start-x=        : Start from x position (requires x y a)."
    echo "  --start-y=        : Start from y position (requires x y a)."
    echo "  --start-a=        : Start from angle (requires x y a)."
    echo "  --first,          : do meta_vehicle1.bhv"
    echo "  --second,         : do meta_vehicle2.bhv"
    echo "  --just_build, -j"
    echo "  --help, -h"
    exit 0;
fi

if [ -z $VNAME ]; then
    echo "No vehicle has been selected..."
    echo "Exiting."
    exit 2
fi


if [ -z $BHV_FILE ]; then
    echo "No .bhv file has been selected..."
    echo "Exiting."
    exit 2
fi


#-------------------------------------------------------
#  Part 3: Create the .moos and .bhv files.
#-------------------------------------------------------

if [[ -n $START_POS_X && (-n $START_POS_Y && -n $START_POS_A)]]; then
  START_POS="$START_POS_X,$START_POS_Y,$START_POS_A"
  echo "Starting from " $START_POS
elif [[ -z $START_POS_X && (-z $START_POS_Y && -z $START_POS_A) ]]; then
  echo "Starting from default postion: " $START_POS
else [[ -z $START_POS_X || (-z $START_POS_Y || -z $START_POS_A) ]]
  echo "When specifing a strating coordinate, all 3 should be specified (x,y,a)."
  echo "See help (-h)."
  exit 1
fi

echo "Assembling MOOS file targ_${VNAME}.moos"


nsplug meta_vehicle.moos targ_${VNAME}.moos -f \
    VNAME=$VNAME                 \
    VPORT=$VPORT                 \
    WARP=$TIME_WARP              \
    SHARE_LISTEN=$SHARE_LISTEN   \
    SHORE_LISTEN=$SHORE_LISTEN   \
    FRONT_SEAT_IP=$FRONT_SEAT_IP \
    FRONT_SEAT_SHARE=$FRONT_SEAT_SHARE \
    SHORE_IP=$SHORE_IP           \
    M200_IP=$M200_IP             \
    LOITER_POS=$LOITER_POS       \
    VARIATION=$VARIATION         \
    VMODEL=$VMODEL                \
    VTYPE="kayak"                \
    VTEAM="blue"                 \
    START_POS=$START_POS         \
    $SIM                        \
    
echo "Assembling BHV file targ_${VNAME}.bhv"
nsplug ${BHV_FILE} targ_${VNAME}.bhv -f  \
        RETURN_POS=${RETURN_POS}    \
        TRAIL_RANGE=$TRAIL_RANGE    \
        TRAIL_ANGLE=$TRAIL_ANGLE    \
        VTEAM=$VTEAM                \
        VNAME=$VNAME                \
        GRAB_POS=$GRAB_POS          \
        UNTAG_POS=$UNTAG_POS        \
        SURVEY_X=$SURVEY_X          \
        SURVEY_Y=$SURVEY_Y          \
        HEIGHT=$HEIGHT1             \
        WIDTH=$WIDTH1               \
        LANE_WIDTH=$LANE_WIDTH1     \
        DEGREES=$DEGREES1           \
        START_POS=$START_POS        \
        CLOCKWISE=$CLOCKWISE

if [ ${JUST_BUILD} = "yes" ] ; then
    echo "Files assembled; vehicle not launched; exiting per request."
    exit 0
fi

#-------------------------------------------------------
#  Part 4: Launch the processes
#-------------------------------------------------------

echo "Launching $VNAME MOOS Community "
pAntler targ_${VNAME}.moos >& /dev/null &
uMAC targ_${VNAME}.moos

echo "Killing all processes ..."
kill -- -$$
echo "Done killing processes."
