#!/bin/bash
LVL=$(($1+1))
if [ $LVL -eq 5 ]
then
  exit 0
fi

echo $$
sleep 3
./$0 $LVL & ./$0 $LVL &

exit 0
