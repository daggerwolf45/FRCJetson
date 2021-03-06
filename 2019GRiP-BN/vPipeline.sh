#!/bin/bash

datestr=$(date +%m-%d-%Y_%H:%M:%S)
homedir="/home/nvidia/FRCJetson/2019GRiP-BN"

if [[ ! -d "logs" ]]; then
	mkdir logs
fi

cd ${homedir}
if [ -f ${homedir}/build/ic_pipeline.stop ]; then
	rm -rf ${homedir}/build/Pipeline.stop
fi
	cd ${homedir}/build	# Run
	./vPipeline 2>&1 | tee ${homedir}/logs/ic_pipeline_${datestr}.log
