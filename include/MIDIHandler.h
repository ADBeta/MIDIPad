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

/** Global Enums and Types ****************************************************/
//Status byte meaning via type define
enum MidiStatus : unsigned char {
	noteOff_t = 128,
	noteOn_t = 144,
};

/** Configuration class *******************************************************/
class MidiConfig {
	//TODO filename for config file. input/output settings. key layout. device
	//type. sound mode(?). key pressed & key idle message.

	public:
	/** Types and enums *******************************************************/
	//A MidiMessage is multiple bytes, usually 3. Type declaration.
	typedef std::vector<unsigned char> MidiMsg;
	
	/** ------- ***************************************************************/	
	//MIDI Messages - Idle - Pressed - Loaded. Single message for each
	
	//TODO
	MidiMsg message = {40, 41, 42};
	
	int val = 69;
	
	private:

}; //class MidiConfig
extern MidiConfig MidiCfg;

/** Current MIDI Note *********************************************************/
struct MidiNoteMessage {
	//3 Byte MIDI Message for note
	unsigned char status;
	unsigned char pitch; 
	unsigned char velocity;
}; //struct MidiMessage
extern MidiNoteMessage MidiNote;

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
