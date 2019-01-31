#!/bin/bash

echo "Initiating broadcast."
echo "Cleaning up old shows..."
xargs rm < schedule.txt
rm schedule.txt

echo "Fetching today's schedule..."
wget http://192.168.0.45:5000/scheduleS.txt

echo "Downloading programs..." 
wget -i scheduleS.txt

python extractMp3.py
rm scheduleS.txt

