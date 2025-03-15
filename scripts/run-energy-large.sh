#!/bin/sh

perf stat -a -r 5 -e 'energy-cores, energy-gpu,energy-pkg,energy-ram' build/fluid/fluid 250 res/large.fld outs/large-250.fld
perf stat -a -r 5 -e 'energy-cores, energy-gpu,energy-pkg,energy-ram' build/fluid/fluid 500 res/large.fld outs/large-500.fld
perf stat -a -r 5 -e 'energy-cores, energy-gpu,energy-pkg,energy-ram' build/fluid/fluid 750 res/large.fld outs/large-750.fld
perf stat -a -r 5 -e 'energy-cores, energy-gpu,energy-pkg,energy-ram' build/fluid/fluid 1000 res/large.fld outs/large-1000.fld
perf stat -a -r 5 -e 'energy-cores, energy-gpu,energy-pkg,energy-ram' build/fluid/fluid 1250 res/large.fld outs/large-1250.fld
perf stat -a -r 5 -e 'energy-cores, energy-gpu,energy-pkg,energy-ram' build/fluid/fluid 1500 res/large.fld outs/large-1500.fld
perf stat -a -r 5 -e 'energy-cores, energy-gpu,energy-pkg,energy-ram' build/fluid/fluid 1750 res/large.fld outs/large-1750.fld
perf stat -a -r 5 -e 'energy-cores, energy-gpu,energy-pkg,energy-ram' build/fluid/fluid 2000 res/large.fld outs/large-2000.fld
perf stat -a -r 5 -e 'energy-cores, energy-gpu,energy-pkg,energy-ram' build/fluid/fluid 2250 res/large.fld outs/large-2250.fld
perf stat -a -r 5 -e 'energy-cores, energy-gpu,energy-pkg,energy-ram' build/fluid/fluid 2500 res/large.fld outs/large-2500.fld
