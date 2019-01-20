#!/bin/bash 

DATE_TIME=$(date +%Y_%m%d_%H%M)
NEW_FOLDER="Real_field_Data_TX1/$DATE_TIME"
mkdir -p $NEW_FOLDER
mv record $NEW_FOLDER
mv LOG_*  $NEW_FOLDER
mv Data_for_scp $NEW_FOLDER


rm -rf  $VERBOSE   LOG_*
rm -f   $VERBOSE   *~
rm -f   $VERBOSE   targ_*
rm -f   $VERBOSE   .LastOpenedMOOSLogDirectory

