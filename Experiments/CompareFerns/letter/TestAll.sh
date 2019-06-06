#!/bin/bash

GetExeName() {
  base_="$1"

  for exe_ in "${base_}" "${base_}.exe"
  do
    if which "${exe_}" > /dev/null 2>&1
    then
      echo "${exe_}"
      return 0
    fi
  done

  return 1
}

if [ $# -ne 1 ]
then
  echo "Usage: $0 weightsFile" 1>&2
  exit 1
fi

bestWeightsFile=$1

#toolFlags="-I /mnt/c/Work/Source/Bleak/Modules/Common/Subgraphs -I /mnt/c/Work/Source/Bleak/Modules/Trees/Subgraphs"
toolFlags="-I C:/Work/Source/Bleak/Modules/Common/Subgraphs -I C:/Work/Source/Bleak/Modules/Trees/Subgraphs"
searchCmd="bleakTool"
toolCmd=`GetExeName "${searchCmd}"`

if [ -z "${toolCmd}" ]
then
  echo "Error: ${searchCmd} is not in PATH." 1>&2
  exit 1
fi

for dir in *
do
  if [ ! -d "${dir}" ]
  then
    continue
  fi

  if [ ! -f "${dir}/Config.sad" ]
  then
    continue
  fi

  echo "Info: Processing ${dir} ..."

  pushd "${dir}"

  for runDir in Run1
  do
    if [ ! -d "${runDir}" ]
    then
      continue
    fi

    if [ ! -f "${runDir}/${bestWeightsFile}" ]
    then
      echo "Error: Missing weights file." 1>&2
      continue
    fi
    
    pushd "${runDir}"
    
    line=`"${toolCmd}" test -n 4000 -g ../../Test.sad -I ../../../hackedSubgraphs -I .. ${toolFlags} -w "${bestWeightsFile}" | grep 'running accuracy'`
    testAcc=`echo "${line}" | awk '{ gsub("\r",""); print $NF }'`
  
    echo "${dir}/${runDir}: Test accuracy = ${testAcc}"
    
    popd
  done
    
  popd
done

