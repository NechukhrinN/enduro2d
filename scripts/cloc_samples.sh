#!/bin/bash
SCRIPT_DIR=`dirname "$BASH_SOURCE"`
cloc \
  $SCRIPT_DIR/../samples/bin \
  $SCRIPT_DIR/../samples/sources
