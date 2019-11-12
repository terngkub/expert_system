#!/usr/bin/env

get_line()
{
    cat tests/AND_conclusion | sed -n $1p 
}

get_char()
{
    cat tests/AND_conclusion | sed -n $1p | head -c 1
}

do_comment()
{
    Start=$1
    End=$2

    while ($Start <= $end)
    do
        if ! [ $(get_char $1) == "#" ]
        then
            to_replace=$(get_char $1)
            sed -e "s/^$to_replace/#/" somefile
        fi
        $Start++
    done
}

do_uncomment()
{
    if [ $(get_char $1) == "#" ]
    then
        ($get_line $1) | sed -r 's/^.{1}//'
    fi
    done
}