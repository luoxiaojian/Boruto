#!/bin/bash
expHome=~/workspace/home/workConserving
tasksetDir=$expHome/tasksets/tasksets4

for dirname in $(ls $tasksetDir)
do
	for tsname in $(ls $tasksetDir/${dirname}/)
	do
		echo $tsname
		./demo $tasksetDir/${dirname}/$tsname
	done
done

#	./demo ./tasksets/$tsname 
