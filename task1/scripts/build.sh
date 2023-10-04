#!/bin/bash

# check that TECHARENA23_TASK1_DIR is set
if [[ -z "$TECHARENA23_TASK1_DIR" ]]; then
    echo "The TECHARENA23_TASK1_DIR bash environment variable is not set."
    echo "To set the TECHARENA23_TASK1_DIR variable, please cd to task1 directory and"
    echo "run the command 'source source.sh'."
    exit 1
fi

# check if docker is installed and running
if ! docker info > /dev/null 2>&1; then
  echo "This script uses docker, and it isn't running - please start docker and try again!"
  exit 1
fi

# check that docker image techarena23_toolchain:latest is installed
if [[ "$(docker images -q techarena23_toolchain:latest 2> /dev/null)" == "" ]]; then
    echo "The techarena23_toolchain docker image is not installed."
    echo "To install the techarena23_toolchain docker, please cd to task1 directory and"
    echo "run the command './scripts/setup_infrastructure.sh'."
    exit 1
fi

# Run the toolchain docker and 
# launch make in the task1 directory
# (using clang-16 and dynamicInstructionCounting 
# library that is required to check the computational 
# resources quota). 
docker \
    run \
    --rm \
    -v $TECHARENA23_TASK1_DIR:/project \
    -w /project \
    techarena23_toolchain \
    /bin/bash -c 'export TECHARENA23_TASK1_DIR=/project; cd /project/task1; make clean; make'
