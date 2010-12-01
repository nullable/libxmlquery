#!/bin/bash

echo $@

while getopts :vs opt; do
    if [ $opt == 'v' ]
    then
        v='-v'
        shift $((OPTIND-1))
    elif [ $opt == 's' ]
    then
        s='-s'
        shift $((OPTIND-1))
    fi
done

if [ $# -ne 3 ]
then
    echo "usage: $0 xml_doc query expected_output"
    exit 1
fi

echo "./dom_parser \"$1\" \"$2\""
./dom_parser "$1" "$2" 2>&1 > __tmp.out
if [ $? -ne 0 ]
then
    echo "FAILED"
    exit 1
fi

echo -n "** Running diff..."
diff "$3" __tmp.out 2>&1 > /dev/null
if [ $? -ne 0 ]
then
    echo " FAILED"
    exit 1
else
    echo " PASS"
fi

rm -f __tmp.out

if [ -z $v ]
then
    if [ -z $s ]
    then
        echo "valgrind --leak-check=full --show-reachable=yes ./dom_parser \"$1\" \"$2\""
    fi

    valgrind --leak-check=full --show-reachable=yes ./dom_parser "$1" "$2" 2>&1 | tee __res.out > /dev/null
    grep "All heap blocks were freed -- no leaks are possible" __res.out 2>&1 > /dev/null
fi

if [ $? -ne 0 ]
then
    if [ -z $s ]
    then
        echo "This test has memory leaks"
    fi
    exit 1
fi

rm -f __res.out
if [ -z $s ]
then
    echo "Success!"
fi

