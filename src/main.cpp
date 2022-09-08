#include "MIDIHandler.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]){
	
	std::cout << MidiCfg.message[0] << MidiCfg.message[1] << MidiCfg.message[2] << std::endl;	
	
	//cliSelectMidiPort();
	openMidiPort(cliSelectMidiPort());

	cleanupMidi();
	return 0;
}
