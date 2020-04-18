#!/bin/bash

# usage: collect_test_result.sh <directory in which JUnit XMLs exist>

DIR=$1

python3 -m junit2htmlreport $DIR/junit.xml $DIR/test_results.html
