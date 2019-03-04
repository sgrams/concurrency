#!/bin/bash
src='zrodlo'
res='wyniki'

if [ -f $res ]; then rm $res; fi

read polynomial
echo $polynomial > $src

while [ ! -f $res ]
do
  [ -f $res ] && break;
done

cat $res 
rm $res
