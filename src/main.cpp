#include "MIDIHandler.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]){
	
	//std::cout << MidiConfig::message[0] << MidiConfig::message[1] << MidiConfig::message[2] << std::endl;	
	
	//cliSelectMidiPort();
	openMidiPort(cliSelectMidiPort());

	cleanupMidi();
	return 0;
}
