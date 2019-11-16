#!/bin/sh

Start_line=5
End_line=6
Actuall_line=$Start_line


source tests/include.sh
if ! [ -x "$(command -v bin/expert_system)" ];
then
    echo "Executable not found, Buildind Executable";
    (cd build && make);
fi

echo "----- First Test (=A with ?ABCDE, excepted result here) ----"
do_comment tests/AND_conclusion $Start_line $End_line
do_uncomment tests/AND_conclusion $Actuall_line
./bin/expert_system tests/AND_conclusion
((Actuall_line++))

echo "----- Second Test (=C with ?ABCDE, excepted result here) ----"
do_comment tests/AND_conclusion $Start_line $End_line
do_uncomment tests/AND_conclusion $Actuall_line
./bin/expert_system tests/AND_conclusion
((Actuall_line++))