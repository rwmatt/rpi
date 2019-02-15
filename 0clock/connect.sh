#!/usr/bin/env bash

# connect.sh

# Usage:
# $ connect.sh <device> <port speed>
# Example: connect.sh /dev/ttyS0 9600
DEV=/dev/ttyACM0  # $1
BAUD=9600          # $2
# Set up device
stty -F $DEV $BAUD

# Let cat read the device $1 in the background
cat $DEV &

# Capture PID of background process so it is possible to terminate it when done
bgPid=$!

# Read commands from user, send them to device $1
while read cmd
do
   echo "$cmd" 
done > $DEV

# Terminate background read process
kill $bgPid
