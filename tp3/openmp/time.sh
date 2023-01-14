#!/bin/bash

SAMPLES=10000000
CLUSTERS=4

# time ./bin/k_means $SAMPLES $CLUSTERS

# time ./bin/k_means $SAMPLES $CLUSTERS 2

# time ./bin/k_means $SAMPLES $CLUSTERS 4

time ./bin/k_means $SAMPLES $CLUSTERS 8

# time ./bin/k_means $SAMPLES $CLUSTERS 16

# time ./bin/k_means $SAMPLES $CLUSTERS 32

# CLUSTERS=32
# 
# time ./bin/k_means $SAMPLES $CLUSTERS
# 
# time ./bin/k_means $SAMPLES $CLUSTERS 2
# 
# time ./bin/k_means $SAMPLES $CLUSTERS 4
# 
# time ./bin/k_means $SAMPLES $CLUSTERS 8
# 
# time ./bin/k_means $SAMPLES $CLUSTERS 16
# 
# time ./bin/k_means $SAMPLES $CLUSTERS 32