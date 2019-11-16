#!/bin/sh

Start_line=10
End_line=11
Actuall_line=$Start_line
Filename=tests/AND_condition_conclusion

source tests/include.sh
if ! [ -x "$(command -v bin/expert_system)" ];
then
    echo "Executable not found, Buildind Executable";
    (cd build && make);
fi

echo "----- First Test (=DEIJOP with ?AFKP, AFKP is true) ----"
do_comment $Filename $Start_line $End_line
do_uncomment $Filename $Actuall_line
./bin/expert_system $Filename
((Actuall_line++))

echo "----- Second Test (=DEIJP with ?AFKP, AFP is true, K is false) ----"
do_comment $Filename $Start_line $End_line
do_uncomment $Filename $Actuall_line
./bin/expert_system $Filename
((Actuall_line++))