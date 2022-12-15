/* This is a testing branch for now. It does not perform any useful function */
#include "MIDIHandler.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]){
	//Enable debug mode
	MIDIDebug = true;

	std::cout << "pointer info: " << findKeyWithID(69)->identifier << std::endl;	
	
	//Open a MIDI port that the user can select via the CLI
	openMidiPort(cliSelectMidiPort());

	//Tell the user how to exit. The callback function will take over.
	std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
	char input;
	std::cin.get(input);

	cleanupMidi();
	return 0;
}
