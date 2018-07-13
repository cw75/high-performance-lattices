#  Copyright 2018 U.C. Berkeley RISE Lab
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

#!/bin/bash

args=( -j -t )
containsElement () {
  local e match="$1"
  shift
  for e; do [[ "$e" == "$match" ]] && return 0; done
  return 1
}

while getopts ":j:t:" opt; do
  case $opt in
   j)
     MAKE_THREADS=$OPTARG
     if containsElement $OPTARG "${args[@]}"
     then
       echo "Missing argument to flag $opt"
       exit 1
     else
       echo "make set to run on $OPTARG threads" >&2
     fi
     ;;
   t)
     TYPE=$OPTARG
     if containsElement $OPTARG "${args[@]}"
     then
       echo "Missing argument to flag $opt"
       exit 1
     else
       echo "build type set to $OPTARG" >&2
     fi
     ;;
   \?)
     echo "Invalid option: -$OPTARG" >&2
     exit 1
     ;;
  esac
done

if [[ -z "$MAKE_THREADS" ]]; then MAKE_THREADS=1; fi
if [[ -z "$TYPE" ]]; then TYPE=Release; fi

rm -rf build
mkdir build
cd build

cmake -std=c++ll -DCMAKE_BUILD_TYPE=$TYPE ..
make -j${MAKE_THREADS}
