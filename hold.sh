#!/bin/bash

num_secs=${1:-10}
counter=0

while [[ $counter -le $num_secs ]]; do
    counter=$((counter + 1))
    sleep 1
done

