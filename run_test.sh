#!/bin/sh
execute_ES()
{
    for file in $1
    do
        echo ""
        echo "=========================================="
        echo "$file"
        echo ""
        echo $file
        
        echo ""
        echo "------------------------------------------"
        echo ""
    done
}


if [ -z "$1"]
then
    echo "/!\\ doing Simple test first /!\\"
    folder="tests/Simple_test/**"
    execute_ES $folder $2
    echo "/!\\ Next, Parser Test, press Enter /!\\"
    read tmp
    folder="tests/parser_test"
    execute_ES $folder $2
    echo "/!\\ Next, Correction Test, press Enter /!\\"
    read tmp
    folder="tests/Correction_test"
    execute_ES $folder $2
fi
if [ ! -z "$1" ]
then
    folder=$1
    execute_ES $folder $2
fi