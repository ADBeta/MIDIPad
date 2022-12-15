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

//Port selection and opening functions, for CLI and UI
int cliSelectMidiPort();
void openMidiPort(int);

//Callback function. Executed every time a message is received.
void getMsgAttributes(double, std::vector<unsigned char> *, void *);

//Close the MIDI port, delete vectors and perform RAM cleanup.
void cleanupMidi();

/** Unified Key handling and Light Handling ***********************************/
//TODO explain attributes and data TODO default?
struct MIDIKey {
	//MIDI Key ID. Byte as it is limited by MIDI Bytes.
	unsigned char ID = 0;
	
	//MIDI Message Status byte triggers - pressed and released
	unsigned char statusPressed = 0;
	unsigned char statusReleased = 0;
	
	//Does this key have a light?
	bool hasLight = false;
	
	//Identifier string. Optional but useful to keep track of which key is which
	std::string identifier = "undefined";
	
	//TODO Key type
	
	//TODO pointer to function to call when pressed


}; //struct MIDIKey

//Number of keys, Set via config or manually.
extern int MIDIKeyCount;

//Array of MIDIKeys (on heap). Size defined by Number of keys. TODO config
extern MIDIKey *keyArray;

//Function that returns a pointer to a MIDIKey, if one exists with the ID Given.
MIDIKey *findKeyWithID(unsigned char);

#endif
