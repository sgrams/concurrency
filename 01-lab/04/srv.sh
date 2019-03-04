#!/bin/bash
src='zrodlo'
res='wyniki'

while true
do
  if [ -e $src ]
  then
    result=$(cat $src | bc)
    echo $result > $res
    rm $src
  fi
done
