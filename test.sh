#!/bin/bash

if [ $# -ne 2 ]
then
    echo "usage: $0 xml_doc query"
    exit 1
fi

echo "./dom_parser $1 $2"
./dom_parser $1 $2 2>&1 > /dev/null
if [ $? -ne 0 ]
then
    echo "FAILED"
    exit 1
fi

echo "valgrind --leak-check=full --show-reachable=yes ./dom_parser $1 $2 &> __res.out"
valgrind --leak-check=full --show-reachable=yes ./dom_parser "$1" "$2" 2>&1 | tee __res.out > /dev/null
grep "All heap blocks were freed -- no leaks are possible" __res.out 2>&1 > /dev/null

if [ $? -ne 0 ]
then
    echo "This test has memory leaks"
    exit 1
fi

rm -f __res.out 
echo "Success!"
