/*******************************************************************************
* This is part of the MIDIPad program
*
* This header/c++ is to control and read the MIDI device. This requres the
* RtMidi Libraries, availible at https://github.com/thestk/rtmidi (preferably
* through your package manager)
*
* MIDIPad is designed specifically around Linux, as windows has most of the 
* MIDI/Music Production software. This is a side project, not intended ot have
* any real-world use. I hold NO RESPONSIBILTY or provide NO WARRANTY.
* MIDIPad is under the GPLv3 licence. Please see the LICENCE file included 
* with this program for more information. 
* ------------------------------------------------------------------------------
* This has only been tested on the 'AKAI APC Mini' device. I have given
* as much thought as I can into support for other deivces but I have none to 
* test with. Please feel free to get in touch
*
* (c) ADBeta 2022 
*******************************************************************************/
#ifndef MIDIHANDLER_h
#define MIDIHANDLER_h

#include <iostream>
#include <cstdlib>
#include <limits>
#include "RtMidi.h" //Through pacman is located in /usr/include

//TODO config must have note keys and system keys, with ranges(?)

/** Global Enums and Types ****************************************************/
//What each STATUS Byte from the MIDI standard means
enum MidiStatus : unsigned char {
	noteOff_t = 128,
	noteOn_t = 144,
};

//MIDI Message type (multiple bytes)
typedef std::vector<unsigned char> MidiMsg;

//TODO
/** Configuration class *******************************************************/
namespace MidiConfig {
	//TODO filename for config file. input/output settings. key layout. device
	//type. sound mode(?). key pressed & key idle message.

	//Is sending MIDI messages to the device permitted
	extern bool outputEnabled;
	
	//Number of keys that the device has
	extern unsigned int noteKeys;
	
	

};

/** Lighting Controller *******************************************************/
class LightingController {
	public:
	
	//TODO Make this able to hold idle, loaded and pressed bytes. arbitrary 
	//data in the enum
	//Contain raw 8bit bytes, as well as a mask for keywords and keytypes
	typedef enum MsgByte {
		//Note ON and OFF are aliased directly to their values.
		noteOn = 144, //Pure note ON (144)
		noteOff = 128, //Note OFF (128)
		
		//Aliased to upper 8 bits
		noteOnChan		= 1 << 8, //Note ON, plus the channel data
		noteOffChan		= 2 << 8, //Note OFF plus channel data
		colourID		= 3 << 8, //Raw colour data
		keyID			= 4 << 8, //Current button that is being pressed
		velocity		= 5 << 8, //Velocity of the key being pressed
		
	} MsgByte;
	
	//TODO MidiMsg templating system for multiple devices later
	const MsgByte MsgFormat[3] = {noteOn, keyID, colourID};
	unsigned char colourIDLE = 0x00;
	unsigned char colourACTIVE = 0x05;
	unsigned char colourLOADED = 0x01;
	
	void test(MidiMsg *);
	
	//TODO get number of keys from config
	//Create an array of messages for idle, pressed and loaded(?) states
	

}; //class LightCtrl
extern LightingController LightCtrl;

/** RtMidi Vars/Functions *****************************************************/
//RtMidiIn In and out objects
extern RtMidiIn *midiin;
extern RtMidiOut *midiout;

int cliSelectMidiPort();
bool openMidiPort(int);

//Callback function when messages come through
void getMsgAttributes(double, std::vector<unsigned char> *, void *);
//CLI control to select a MIDI port
int cliSelectMidiPort();

//Open a port for MIDI in and out
bool openMidiPort(int port);

void cleanupMidi();
	
#endif
