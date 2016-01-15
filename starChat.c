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
   // Some random variables
   int activity = 0;
   int max_fd = 0;


   state s;
   s.radio_idx = 0;

   // Open the serial port.
   s.radio_fd = serial_open(argv[1], 9600, 0);

   // Configure the radio 
   // initRadio();

   // Initialize a FD_SET
   fd_set readfs;
   // The max file descriptor is going to be the serial port.
   // If we ever open another file, we need to increment this.
   max_fd = s.radio_fd;

   while (1) {
      // Zero our FD_SET
      FD_ZERO(&readfs);
      // Add the radio to the FD_SET
      FD_SET(s.radio_fd);

      // Call select
      activity = select(max_fd + 1, &readfs, NULL, NULL, NULL);
      if (activity < 0) {
         perror("SELECT ERROR");
         exit(-1);
      }

      if (FD_ISSET(s.radio_fd, &readfs)) {
         handle_radio_data(&s);
      }

   }

   // Add stdin to FD_SET

   // Add serial port to FD_SET

   // Listen for input

   close(serialPort);

   return 0;
}

/**
 * This is where we're going to read data from the serial port into the
 * state's radio buffer. We'll only take a little bit at a time, and when
 * we detect that we have a good packet, we can then call a function to
 * parse things.
 */
void handle_radio_data(state* s) {
   // A temporary buffer to store our serial data in
   char buf[BUFFER_SIZE];
   int ret;
   // The pointer to where the end of the packet is. It's a 0X0D.
   char *end;

   // Full our temporary buffer and cap it off with a 0.
   ret = read(s->radio_fd, buf, BUFFER_SIZE);
   buf[ret] = 0;

   // If we had a read error or got no bytes, there's no coming back.
   // Just return.
   if (ret < 0) {
      perror("Radio read");
      return -errno;
   } else if (ret == 0) {
      return 0;
   }

   // Make sure we aren't overrunning our buffer in our state.
   // If we are, we're throwing away packets. I guess we didn't
   // want them anyway?
   if (s->radio_idx + ret > sizeof(s->radio_buf)) {
      s->radio_idx = 0;
   }

   // Look for the end of the packet. We might have to do this twice
   // because our packets are both led and terminated with a <cr>
   end = strchr(buf, CR);
   if (end) {
      // TODO: DO THE THING HERE.
      // Need to figure out how to detect when we have a full packet,
      // Becuase theoretically, it starts with a CR and ends with a CR.
      // Is it as simple as looking for two CRs? Maybe look for one, zero
      // it out, and look for a second. If there is a second, we know we
      // have a full packet between the two 0 values.
   } else {
      // If there's no CR found, just add the bytes to the end of the
      // state buffer and increment the index.
      memcpy(&s->radio_buf[s->radio_idx], buf, ret);
      s->radio_idx += ret;
   }
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
