#!/bin/bash

set -e

cmake -S . -B _build/
cmake --build _build/
