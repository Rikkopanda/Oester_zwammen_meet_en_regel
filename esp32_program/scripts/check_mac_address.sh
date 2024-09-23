#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <MAC_ADDRESS>"
    exit 1
fi

DESIRED_MAC=$1

# List all connected serial ports
COMPORTS=$(pio device list | grep -oE '/dev/ttyUSB[0-9]+')

# Iterate over each port
for COMPORT in $COMPORTS; do
  # Read MAC address from the port
  MAC=$(esptool.py -p $COMPORT read_mac | grep 'MAC' | head -n 1 | awk '{print $2}')
  MACS+=$MAC
  # Compare MAC address
  if [ "$MAC" == "$DESIRED_MAC" ]; then
    echo $COMPORT
    exit 0
  fi
done

echo "Error: Device with MAC address $DESIRED_MAC not found, found instead:"

echo $MACS

exit 1
