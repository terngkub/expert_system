#!/usr/bin/env

get_char()
{
    cat tests/AND_conclusion | sed -n $1p | head -c 1
}

do_comment()
{
    if ! [ $(get_char $1) == "#" ]
    then
        to_replace=$(get_char $1)
        echo $to_replace
        sed -e "s/^$to_replace/#/" somefile
        #cat tests/AND_conclusion | sed -n $1p | head -c 1 | sed -e 's/^/#/'
    fi
}