# MIDIPad 

(Python Edition)
MIDIPad is a simple MIDI drum machine for linux, using Launchpad, APC etc.

## Description

This program is a fast implimentation of a basic MIDI input/output drum machine.
It is not intened to be fast, or particularly useful. 

Note: This is designed for linux specifically to make up for the lack of 
FLStudio or Ableton etc. 

## Dependancies & How To Use

* rtmidi-python
* simpleaudio

Run MIDIPad with python3, the port can be set via the MIDIPad.py file or via
command line list and select.

You will need to copy some sample files to a directory called "samples" :
kick.wav
snare.wav
hat.wav
cowbell.wav
crash.wav

## TO-DO 

Currently the program is only tested with the "AKAI APC Mini", the lighting 
commands may not work with other deivces, no guarantees are made it wil work
at all for other devices

Current very janky, and with limited functionality. Future revisions will 
have GUI modules, faster execution and more options, controls and functions
(Revision 2 will be written in C++)

Written by ADBeta 

## Licence

MIDIPad is under the GPL (GPL3.0), please see LICENCE for information

