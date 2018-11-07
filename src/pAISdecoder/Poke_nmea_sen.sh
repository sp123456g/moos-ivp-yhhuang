#!/bin/bash
filename='nmea-sample-2'
exec < $filename

while read line
do
        echo $line
        uPokeDB pAISdecoder.moos AIS_DATA="$line"
        done
