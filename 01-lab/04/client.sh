#!/bin/bash
src="zrodlo"
res="wyniki"

read polynomial

if [ -f $src ]; then echo "" > $src; fi
if [ -f $res ]; then echo "" > $res; fi

echo $polynomial > $src
cat $src

if [ -f $res ]
then
  cat $res
  exit
fi

