#!/bin/bash

while [[ 1 ]]; do
    hold_cmd_cnt=0
    while [[ $hold_cmd_cnt -eq 0 ]]; do
        hold_cmd_cnt=$(ps -ef | grep -E "bash $HOME/\.local/bin/hold" | wc -l)
        sleep 0.25
    done

    # TODO Change this to sleep for 15 or so seconds and then reboot
    # TODO Better yet, have this read the argument passed to hold (10 is the default), and sleep for that + 10 seconds to give the CI job time to complete
    shutdown -r +1

    # Use another loop to continuously verify that a shutdown is still scheduled.
    # If not, break out of the second loop and wait for the hold command to be called again
    while [[ -n "$(ls /run/systemd/shutdown)" ]]; do
        sleep 0.25
    done
done

