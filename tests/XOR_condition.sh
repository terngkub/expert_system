#!/bin/sh

Start_line=5
End_line=8
Actuall_line=$Start_line
Filename=tests/XOR_condition

source tests/include.sh
if ! [ -x "$(command -v bin/expert_system)" ];
then
    echo "Executable not found, Buildind Executable";
    (cd build && make);
fi

echo "----- First Test (= with ?A, A should be false) ----"
do_comment $Filename $Start_line $End_line
do_uncomment $Filename $Actuall_line
./bin/expert_system $Filename
((Actuall_line++))

echo "----- Second Test (=D with ?A, A should be true) ----"
do_comment $Filename $Start_line $End_line
do_uncomment $Filename $Actuall_line
./bin/expert_system $Filename
((Actuall_line++))

echo "----- Third Test (=E with ?A, A should be true) ----"
do_comment $Filename $Start_line $End_line
do_uncomment $Filename $Actuall_line
./bin/expert_system $Filename
((Actuall_line++))

echo "----- Fourth Test (=DE with ?A, A should be true) ----"
do_comment $Filename $Start_line $End_line
do_uncomment $Filename $Actuall_line
./bin/expert_system $Filename
((Actuall_line++))