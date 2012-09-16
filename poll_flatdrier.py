#!/usr/bin/env python
"""
Copyright (c) 2012, Angus Gratton. Licensed under the Modified BSD License.

A python script to poll the Arduino running flatdrier and append the
data to a CSV file. Also switches on/off the dehumidifier. Designed to
be run from cron at one minute intervals.

Output file directory is specified below in script config section,
filename is the current date as YYYYMMDD.csv.

This script relies on the Arduino having its "reset-on-connect"
functionality disabled (either by removing a cap, cutting a jumper, or
just setting a switch on clones like Seeeduino.) If not, the data
output will always be empty!
"""

############################
# Script config parameters
############################

# Directory to store the CSV files with sensor data
OUTPUT_DIR="/www/flatdrier/data/"

# Serial port to connect to the Arduino
SERIAL="/dev/ttyUSB0"

# Allowable hours to have dehumudifier on. Will always be switched off outside these hours.
ON_HOURS=(9,21)

# A "dew trigger" is one where 'minimum temperature - DEW_THRESHOLD < maximum dewpoint'
DEW_THRESHOLD=2.5

# How many hours to look back for a "dew trigger" (consequently, dehumidifier will stay on for this long at minimum)
LOOKBACK_HOURS=4

# Never switch between on/off within this many minutes (avoid wear on the dehumidifier)
ANTI_HYSTERISIS_MINUTES=30

import datetime,time,os,serial,csv,itertools
from datetime import datetime,timedelta

def get_path(datetime):
    return os.path.join(OUTPUT_DIR, datetime.strftime("%Y%m%d.csv"))

def main():
    s = serial.Serial(SERIAL, 9600, timeout=1)
    write_latest(s)
    set_dehumidifier(s)

def write_latest(s):
    """ Fetch the latest sample from the Arduino and write it to a CSV file"""
    now = time.time()
    with open(get_path(datetime.utcnow()), "a") as f:
        while s.inWaiting(): # flush anything in the read buffer
            s.read()
        s.write("r")
        try:
            resp = s.readline().strip()
        except serial.SerialTimeoutException:
            return # something went wrong, try again next time?
        f.write("%d,%s\n" % (now,resp))


def set_dehumidifier(s):
    """ Load recent samples and determine if the dehumidifier should be switched on or off """
    now = time.time()
    samples = []
    # read all the samples for the past 2 days (LOOKBACK_HOURS may span two days' worth of files)
    for path in [ get_path(datetime.utcnow()-timedelta(days=days)) for days in [1,0] ]:
        with open(path, "r") as f:
            samples += [parse_sample(line) for line in csv.reader(f, delimiter=',')]
    # work out if the dehumidifier should be on
    now = datetime.now()
    in_hours = now.hour >= ON_HOURS[0] and now.hour < ON_HOURS[1]
    should_be_on = in_hours and any(dehumidifier_should_be_on(r) for r in samples
                                             if r["ts"] > time.time() - LOOKBACK_HOURS*60*60 ) # last X hours of samples
    try:
        latest = samples[-1]
    except IndexError:
        latest = { "status" : None }
    if latest["status"] != should_be_on:
        # anti-hysterisis check if it's changed lately before switching
        if not any(r["status"]==should_be_on for r in samples if r["ts"] > time.time() - ANTI_HYSTERISIS_MINUTES*60):
            s.write("o" if should_be_on else "f")


def dewpoint(temp,humidity):
    return temp - (100-humidity)/5

def dehumidifier_should_be_on(sample):
    """ Does this sample trigger the dehumidifier to be on? """
    return sample["min_temp"] <= sample["max_dewpoint"]+DEW_THRESHOLD

def parse_sample(resp):
    """ Parse a sample from the relevant fields in the CSV data """
    ts,h0,t0,h1,t1,h2,t2,h3,t3,_,status = resp
    status = { "?" : None, "0": False, "1" : True }[status]
    temps = [ float(t) for t in [t0,t1,t2,t3] ]
    humidity = [ float(h) for h in [h0,h1,h2,h3] ]

    return {
        "ts" : int(ts),
        "status"  : status,
        "min_temp" : min(temps),
        "max_dewpoint" : max((dewpoint(t,h) for t,h in zip(temps, humidity))),
        }


if __name__ == "__main__":
    main()

