#!/bin/bash

## Entry point to run external tools that prepare the output used by the
## File clock face. Make sure that "fetch/.env" file exists before calling.
## This script could be run from cronjob so that the output updates at the
## desired rate.

# Hold output in a tmp folder
mkdir -p tmp

# Source environment variables
source fetch/.env

# Fetch the data into plain text files
python3 fetch/weather.py > tmp/weather
