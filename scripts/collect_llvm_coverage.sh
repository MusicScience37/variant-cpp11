#!/bin/bash -e

# usage: collect_llvm_coverage.sh <directory of .profraw files>

# ctest must have been called with LLVM_PROFILE_FILE=<build directory>/coverage/coverage_%p.profraw

DIR=$1

# make a file for codecov
llvm-profdata merge -o $DIR/coverage/coverage.profdata $DIR/coverage/coverage_*.profraw
opts="-object $DIR/tests/variant_cpp11_test"
llvm-cov show -ignore-filename-regex='(extern|tests)/*' -instr-profile=$DIR/coverage/coverage.profdata $opts \
    >$DIR/coverage/coverage.txt
llvm-cov show -ignore-filename-regex='(extern|tests)/*' -instr-profile=$DIR/coverage/coverage.profdata $opts \
    -format=html -output-dir=$DIR/coverage/html

# output summary to stdout
llvm-cov report -ignore-filename-regex='(extern|tests)/*' -instr-profile=$DIR/coverage/coverage.profdata $opts |
    tee $DIR/coverage/coverage_summary.txt

# output line coverage for Gitlab CI to get the coverage value
line_cov=$(cat $DIR/coverage/coverage_summary.txt | awk '{ if (NF > 0) { last = $NF } } END { print last }')
echo "Line Coverage: $line_cov"
