#!/bin/bash 

DATE_TIME=$(date +%Y_%m%d_%H%M)
NEW_FOLDER="Real_field_Data_Shore/$DATE_TIME"

mkdir -p $NEW_FOLDER
mv  LOG_* $NEW_FOLDER
mv  Data_from_scp $NEW_FOLDER
rm -f ./Spectrogram_data/final_P
rm -f ./Spectrogram_data/original_P

rm -f   *~
rm -f   targ_*
rm -f   .LastOpenedMOOSLogDirectory

