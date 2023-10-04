#!/bin/bash

cd $TECHARENA23_TASK1_DIR

INPUT_TAR="$1"

if [ -d "$TECHARENA23_TASK1_DIR/task1/PredictionAlgorithm" ]; then 
    mv $TECHARENA23_TASK1_DIR/task1/PredictionAlgorithm $TECHARENA23_TASK1_DIR/task1/PredictionAlgorithm.old
fi

mkdir -p $TECHARENA23_TASK1_DIR/task1/PredictionAlgorithm && tar --strip-components 1 -xvf $INPUT_TAR -C $TECHARENA23_TASK1_DIR/task1/PredictionAlgorithm

$TECHARENA23_TASK1_DIR/task1/scripts/build.sh >> build.log
$TECHARENA23_TASK1_DIR/task1/scripts/evaluate.sh -r routes/route.txt >> result.log 
