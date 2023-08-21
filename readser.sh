#!/usr/bin/bash
set -euo pipefail

usage="$(basename $0) <SERIAL DEVICE> [<END MARKER>]"

# Ensure that a serial device was specified
if [[ -z "${1:-}" ]]; then
	echo -e "\nUsage: $usage\n"
	echo "Exiting"
	exit 1
fi

# Warn the user if an end marker wasn't specified
if [[ -z "${2:-}" ]]; then
	echo -e "\nWARNING: An end marker was not specified (see usage below); using 'done'"
	echo -e "Usage: $usage\n"
fi

# Serial device to read from
ser=$1
# Marker that indicates when the script should stop reading input from the serial device
# If not specified, use "done" as the end marker
endmarker=${2:-done}

# Clear out the contents of the serial device right away to avoid reading a false end marker immediately
echo "-------------------------"
echo "Current contents of $ser:"
cat $ser
echo -e "\n-------------------------"

end=0
while [[ $end -eq 0 ]]; do
	out=$(cat $ser)
	if [[ "${out,,}" =~ $endmarker ]]; then
		echo -e "\nRead '${out,,}', which contains '$endmarker'\n\nExiting\n"
		end=1
	else
		echo -e "$out"
		sleep 1
	fi
done

