#!/bin/bash
expHome=~/workspace/leave
tasksetDir=$expHome/tasksets/origin
targetDir=$expHome/exp/exp2/result

for dirname in $(ls $tasksetDir)
do
	for tsname in $(ls $tasksetDir/${dirname}/)
	do
		echo $tsname
		./demo $tasksetDir/${dirname}/$tsname >> $targetDir/$dirname
	done
done

#	./demo ./tasksets/$tsname 
