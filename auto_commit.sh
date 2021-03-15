#!/bin/bash
count=0
while true
do
    count=$((count+1))
    git add *
    git commit -m "auto update"
    if [[ $((count%10)) -eq 0 ]]
    then
        git push -u orgin
    fi
    sleep 600
done >git.log