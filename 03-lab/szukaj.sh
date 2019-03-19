#!/bin/bash

if [ "$#" -lt 2 ]; then
  echo "szukaj PLIK KATALOG"
  exit -1
else
  PLIK=$1
  KATALOG=$2
  POTOMNY=$3
fi

ILOSC_POTOMNYCH=0
for dir in $(find "$KATALOG" -maxdepth 1 -mindepth 1 -type d)
do
  ./szukaj.sh $PLIK $dir "TRUE" &
  ((ILOSC_POTOMNYCH=$ILOSC_POTOMNYCH+1))
done

ZNALEZIONO=0

for file in $(find "$KATALOG" -maxdepth 1 -mindepth 1 -type f -printf "%f\n")
do
  if [ "$file" == "$PLIK" ]; then
    echo "$KATALOG/$PLIK"
    ((ZNALEZIONO=$ZNALEZIONO+1))
  fi
done

while [ "$ILOSC_POTOMNYCH" -gt 0 ]
do
  wait
  ((ILOSC_POTOMNYCH=$ILOSC_POTOMNYCH-1))
done

if [ -z "$POTOMNY" ]; then
  if [ "$ZNALEZIONO" -eq 0 ]; then
    echo "Nie znaleziono podanego pliku"
  fi
fi
