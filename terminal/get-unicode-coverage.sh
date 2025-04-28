#!/bin/bash
mkdir -p data
for file in `ls -1 /usr/share/consolefonts/*Terminus24x12*`; do
  name=`basename "$file"`
  zcat "$file" | psfgettable - > data/$name.txt
done
