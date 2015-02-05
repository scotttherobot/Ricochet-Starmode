/**
 * A Starmode Chat program for Metricom Ricochet radios
 * Uses the star_packet.c libraries
 * Scott Vanderlind
 */

#include "star_packet.h"

int main(int argc, char* argv[]) {
   // Initialize a FD_SET
   // Configure the radio 
   // Add stdin to FD_SET
   // Add serial port to FD_SET
   // Listen for input

}

void handlePacketString(char* packetString) {
   // Parse the string into a packet.
   star_packet* packet = parsePacketString(packetString);
   

}

void initRadio(int serialPort) {
}
