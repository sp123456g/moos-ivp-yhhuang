#!/bin/bash

rm -rf MOOSLog_* .LastOpenedMOOSLog*
rm -rf record
rm -rf ./Spectrogram_data/final_P
rm -rf ./Spectrogram_data/original_P
mv ./Spectrogram_figure/*.fig ~/LogFile_Thesis/figure
mv ./Spectrogram_figure/*.png ~/LogFile_Thesis/figure
