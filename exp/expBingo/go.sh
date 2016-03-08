#!/bin/bash
expHome=~/workspace/leave
tasksetDir=$expHome/tasksets/origin
targetDir=$expHome/exp/expBingo/result

for tsname in $(ls $tasksetDir/u_100)
do
	echo $tsname >> ./result
	./demo $tasksetDir/u_100/$tsname >> ./result
done
