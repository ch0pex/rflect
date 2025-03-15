#!/bin/sh

perf stat -r 5 build/fluid/fluid 1000 res/large.fld outs/results-large-5.fld
perf stat -r 5 build/fluid/fluid 1000 res/small.fld outs/results-small-5.fld
