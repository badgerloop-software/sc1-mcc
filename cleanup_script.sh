#!/bin/bash

job_triggering_reboot=compile-and-flash
flash_summary_file=$RUNNER_TEMP/flash_summary

if [[ "$GITHUB_JOB" == "$job_triggering_reboot" && "$GITHUB_WORKFLOW" == "test-self-hosted" ]]; then
    if [[ -f $flash_summary_file && "$(cat $flash_summary_file)" =~ [Ss]uccess ]]; then
        echo "Successful flash"
        # Run a script to sleep for 10 seconds. This will trigger a reboot through listen_for_job_end.sh, which runs in the background
        hold 10 &
        # Kill the runner process: pkill -2 Runner.Listener
    else
        echo "Failed flash"
    fi
fi

