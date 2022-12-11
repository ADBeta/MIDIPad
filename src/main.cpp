#include "MIDIHandler.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]){
	
	//std::cout << MidiConfig::message[0] << MidiConfig::message[1] << MidiConfig::message[2] << std::endl;	
	
	//cliSelectMidiPort();
	openMidiPort(cliSelectMidiPort());

	//TODO get rid of this eventually
	std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
	char input;
	std::cin.get(input);

	cleanupMidi();
	return 0;
}
