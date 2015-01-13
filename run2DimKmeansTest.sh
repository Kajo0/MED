#!/bin/bash

if [ $# -lt 2 ]; then
    echo "usage: needs 2 params - baseFileName and 'parameters' [iterations] [treatAsMultiDimensional] eg:"
    echo "\t$0 2dim_Compound '-ke -i 100 -l 0.1 -g 5'"
    exit 1;
fi

name=$1
params=$2
iterations=10
no2dim=0

if [ $# -ge 3 ]; then
    iterations=$3
fi
if [ $# -eq 4 ]; then
    no2dim=1
fi

l="./test/data/${name}-labels.txt"
f="./test/data/${name}.txt"
o="./test/2dim/${name}.data"
r="./test/2dim/${name}.txt"

res=()
maxId=1
maxVal=0

for i in `seq 1 $iterations`; do
    if [ $no2dim -eq 0 ]; then
        res[$i]=`./app -f $f -o ${o}$i -r ${r}$i $params` #> /dev/null
    else
        res[$i]=`./app -f $f -r ${r}$i $params` #> /dev/null
    fi
    val=`./test/rand/randInd.r ${r}$i $l | sed -e 's/\[1\] //g'`
    echo $val

    if [ "$(echo $val '>' $maxVal | bc -l)" -eq 1 ]; then
        maxId=$i
        maxVal=$val
    fi
done

echo
if [ $no2dim -eq 0 ]; then
    echo "./app -f $f -o ${o}$maxId -r ${r}$maxId $params"
else
    echo "./app -f $f -r ${r}$maxId $params"
fi
echo "${res[$maxId]}"
echo "[1] $maxVal"

if [ $no2dim -eq 0 ]; then
    mv -f ${o}$maxId ${o}
fi
mv -f ${r}$maxId ${r}
rm -f ${o}?* ${r}?*

if [ $no2dim -eq 0 ]; then
    ./test/2dim/med2dimChart.r $o
fi

