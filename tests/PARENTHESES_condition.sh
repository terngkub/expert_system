#!/bin/sh

Start_line=4
End_line=15
Actuall_line=$Start_line
Filename=tests/PARENTHESES_condition

source tests/include.sh
if ! [ -x "$(command -v bin/expert_system)" ];
then
    echo "Executable not found, Buildind Executable";
    (cd build && make);
fi

echo "----- First Test (= with ?E, E should be false) ----"
do_comment $Filename $Start_line $End_line
do_uncomment $Filename $Actuall_line
./bin/expert_system $Filename
((Actuall_line++))

echo "----- Second Test (=E with ?E, E should be true) ----"
do_comment $Filename $Start_line $End_line
do_uncomment $Filename $Actuall_line
./bin/expert_system $Filename
((Actuall_line++))

echo "----- Third Test (=B with ?E, E should be false) ----"
do_comment $Filename $Start_line $End_line
do_uncomment $Filename $Actuall_line
./bin/expert_system $Filename
((Actuall_line++))

echo "----- Fourth Test (=C with ?E, E should be true) ----"
do_comment $Filename $Start_line $End_line
do_uncomment $Filename $Actuall_line
./bin/expert_system $Filename
((Actuall_line++))

echo "----- First Test (=AC with ?E, E should be false) ----"
do_comment $Filename $Start_line $End_line
do_uncomment $Filename $Actuall_line
./bin/expert_system $Filename
((Actuall_line++))

echo "----- First Test (=CB with ?E, E should be true) ----"
do_comment $Filename $Start_line $End_line
do_uncomment $Filename $Actuall_line
./bin/expert_system $Filename
((Actuall_line++))

echo "----- First Test (=F with ?E, E should be false) ----"
do_comment $Filename $Start_line $End_line
do_uncomment $Filename $Actuall_line
./bin/expert_system $Filename
((Actuall_line++))

echo "----- First Test (=G with ?E, E should be false) ----"
do_comment $Filename $Start_line $End_line
do_uncomment $Filename $Actuall_line
./bin/expert_system $Filename
((Actuall_line++))

echo "----- First Test (=H with ?E, E should be false) ----"
do_comment $Filename $Start_line $End_line
do_uncomment $Filename $Actuall_line
./bin/expert_system $Filename
((Actuall_line++))

echo "----- First Test (=FH with ?E, E should be true) ----"
do_comment $Filename $Start_line $End_line
do_uncomment $Filename $Actuall_line
./bin/expert_system $Filename
((Actuall_line++))

echo "----- First Test (=GH with ?E, E should be true) ----"
do_comment $Filename $Start_line $End_line
do_uncomment $Filename $Actuall_line
./bin/expert_system $Filename
((Actuall_line++))