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
   serialPort = initPort(argv[1]);
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

   close(serialPort);

   return 0;
}

int initPort(char* portName) {
   int fd;
   struct termios options;

   fd = open(portName, O_RDWR | O_NOCTTY | O_NDELAY);
   fcntl(fd, F_SETFL, 0);

   // Flush the buffer
   sleep(1);
   tcflush(fd, TCIOFLUSH);

   // Get the current options
   tcgetattr(fd, &options);

   // Set raw mode + timeout
   options.c_cflag |= (CLOCAL | CREAD);
   options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
   options.c_oflag &= ~OPOST;
   options.c_cc[VMIN] = 0;
   options.c_cc[VTIME] = 10;

   // Set the options
   tcsetattr(fd, TCSANOW, &options);

   return fd;
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

   dumpb(buf);

   // Enter Starmode
   debug("Entering starmode with %s", starmode);
   write(serialPort, starmode, strlen(starmode) - 1);
   // Starmode doesn't return anything

   // Try sending a packet
   debug("Sending at~la");
   star_packet* p = allocPacket("", "&COMMAND", "at~la");
   //char* pstr = packetString(p);
   char* pstr = "\r*&COMMAND*at~la\r";
   dumpb(pstr);

   write(serialPort, pstr, strlen(pstr) - 1);
   memset(buf, '\0', sizeof buf);
   int nbytes;
   char* bufptr = buf;
   while ((nbytes = read(serialPort, bufptr, buf + sizeof(buf) - bufptr - 1)) > 0) {
      bufptr += nbytes;
      if (bufptr[-1] == '\n' || bufptr[-1] == '\r') {
         debug("read \\r");
         break;
      }
   }
   *bufptr = '\0';
   dumpb(buf);

   /*
   // leave Starmode
   memset(buf, '\0', sizeof buf);
   debug("Leaving starmode with %s", escape);
   write(serialPort, escape, strlen(escape));
   readBytes = read(serialPort, buf, sizeof buf);
   debug("Got %d bytes: %s back", readBytes, buf);
   dumpb(buf);
   */

   // That's all she wrote

}

// Dump a buffer
void dumpb(char* buf) {
   int i;

   debug("strlen: %d", (int)strlen(buf));
   debug("string: %s", buf);
   for (i = 0; i < (int)strlen(buf); i++) {
      debug("buf[%d] = %d", i, (int)buf[i]);
   }
}

void sendPacket(int serialPort, star_packet* packet) {

}
