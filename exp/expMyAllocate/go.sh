#!/bin/bash
expHome=~/workspace/leave
tasksetDir=$expHome/tasksets/origin
targetDir=$expHome/exp/expMyAllocate/result

for dirname in $(ls $tasksetDir)
do
	for tsname in $(ls $tasksetDir/${dirname}/*00 )
	do
		echo $tsname
		./demo $tsname >> $targetDir/$dirname
	done
done

#	./demo ./tasksets/$tsname 
