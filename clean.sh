#!/bin/bash

rm -rf build/*
rm -rf lib/*
rm -rf bin/p*
rm -rf bin/e*
rm -rf bin/u*

find . -name '.DS_Store'  -print -exec rm -rfv {} \;
find . -name '*~'  -print -exec rm -rfv {} \;
find . -name '#*'  -print -exec rm -rfv {} \;
find . -name '*.moos++'  -print -exec rm -rfv {} \;

touch lib/README.txt
touch build/README.txt
#find . -name 'MOOSLog*'  -print -exec rm -rfv {} \;

