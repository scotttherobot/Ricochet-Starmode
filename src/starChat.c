/**
 * A Starmode Chat program for Metricom Ricochet radios
 * Uses the star_packet.c libraries
 * Scott Vanderlind
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include "dbg.h"
#include "star_packet.h"
#include "starChat.h"

int main(int argc, char* argv[]) {
   // Open the serial port
   int serialPort;
   serialPort = open(argv[1], O_RDWR | O_NOCTTY);
   if (serialPort == -1) {
      perror("Error opening port");
      exit(-1);
   }

   // Configure the radio 
   initRadio(serialPort);

   // Initialize a FD_SET

   // Add stdin to FD_SET

   // Add serial port to FD_SET

   // Listen for input

}

void handlePacketString(char* packetString) {
   // Parse the string into a packet.
   star_packet* packet = parsePacketString(packetString);
}

void initRadio(int serialPort) {
   char* buf = malloc(DATA_SIZE);
   int readBytes = 0;

   // Our init string. Turn off local echo.
   char* ate = "ate0\r";
   char* starmode = "atdt**starmode\r";
   char* escape = "+++";

   memset(buf, '\0', sizeof buf);
   // Disable Command echo ate0
   // We can use write() and read() here
   // prototype(int target, char* src, int nBytes)
   debug("Initializing with %s", ate);
   write(serialPort, ate, strlen(ate));
   readBytes = read(serialPort, buf, sizeof buf);
   debug("Got %d bytes: %s back", readBytes, buf);

   // Enter Starmode
   debug("Entering starmode with %s", starmode);
   write(serialPort, starmode, strlen(starmode));
   // Starmode doesn't return anything
   /*
   memset(buf, '\0', sizeof buf);
   readBytes = read(serialPort, buf, sizeof buf);
   debug("Got %d bytes: %s back", readBytes, buf);
   */

   // Try sending a packet
   debug("Sending at~la");
   star_packet* p = allocPacket("", "&COMMAND", "at~la");
   char* pstr = packetString(p);
   debug("Sending %s", pstr);
   write(serialPort, pstr, strlen(pstr));
   memset(buf, '\0', sizeof buf);
   while (readBytes = read(serialPort, buf, sizeof buf)) {
      debug("Got %d bytes: %s back", readBytes, buf);
   }

   // leave Starmode
   memset(buf, '\0', sizeof buf);
   debug("Leaving starmode with %s", escape);
   write(serialPort, escape, strlen(escape));
   readBytes = read(serialPort, buf, sizeof buf);
   debug("Got %d bytes: %s back", readBytes, buf);

   // That's all she wrote
   close(serialPort);

}

void sendPacket(int serialPort, star_packet* packet) {

}
