#!/bin/bash

oscsend 224.0.7.23 7770 /quit
sleep 10

while true; do

oscsend 224.0.7.23 7770 /time d $(( $RANDOM % (700) + 20 ))
oscsend 224.0.7.23 7770 /clear

./test/multicast/frag.lua glsl/content/UnderWater.glsl
./terminal/bubbles 960 268 150

oscsend 224.0.7.23 7770 /time d $(( $RANDOM % (700) + 20 ))
./cli/frag glsl/content/Ocean.glsl
sleep 100

oscsend 224.0.7.23 7770 /clear
./cli/frag glsl/content/discard.glsl
./terminal/stars 960 268 35
./cli/frag glsl/content/flat2.glsl
./terminal/stars 960 268 65
oscsend 224.0.7.23 7770 /desync d 1
sleep 3
oscsend 224.0.7.23 7770 /desync d 3
sleep 4
oscsend 224.0.7.23 7770 /desync d 7
sleep 7

oscsend 224.0.7.23 7770 /time d $(( $RANDOM % (700) + 20 ))
./cli/frag glsl/content/flat.glsl
./terminal/stars 960 268 60

./cli/frag glsl/content/b.glsl
sleep 60

./cli/frag glsl/content/a.glsl
sleep 60

./cli/frag glsl/content/Ocean.glsl
sleep 60

./cli/frag glsl/content/e.glsl
sleep 60

./cli/frag glsl/content/d.glsl
sleep 60

./cli/frag glsl/content/g.glsl
sleep 60

./cli/frag glsl/content/discard.glsl
./terminal/stars 960 268 30


done


