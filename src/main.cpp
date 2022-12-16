/* This is a testing branch for now. It does not perform any useful function */
#include "MIDIHandler.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]){
	//Enable debug mode
	MIDIDebug = true;
	
	//Set up the 64 audio keys, basic no-config method for now
	for(int k = 0; k < MIDIKeyCount; k++) {
		MIDIKey *cKey = &keyArray[k];
		
		cKey->ID = k;
		cKey->hasLight = true;
		
		cKey->statusPressed = 144;
		cKey->statusReleased = 128;
		
		cKey->lightPressed = 5;
		cKey->lightReleased = 0;
		
		cKey->identifier = "audio_key";
	} 
	
	findKeyWithID(5);
	
	//Open a MIDI port that the user can select via the CLI
	openMIDIPort(cliSelectMIDIPort());
	
	//Tell the user how to exit. The callback function will take over.
	std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
	char input;
	std::cin.get(input);

	cleanupMIDI();
	return 0;
}
