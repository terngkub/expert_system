#!/bin/sh


execute_ES()
{

    if [ -d $1 ]
    then
        to_execute="$1/**"
    else
        to_execute=$1
    fi
    to_execute_size=$(echo $to_execute | wc -w)

    for file in $to_execute
    do
        echo ""
        echo "\033[0;32m$file"
        echo "\033[0m"
        cat $file
        echo ""
        ./expert_system $file $2
        echo ""
        echo "Press Enter to run the next test"
        read rdm_var
        echo ""
        echo "\033[38;5;186m======================================================"
        echo "\033[0m"
    done
}


if [ -z $1 ]
then
    echo "/!\\ doing Simple test first /!\\";
    folder="tests/Simple_test";
    execute_ES $folder $2;
    echo "\033[38;5;209m======================================================"
    echo "\033[0m"
    echo "/!\\ Next, Parser Test, press Enter /!\\";
    read tmp;
    folder="tests/parser_test";
    execute_ES $folder $2;
    echo "\033[38;5;209m======================================================"
    echo "\033[0m"
    echo "/!\\ Next, Correction Test, press Enter /!\\";
    read tmp;
    folder="tests/Correction_test";
    execute_ES $folder $2;
    echo "\033[38;5;209m======================================================"
    echo "\033[0m"
else
    folder=$1;
    execute_ES $folder $2;
fi
