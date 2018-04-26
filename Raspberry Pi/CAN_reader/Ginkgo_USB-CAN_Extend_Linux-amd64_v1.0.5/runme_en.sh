#!/bin/sh
echo $PWD
export LD_LIBRARY_PATH="$PWD:$LD_LIBRARY_PATH"
echo $LD_LIBRARY_PATH
./USB-CAN_Extend_En
