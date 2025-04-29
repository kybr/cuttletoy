#!/bin/bash
cd data
mkdir -p code
mkdir -p h
for file in `ls -1 *.txt`; do
  ../table.raku "$file" > code/$file
  ../header.raku "$file" > h/$file.h
done
