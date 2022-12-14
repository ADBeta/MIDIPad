# MIDIPad 
### (c) ADBeta 2022 (C++ Edition)
MIDIPad is a simple MIDI drum machine for linux, using Launchpad, APC etc.  
It allows for MIDI Sequencer or Drum Machine modes, with multiple audio   
backends (?).  

Currently work in progress  

## Description
Due to the lack of FLStudio and Abelton, others have made replacements or  
packages to fill the gap - None of the main ones are easy to use or simple,  
and some do not include the functionality I require and would like in my  
package.  

It will be able to do simple Drum Machine triggering audio samples, as well as  
time based sequencer functionality. Eventually I would like to build in a very  
higly integrated audio backend without different output APIs.  

I also would like to eventually create a linear expandable timebase to create  
tracks of arbitrary length with audio track drag and drop. Much like FLStudio.  

** This is just a hobby project. It may be slow to be developed and will not  
be particularly amazing compared to other projects **  

## Dependancies
* rtmidi library  

## TO-DO 
* Choose audio controller  
* Impliment and test the audio handler API  
* design simple container for key  data, press time, unified data container  
* design time based sequencer and UI/CLI elements to support it.  
* add a fully capable error handler for CLI and UI  
* long term: add a customizable config file to add more lighting controls. e.g.  
button count, lighting controls, lighting commands, default values etc  

## Changelog
* 2.0.1 - Complete overhaul of systems and API calls. Finished some documentation.  
* 2.0.2 - Removed un-needed and broken code. Refactored a lot of functions.  
* 2.0.3 - Refactored the lighting controller to be more adaptable for future.  
* 2.0.4 - Added higher granularity for the lighitng controller.  
* 2.1.4 - Changed lightHandler implimentation again. -_-  

### Version 2.0.1

## Licence
MIDIPad is under the GPL (GPL3.0), please see LICENCE for information
