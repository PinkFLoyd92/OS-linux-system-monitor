#!/bin/bash

times=0
echo -n "Enter number of processes you want > "
read times
for i in `seq 2 $(($times+1))`
do
    rand=$(shuf -i 30-200 -n 1)
    ./processes/cpu_hungry $rand & 
    ./processes/mem_hungry $rand &
done

