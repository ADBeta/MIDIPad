/*******************************************************************************
* This is part of the MIDIPad program
*
* This header/C++ is to control and read the MIDI device. This requres the
* RtMIDI Libraries, availible at https://github.com/thestk/rtmidi (preferably
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
* Version 2.1.6
*******************************************************************************/
#include "MIDIHandler.h"

#include <iostream>
#include <cstdlib>
#include <limits>
#include "RtMidi.h" //Through pacman is located in /usr/include

//Define the MIDI in and out objects
RtMidiIn *midiin = new RtMidiIn();
RtMidiOut *midiout = new RtMidiOut();

int MIDIKeyCount = 64;
//Create an array of MIDIKeys on the heap.
MIDIKey *keyArray = new MIDIKey[MIDIKeyCount];

//Configuration extern variables
bool MIDIDebug = false;

/** System background functions ***********************************************/
//Callback function, execs every time a message is received. Splits message into
//bytes, handles lighting and external calls.
void getMsgAttributes(double delta_t, MIDIMsg *msg, void *) {
	//Error if the amount of bytes is more than 3. Other devices may fail this.
	if(msg->size() > 3) {
		std::cerr << "Error: Too many MIDI data bytes." << std::endl;
		cleanupMIDI();
		
		exit(EXIT_FAILURE);		
	}
	
	//If the Debug flag is set, call the debug output function.
	if(MIDIDebug) CLIDebugMsg(msg);
	
	//Do a key ID check and execute lighting commands
	MIDIKey *trigKey = findKeyWithID(msg->at(1));
	if(trigKey != NULL) {
		unsigned char outMsg[3];
		outMsg[0] = 144;
		outMsg[1] = trigKey->ID;
		
		if(msg->at(0) == trigKey->statusPressed) {
			outMsg[2] = trigKey->lightPressed;
		} else 
		if(msg->at(0) == trigKey->statusReleased) {
			outMsg[2] = trigKey->lightReleased;
		}
		
		midiout->sendMessage(outMsg, 3);		
	}
	
	//If the key pressed has a light
	
	//NOTE: delta_t is the diff in secs between messages. this may be useful
	//at some point. I will leave a small of example of this functionality for
	//reference if it useful at some point
	//std::cout << delta_t << std::endl;
}

/*** Port Setting Functions ***************************************************/
//Select a MIDI port to open via the CLI
int cliSelectMIDIPort() {	
	//Get the number of ports availible.
	unsigned int nPorts = midiin->getPortCount();
	
	//If no ports are availible then just exit
	if(nPorts == 0) {
		std::cerr << "No input ports available." << std::endl;
		cleanupMIDI();
		exit(EXIT_FAILURE);
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
		std::cout << "\nChoose a port: ";
		std::cin >> selection;
	} while(selection >= nPorts);
	
	//Reset the cin flags and clear the istream buffer (causes problems if not)
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	//Return the selected port number
	return selection;
}

//Opens both MIDI IN and MIDI OUT devices, depends on flags in the MIDI object
void openMIDIPort(int port) {
	try {
		//Open the MIDI devices on the passed port ID
		midiin->openPort(port);
		//Open an output port.
		midiout->openPort(port);

		//Set the callback function.  This should be done immediately after
		//opening the port to avoid having incoming messages written to the
		//queue instead of sent to the callback function.
		midiin->setCallback(&getMsgAttributes);
		//Don't ignore sysex, timing, or active sensing messages on midiin
		midiin->ignoreTypes(false, false, false);
	} catch ( RtMidiError &error ) {
		error.printMessage();
		
		//TODO error and exit program		
		exit(EXIT_FAILURE);
	}
}

//Close the MIDI port, delete vectors and cleanup the RAM
void cleanupMIDI() {
	delete midiin;
	delete midiout;
	
	delete[] keyArray;
}

/** Higher level MIDI functions ***********************************************/
//Prints a debug message to the CLI. Outputs the message bytes
void CLIDebugMsg(MIDIMsg *debugMsg) {
	std::cout << "Debug Msg: ";
	
	//Print 3 (change if needed) bytes from the MIDI heap.
	for(unsigned int cByte = 0; cByte < 3; cByte++) {
		std::cout << cByte << ": " << (int)debugMsg->at(cByte) << "\t\t";
	}
	std::cout << std::endl;
}

/** MIDIKey functions *********************************************************/
MIDIKey *findKeyWithID(unsigned char targetID) {
	//Go through all the keys in the heap keyArray
	for(int key = 0; key < MIDIKeyCount; key++) {
		if(keyArray[key].ID == targetID) {
			//If debug mode is enables, print some info.
			if(MIDIDebug) std::cout << "Key ID " << (int)targetID 
			  << " Matches keyArray[" << key << "] with Identifier string: " <<
			  keyArray[key].identifier << std::endl;
			
			//Return the pointer to the keyArray struct
			return &keyArray[key];
		}
	}
	
	//If not match is found: If degub mode is set, print a message.
	if(MIDIDebug) std::cout << "Key ID " << (int)targetID << 
	  " Does not match a keyArray struct." << std::endl;
	
	//Return a NULL pointer so you can detect it
	return NULL;
}

