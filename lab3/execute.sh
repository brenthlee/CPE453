#!/bin/bash

if [ $# -ge 1 ]; then
    make clean
    make
    echo $1 | ./create
    echo -e ""
    ./sequence
    ./thread
else
   echo Usage: [int seed]
fi
