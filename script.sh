#!/bin/sh

oscsend 224.0.7.23 7770 /display i 1
oscsend 224.0.7.23 7770 /quit

while true; do

oscsend 224.0.7.23 7770 /time d 0.0
oscsend 224.0.7.23 7770 /clear

./cli/frag glsl/content/UnderWater.glsl
./terminal/bubbles 960 268 130 # blocks for 130 seconds
oscsend 224.0.7.23 7770 /clear

./cli/frag glsl/content/Ocean.glsl
sleep 130

./cli/frag glsl/content/discard.glsl
./terminal/stars 960 268 35
./cli/frag glsl/content/flat2.glsl
./terminal/stars 960 268 65
oscsend 224.0.7.23 7770 /desync 1
sleep 1
oscsend 224.0.7.23 7770 /desync 1
sleep 1
oscsend 224.0.7.23 7770 /desync 1
sleep 1


oscsend 224.0.7.23 7770 /desync d 10
./cli/frag glsl/content/flat.glsl
sleep 20


oscsend 224.0.7.23 7770 /time d 0.0
./cli/frag glsl/content/b.glsl
sleep 60

./cli/frag glsl/content/a.glsl
sleep 60

./cli/frag glsl/content/e.glsl
sleep 60

./cli/frag glsl/test-pi/time-ramp-sync.glsl
oscsend 224.0.7.23 7770 /desync d 2.0
sleep 20
oscsend 224.0.7.23 7770 /time d 0.0

done


