#!/bin/bash

set -e

mkdir -p _build/linux

cmake -S . -B _build/linux/ \
	-Dcm_plat=linux 

cmake --build _build/linux/

