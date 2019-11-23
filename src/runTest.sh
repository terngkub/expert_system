#!/bin/bash

if [ -z "$1" ]
then
    folder="tests/**"
else
    folder="tests/$1"
fi

for file in $folder/**
do
    echo ""
    echo "====================================="
    echo ""
    echo $file
    echo ""
    awk '{printf("%2d: %s\n", NR, $0)}' $file
    echo ""
    echo "-------------------------------------"
    echo ""
    ./avm -v $file
done