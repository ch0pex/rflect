#!/bin/sh

perf stat -a -r 5 -e 'energy-cores, energy-gpu,energy-pkg,energy-ram' build/fluid/fluid 250 res/small.fld outs/small-250.fld
perf stat -a -r 5 -e 'energy-cores, energy-gpu,energy-pkg,energy-ram' build/fluid/fluid 500 res/small.fld outs/small-500.fld
perf stat -a -r 5 -e 'energy-cores, energy-gpu,energy-pkg,energy-ram' build/fluid/fluid 750 res/small.fld outs/small-750.fld
perf stat -a -r 5 -e 'energy-cores, energy-gpu,energy-pkg,energy-ram' build/fluid/fluid 1000 res/small.fld outs/small-1000.fld
perf stat -a -r 5 -e 'energy-cores, energy-gpu,energy-pkg,energy-ram' build/fluid/fluid 1250 res/small.fld outs/small-1250.fld
perf stat -a -r 5 -e 'energy-cores, energy-gpu,energy-pkg,energy-ram' build/fluid/fluid 1500 res/small.fld outs/small-1500.fld
perf stat -a -r 5 -e 'energy-cores, energy-gpu,energy-pkg,energy-ram' build/fluid/fluid 1750 res/small.fld outs/small-1750.fld
perf stat -a -r 5 -e 'energy-cores, energy-gpu,energy-pkg,energy-ram' build/fluid/fluid 2000 res/small.fld outs/small-2000.fld
perf stat -a -r 5 -e 'energy-cores, energy-gpu,energy-pkg,energy-ram' build/fluid/fluid 2250 res/small.fld outs/small-2250.fld
perf stat -a -r 5 -e 'energy-cores, energy-gpu,energy-pkg,energy-ram' build/fluid/fluid 2500 res/small.fld outs/small-2500.fld
