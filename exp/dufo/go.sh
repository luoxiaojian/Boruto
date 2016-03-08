#!/bin/bash
expHome=~/workspace/leave
tasksetDir=$expHome/tasksets/modified
targetDir=$expHome/exp/dufo/result

for dirname in $(ls $tasksetDir)
do
	for tsname in $(ls $tasksetDir/${dirname}/* )
	do
		./algo $tsname >> $targetDir/$dirname
		if [ $? = 0 ]; then
			echo $tsname "successful schedule"
		else
			echo $tsname "error"
		fi
	done
done

#	./demo ./tasksets/$tsname 
