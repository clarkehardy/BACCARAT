#!/bin/bash

g++ -g -o LSDetectorHitSelection `root-config --cflags --libs` -L/g/g92/hardy27/BACCARAT/tools -lBaccRootConverterEvent LSDetectorHitSelection.cc
export LD_LIBRARY_PATH=/g/g92/hardy27/BACCARAT/tools:$LD_LIBRARY_PATH
#cp ./BACCARAT/tools/libBaccRootConverterEvent.so ./
