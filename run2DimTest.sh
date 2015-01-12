#!/bin/sh

if [ $# -ne 2 ]; then
    echo "usage: needs 2 params - baseFileName and 'parameters' eg:"
    echo "\t$0 2dim_Compound '-de -s 3 -p 1'"
    exit 1;
fi

name=$1
params=$2

l="./test/data/${name}-labels.txt"
f="./test/data/${name}.txt"
o="./test/2dim/${name}.data"
r="./test/2dim/${name}.txt"

set -x

./app -f $f -o $o -r $r $params
./test/rand/randInd.r $r $l
./test/2dim/med2dimChart.r $o

