#!/bin/sh


execute_ES()
{
    if [ -d $1 ]
    then
        to_execute="$1/**"
    else
        to_execute=$1
    fi

    for file in $to_execute
    do
        echo ""
        echo "======================================================"
        echo "\033[0;31m $file"
        echo "\033[0m"
        cat $file
        echo ""
        echo "Press Enter to execute file"
        read rdm_var
        echo ""
        ./expert_system $file $2
        echo ""
        echo ""
        echo ""
    done
}


if [ -z $1 ]
then
    echo "/!\\ doing Simple test first /!\\";
    folder="tests/Simple_test";
    execute_ES $folder $2;
    echo "/!\\ Next, Parser Test, press Enter /!\\";
    read tmp;
    folder="tests/parser_test";
    execute_ES $folder $2;
    echo "/!\\ Next, Correction Test, press Enter /!\\";
    read tmp;
    folder="tests/Correction_test";
    execute_ES $folder $2;
else
    folder=$1;
    execute_ES $folder $2;
fi