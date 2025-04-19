#!/bin/bash

if ! test -f server; then
  echo COMPILING....
  make
fi
if ! test -f server; then
  echo FAILED TO COMPILE
  exit 1
fi

while true; do
  DEVICE=`find /dev/input -type l -name \*Microsoft\*event\*`
  if test -n "$DEVICE"; then
    ./server "$DEVICE"
  else
    echo NO DEVICE FOUND
  fi
  sleep 1
done

