/*******************************************************************************
* This is part of the MIDIPad program
*
* This header/C++ is to control and read the MIDI device. This requres the
* RtMidi Libraries, availible at https://github.com/thestk/rtmidi (preferably
* through your package manager)
*
* MIDIPad is designed specifically around Linux, as windows has most of the 
* MIDI/Music Production software. This is a side project, not intended to have
* any real-world use. I provide NO WARRANTY.
* MIDIPad is under the GPLv3 licence. Please see the LICENCE file included 
* with this program for more information. 
* ------------------------------------------------------------------------------
* This has only been tested on the 'AKAI APC Mini' device. I have given
* as much thought as I can into support for other deivces but I have none to 
* test with. Please feel free to test it with other devices and get in touch or
* do your own patches to get those devices working.
*
* (c) ADBeta 2022 
*******************************************************************************/
#ifndef MIDIHANDLER_h
#define MIDIHANDLER_h

#include <iostream>
#include <cstdlib>
#include <limits>
#include "RtMidi.h" //Through pacman is located in /usr/include

/** RtMidi Vars/Functions *****************************************************/
//MIDI Message type (multiple byte vector)
typedef std::vector<unsigned char> MidiMsg;

//RtMidiIn In and out objects
extern RtMidiIn *midiin;
extern RtMidiOut *midiout;

//Debug flag. This will enable CLI output whenever a key is pressed. (see funct)
extern bool MIDIDebug;
void CLIDebugMsg(MidiMsg *);

//MIDI Configuration variables
extern bool MIDILightingEnabled;

//Port selection and opening functions, for CLI and UI
int cliSelectMidiPort();
void openMidiPort(int);

//Callback function. Executed every time a message is received.
void getMsgAttributes(double, std::vector<unsigned char> *, void *);

//Close the MIDI port, delete vectors and perform RAM cleanup.
void cleanupMidi();

/** Lighting Controller *******************************************************/
namespace LightCtrl {
	//Array of ints, bytes that can trigger a lighting event. 
	//See MIDIHandler.cpp to edit or add values.
	extern int lightEventByte[];

	//How many bytes are in the lighting trigger array.
	extern int lightEventByteCount;
	
	
	//Does the message relate to an event ID? If so return ID#, if not return -1
	int getEventID(MidiMsg *);

}; //namespace LightCtrl
	
#endif
