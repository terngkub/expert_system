#!/usr/bin/env

get_line()
{
    cat $1 | sed -n $2p 
}

get_char()
{
    cat $1 | sed -n $2p | head -c 1
}

do_comment()
{
    Start=$2
    End=$3
    while (( $Start <= $End ))
    do
        if ! [ $(get_char $1 $Start) == "#" ]
        then
            to_replace=$(get_char $1 $Start)
            sed -i.bak "${Start}s/.*/#&/" $1
        fi
        ((Start++))
    done
}

do_uncomment()
{
    if [ $(get_char $1 $2) == "#" ]
    then
        to_replace=$(get_char $1 $2)
        sed -i.bak "$2s/^$to_replace//" $1 
    fi
}