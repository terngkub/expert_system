#!/bin/sh

Start_line=5
End_line=6
Actuall_line=$Start_line
Filename=tests/AND_condition

source tests/include.sh
if ! [ -x "$(command -v bin/expert_system)" ];
then
    echo "Executable not found, Buildind Executable";
    (cd build && make);
fi

echo "----- First Test (=A with ?ABCD, Everything is true) ----"
do_comment $Filename $Start_line $End_line
do_uncomment $Filename $Actuall_line
./bin/expert_system $Filename
((Actuall_line++))

echo "----- Second Test (=B with ?ABCD, verything is False except B) ----"
do_comment $Filename $Start_line $End_line
do_uncomment $Filename $Actuall_line
./bin/expert_system $Filename
((Actuall_line++))