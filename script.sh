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

oscsend 192.68.7.3 7770 /time d

oscsend 192.68.7.4 7770 /time d
oscsend 192.68.7.5 7770 /time d
oscsend 192.68.7.6 7770 /time d
oscsend 192.68.7.7 7770 /time d
oscsend 192.68.7.8 7770 /time d
oscsend 192.68.7.9 7770 /time d
oscsend 192.68.7.10 7770 /time d
oscsend 192.68.7.11 7770 /time d
oscsend 192.68.7.12 7770 /time d
oscsend 192.68.7.13 7770 /time d
oscsend 192.68.7.14 7770 /time d
oscsend 192.68.7.15 7770 /time d
oscsend 192.68.7.16 7770 /time d
oscsend 192.68.7.17 7770 /time d
oscsend 192.68.7.18 7770 /time d











