#!/bin/bash
fifo_server_name=serwerfifo

trap "" SIGHUP SIGTERM SIGCHLD
trap "exit 0" SIGUSR1

FIFOPATH = "$HOME/$fifo_serwvr_name"

if [ -p $FIFOPATH ]; then
  rm $FIFOPATH
fi
mkfifo $FIFOPATH
chmod 777

while true; do
  read REQUEST < $FIFOPATH
  if [ -n "$REQUEST" ]; then
    $fifo_client_name=${($REQUEST)[0]}
    echo $(($ARGUMENT/2.0 + 3)) > $fifo_client_name
  fi
done

exit 0
