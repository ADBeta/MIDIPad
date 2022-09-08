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
#include "MIDIHandler.h"

#include <iostream>
#include <cstdlib>
#include <limits>
#include "RtMidi.h" //Through pacman is located in /usr/include

//Define the MIDI in and out objects
RtMidiIn *midiin = new RtMidiIn();
RtMidiOut *midiout = new RtMidiOut();

//Define the midi configuation object
MidiConfig MidiCfg;

//Create a cusom callback function, called when receiving messages
//This callback splits the message into bytes and stores them in a message 
//class object. 
void getMsgAttributes(double delta_t,
                      std::vector<unsigned char> *msg, void *) {
	//Get the size of the vector in bytes
	unsigned int nBytes = msg->size();
	
	//Error if the amount of bytes is more than 3. Might not be useful for other
	//devices. 
	if(nBytes > 3) {
		//TODO add a full error handler, gui ready
		std::cerr << "Error: Too many MIDI data bytes." << std::endl;
		cleanupMidi();
		
		exit(EXIT_FAILURE);		
	}
	
	/* The fist byte is the status byte, use this (via enum) to take action */
	//Note message
	if(msg->at(0) == noteOn_t || msg->at(0) == noteOff_t) {
		//Pass on the vector data to the NoteHandler class set, which will
		//handle lighting, and pass information onto the sound controller
		
		unsigned char outMsg[3] = {144, msg->at(1)};
		if(msg->at(0) == noteOn_t) { 
			outMsg[2] = 0x01;
		} else {
			outMsg[2] = 0x00;
		}
		
		midiout->sendMessage(outMsg, 3);		
		
		std::cout << "note Type" << std::endl;
	}
	
	//TODO Slider message
	
	//Go through the vector entries
	for(unsigned int i = 0; i < nBytes; i++) {
		std::cout << "Byte " << i << " = " << (int)msg->at(i) << ", ";
	}
	std::cout << std::endl;
	
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
	if ( nPorts == 0 ) {
		std::cout << "No input ports available" << std::endl;
		return -1;
	}
	
	//Print all the ports by name, using getPortName
	for (unsigned int pn = 0; pn < nPorts; pn++) {
		std::string portName = midiin->getPortName(pn);
		//Print the current port name at index pn
		std::cout << "\tInput port #" << pn << ": " << portName << std::endl;
	}

	//Keep asking for a valid input until one is given
	unsigned int select;
	do {
		std::cout << "\nChoose a port number: ";
		std::cin >> select;
	} while ( select >= nPorts );
	
	//Reset the cin flags and clear the istream buffer (causes problems if not)
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	//Return the selected port number
	return select;
}

//Opens both MIDI IN and MIDI OUT devices, depends on flags in the MIDI object TODO
bool openMidiPort(int port) {
	//Failsafe, allows passing to this function if escaped correctly
	if(port < 0) {
		return false;
	}
	
	try {
		//Open the MIDI devices on the passed port ID
		midiin->openPort(port);
		
		//TODO check if output enabled
		midiout->openPort(port);
	
		/*
		//TODO NOTE This can be done with either array or vector
		unsigned char msg[3][3] = {
			{144, 5, 1},
			{144, 6, 3},
			{144, 7, 5},
		};
		// Program change: 192, 5
		//message.push_back( 144 );
		//message.push_back( 5 );
		//message.push_back( 3 );
		midiout->sendMessage(msg[0], 3);
		midiout->sendMessage(msg[1], 3);
		midiout->sendMessage(msg[2], 3);
		*/

		//Set the callback function.  This should be done immediately after
		//opening the port to avoid having incoming messages written to the
		//queue instead of sent to the callback function.
		
		midiin->setCallback(&getMsgAttributes);
		
		//Don't ignore sysex, timing, or active sensing messages on midiin
		midiin->ignoreTypes(false, false, false);


		//TODO get rid of this eventually
		std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
		char input;
		std::cin.get(input);

	} catch ( RtMidiError &error ) {
		error.printMessage();
	}
	
	return true;
}

void cleanupMidi() {
	delete midiin;
	delete midiout;
}
