#!/bin/bash
src="zrodlo"
res="wyniki"

while true
do
  if [ -f $src ]
  then
    result=$(cat $src | bc)
    echo $result > $res
    echo "" > $src
  fi
done
