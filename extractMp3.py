import os
import subprocess

file = open("scheduleS.txt", 'r')
output = open("schedule.txt", 'w')

for line in file:
	extract = os.path.basename(os.path.normpath(line))
	newExtract = "32-" + extract
	subprocess.call(['lame', '-b32', '-a', '--resample', '22.05', extract[:-1], newExtract[:-1]])
	output.write(newExtract)
	subprocess.call(['rm', extract[:-1]])
	
