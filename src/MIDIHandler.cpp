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
* Version 2.0.2
*******************************************************************************/
#include "MIDIHandler.h"

#include <iostream>
#include <cstdlib>
#include <limits>
#include "RtMidi.h" //Through pacman is located in /usr/include

//Define the MIDI in and out objects
RtMidiIn *midiin = new RtMidiIn();
RtMidiOut *midiout = new RtMidiOut();

//Lighting Controller Variables.
namespace LightCtrl { 
	//MIDI Message Byte values that trigger a lighting event. 
	//Add or edit values to create more events.
	int lightEventByte[2] = {144, 128};
	
	//enum event types to the values in lightEventByte. For example by default
	//lightEventByte[0] is 144, which is light_on event. these can be modified
	//by functions later depending on flags.
	enum eventID {
		light_on = 0,
		light_off = 1,
	}; 
	
	//How many bytes are in the array.
	int lightEventByteCount = sizeof(lightEventByte) / sizeof(int);
}

//Configuration extern variables
bool MIDIDebug = false;
bool MIDILightingEnabled = true;

/** System background functions ***********************************************/
//Callback function, execs every time a message is received. Splits message into
//bytes, handles lighting and external calls.
void getMsgAttributes(double delta_t, MidiMsg *msg, void *) {
	//Get the size of the vector in bytes
	unsigned int nBytes = msg->size();
	
	//Error if the amount of bytes is more than 3. Other devices may fail this.
	if(nBytes > 3) {
		std::cerr << "Error: Too many MIDI data bytes." << std::endl;
		cleanupMidi();
		
		exit(EXIT_FAILURE);		
	}
	
	//If the Debug flag is set, call the debug output function.
	if(MIDIDebug == true) CLIDebugMsg(msg);
	
	//Check if the message is one that the Lighting Controller Should care about
	//if(LightCtrl::isValidMsg(msg)) {
		std::cout << LightCtrl::getEventID(msg) << std::endl;
	//}
	
	//NOTE: delta_t is the diff in secs between messages. this may be useful
	//at some point. I will leave a small of example of this functionality for
	//reference if it useful at some point
	//std::cout << delta_t << std::endl;
}

/*** Port Setting Functions ***************************************************/
//Select a MIDI port to open via the CLI
int cliSelectMidiPort() {	
	//Get the number of ports availible.
	unsigned int nPorts = midiin->getPortCount();
	
	//If no ports are availible then just exit
	if(nPorts == 0) {
		std::cerr << "No input ports available" << std::endl;
		return -1;
	}
	
	//Print all the ports by name, using getPortName
	for(unsigned int pn = 0; pn < nPorts; pn++) {
		std::string portName = midiin->getPortName(pn);
		//Print the current port name at index pn
		std::cout << "\tInput port #" << pn << ": " << portName << std::endl;
	}

	//Keep asking for a valid input until one is given
	unsigned int selection;
	do {
		std::cout << "\nChoose a port number: ";
		std::cin >> selection;
	} while (selection >= nPorts);
	
	//Reset the cin flags and clear the istream buffer (causes problems if not)
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	//Return the selected port number
	return selection;
}

//Opens both MIDI IN and MIDI OUT devices, depends on flags in the MIDI object
void openMidiPort(int port) {
	//Failsafe, allows passing to this function if escaped correctly
	//if(port < 0) {
	//	return false;
	//}
	
	try {
		//Open the MIDI devices on the passed port ID
		midiin->openPort(port);
		
		//If lighting or output is enabled, open an output port.
		if(MIDILightingEnabled == true) midiout->openPort(port);

		//Set the callback function.  This should be done immediately after
		//opening the port to avoid having incoming messages written to the
		//queue instead of sent to the callback function.
		midiin->setCallback(&getMsgAttributes);
		//Don't ignore sysex, timing, or active sensing messages on midiin
		midiin->ignoreTypes(false, false, false);
	} catch ( RtMidiError &error ) {
		error.printMessage();
		
		//TODO exit program		
		exit(EXIT_FAILURE);
	}
}

//Close the MIDI port, delete vectors and cleanup the RAM
void cleanupMidi() {
	delete midiin;
	delete midiout;
}

/** Higher level MIDI functions ***********************************************/
//Prints a debug message to the CLI. Outputs the message bytes
void CLIDebugMsg(MidiMsg *debugMsg) {
	std::cout << "Debug Msg: ";
	
	//Print 3 (change if needed) bytes from the MIDI heap.
	for(unsigned int cByte = 0; cByte < 3; cByte++) {
		std::cout << cByte << ": " << (int)debugMsg->at(cByte) << "\t\t";
	}
	std::cout << std::endl;
}

/** Lighting Control Functions ************************************************/
namespace LightCtrl {
////
int getEventID(MidiMsg *msg) {
	//Get the message type, the first byte of the message vector.
	int msgTypeByte = (int)msg->at(0);
	
	//Go through all the trigger bytes and look for a match.
	for(int leb = 0; leb < lightEventByteCount; leb++) {
		if(msgTypeByte == lightEventByte[leb]) {
			return leb;
		}
	}
	
	//If no match is found
	return -1;
}


}; //nameapace LightCtrl
