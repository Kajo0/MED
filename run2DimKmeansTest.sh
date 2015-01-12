#!/bin/bash

if [ $# -lt 2 ]; then
    echo "usage: needs 2 params - baseFileName and 'parameters' [iterations] eg:"
    echo "\t$0 2dim_Compound '-ke -i 100 -l 0.1 -g 5'"
    exit 1;
fi

name=$1
params=$2
iterations=10

if [ $# -eq 3 ]; then
    iterations=$3
fi

l="./test/data/${name}-labels.txt"
f="./test/data/${name}.txt"
o="./test/2dim/${name}.data"
r="./test/2dim/${name}.txt"

res=()
maxId=1
maxVal=0

for i in `seq 1 $iterations`; do
    res[$i]=`./app -f $f -o ${o}$i -r ${r}$i $params` #> /dev/null
    val=`./test/rand/randInd.r ${r}$i $l | sed -e 's/\[1\] //g'`
    echo $val

    if [ "$(echo $val '>' $maxVal | bc -l)" -eq 1 ]; then
        maxId=$i
        maxVal=$val
    fi
done

echo
echo "./app -f $f -o ${o}$i -r ${r}$i $params"
echo "${res[$maxId]}"
echo "[1] $maxVal"

mv -f ${o}$maxId ${o}
mv -f ${r}$maxId ${r}
rm -f ${o}?* ${r}?*

./test/2dim/med2dimChart.r $o

