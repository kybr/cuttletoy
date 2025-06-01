#!/bin/sh


oscsend 224.0.7.23 7770 /display i 1
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
oscsend 224.0.7.23 7770 /desync


