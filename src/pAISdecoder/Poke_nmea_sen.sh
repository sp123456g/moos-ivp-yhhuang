#!/bin/bash
filename='nmea-sample'
exec < $filename

while read line
do
        echo $line
        uPokeDB pAISdecoder.moos AIS_DATA="$line"
        done
