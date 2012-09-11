#!/bin/bash

shadow_build_dir=.test-suite-shadow-build
project_dir=..


if [ "$1" == "-u" ]; then
  run_unit=yes
  run_e2e=no
  config="CONFIG+=unit_tests"
elif [ "$1" == "-e" ]; then
  run_unit=no
  run_e2e=yes
  config="CONFIG+=end_to_end_tests"
elif [ "$1" == "-a" ]; then
  run_unit=yes
  run_e2e=yes
  config="CONFIG+=unit_tests end_to_end_tests"
else
  echo "Syntax:"
  echo "   $0 -u   # Run unit tests"
  echo "   $0 -e   # Run end to end tests"
  echo "   $0 -a   # Run all tests"
  exit 1
fi

set -e

mkdir -p $shadow_build_dir
cd $shadow_build_dir
qmake $project_dir "$config"
make

if [ "$run_unit" == "yes" ]; then
  ( cd csv-document-unit-test && ./csvDocumentUnitTest )
  ( cd conversation-rw-unit-test && ./conversationRwTest )
  ( cd default-sanitizer-unit-test && ./defaultSanitizerUnitTest ) 
  ( cd aiml-engine-unit-test && ./aimlEngineUnitTest )
  ( cd simple-aiml-engine-unit-test && ./simpleAimlEngineUnitTest )
  ( cd secure-stats-file-unit-test && ./secureStatsFileUnitTest )
  ( cd stats-manager-unit-test && ./statsManagerUnitTest )
  ( cd cipher-unit-test && ./cipherUnitTest )
fi

if [ "$run_e2e" == "yes" ]; then
  ( cd end-to-end-test && ./endToEndTest )
fi

cd $project_dir


