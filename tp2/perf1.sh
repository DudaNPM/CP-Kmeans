#!/bin/bash



perf record make runpar
perf report -n --stdio > perfreport
