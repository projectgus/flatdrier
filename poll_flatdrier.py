#!/usr/bin/env python
"""
Copyright (c) 2012, Angus Gratton. Licensed under the Modified BSD License.

A python script to poll the Arduino running flatdrier and append the data to a CSV
file. Designed to be run from cron at one minute intervals.

Output file directory is specified below in script config section,
filename is the current date as YYYYMMDD.csv.

This script relies on the Arduino having its "reset-on-connect"
functionality disabled (either by removing a cap, cutting a jumper, or
just setting a switch on clones like Seeeduino.) If not, the data
output will always be empty!
"""

# Script config parameters
OUTPUT_DIR="./data/"
SERIAL="/dev/ttyUSB0"

import datetime,time,os,serial

def main():
    output_path = os.path.join(OUTPUT_DIR, datetime.date.today().strftime("%Y%m%d.csv"))
    with open(output_path, "a") as f:
        with serial.Serial(SERIAL, 9600, timeout=1) as s:
            while s.inWaiting(): # flush anything in the read buffer
                s.read()
            s.write("r")
            try:
                resp = s.readline().strip()
            except serial.SerialTimeoutException:
                return # something went wrong, try again next time?
            f.write("%d,%s\n" % (time.time(),resp))

if __name__ == "__main__":
    main()

