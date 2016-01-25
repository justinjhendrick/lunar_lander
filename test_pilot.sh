#!/bin/bash

# test the autopilot by running it a bunch
# if it fails, save stdout from that run (it contains the random seed)

mkdir test_results
i=0;
while true; do
    ./lunar_lander -c >test_results/out.txt;
    grep 'lose' test_results/out.txt;
    if [ "$?" -eq 0 ]; then
        mv test_results/out.txt test_results/fail_"$i".txt;
    fi
    ((i++))
done
