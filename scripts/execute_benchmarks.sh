#!/bin/bash

# usage: execute_benchmarks.sh <build directory>

DIR=$1
BENCHS="assignment get"

for BENCH in $BENCHS; do
    ${DIR}/benchmark/variant_cpp11_bench_$BENCH \
        --benchmark_out=${DIR}/benchmark/variant_cpp11_bench_${BENCH}.json \
        --benchmark_out_format=json
    echo ""
done
