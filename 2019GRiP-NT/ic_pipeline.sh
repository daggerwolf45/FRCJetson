#!/bin/bash

datestr=$(date +%m-%d-%Y_%H:%M:%S)
homedir="/home/nvidia/FRCJetson/2019GRiP"

if [[ ! -d "logs" ]]; then
	mkdir logs
fi

cd ${homedir}
if [ -f ${homedir}/build/ic_pipeline.stop ]; then
	rm -rf ${homedir}/build/ic_pipeline.stop
fi
	cd ${homedir}/build	# Run
	./ic_pipeline >> ${homedir}/logs/ic_pipeline_${datestr}.log 2>&1
