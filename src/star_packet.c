/**
 * Metricom Starmode Packet Utility
 * Scott Vanderlind
 */

#include <stdlib.h>
#include <string.h>
#include "star_packet.h"
#include "dbg.h"

star_packet* allocPacket(char* identifier, char* host, char* data) {
   // A packet.
   star_packet* packet = malloc(sizeof(star_packet));

   packet->identifier_len = strlen(identifier);
   packet->host_len = strlen(host);
   packet->data_len = strlen(data);
   
   // Create space for the id, host, and data.
   packet->identifier = malloc(packet->identifier_len);
   packet->host = malloc(packet->host_len);
   packet->data = malloc(packet->data_len);

   // Copy everything in.
   memcpy(packet->identifier, identifier, packet->identifier_len);
   memcpy(packet->host, host, packet->host_len);
   memcpy(packet->data, data, packet->data_len);

   if (packet->data_len + packet->identifier_len > DATA_SIZE) {
      // This is an error condition.
      debug("Data length exceeds max: %d > %d", packet->data_len, DATA_SIZE - packet->identifier_len);
   }

   return packet;
}

void freePacket(star_packet* packet) {
   free(packet->identifier);
   free(packet->host);
   free(packet->data);
   free(packet);
}

char* packetString(star_packet* packet) {
   // Create room for a string terminated by a cr
   int packetLen = HEADER_SIZE + packet->data_len + packet->identifier_len + 1;
   char* asciiPacket = malloc(packetLen);
   char cr = 13;
   char asterisk = '*';
   int index = 0;

   // The first byte is a carriage return, then an asterisk
   memcpy(asciiPacket, &cr, 1);
   memcpy(asciiPacket + ++index, &asterisk, 1);

   // Then comes the host 
   memcpy(asciiPacket + ++index, packet->host, packet->host_len);
   debug("Inserting host at %d", index);

   // then another asterisk
   index += packet->host_len;
   memcpy(asciiPacket + index, &asterisk, 1);

   // Then the identifier
   memcpy(asciiPacket + ++index, packet->identifier, packet->identifier_len);
   debug("Inserting identifier at %d", index);

   // Then the data
   index += packet->identifier_len;
   memcpy(asciiPacket + index, packet->data, packet->data_len);
   debug("Inserting data at %d", index);

   // Don't forget the <cr> terminator
   memcpy(asciiPacket + packetLen, &cr, 1);

   return asciiPacket;
}

int findSecondAsterisk(char* packetString) {
   int i, seen = 0;
   for (i = 0; i < strlen(packetString); i++) {
      if (packetString[i] == '*') {
         seen++;
         if (seen == 2) {
            // We found it!
            return i;
         }
      }
   }
   return 0;
}

star_packet* parsePacketString(char* packetString) {
   int secondAsterisk, senderLen, identificationLen, dataLen, headerLen;
   char* sender, *identification, *data;
   star_packet* packet;
   
   secondAsterisk = findSecondAsterisk(packetString);
   // senderLen = secondAsterisk index - 2 bytes in front;
   senderLen = secondAsterisk - 2;
   // Remember to make room for a null;
   sender = malloc(senderLen + 1);
   memcpy(sender, packetString + 2, senderLen);
   sender[senderLen] = '\0';
   debug("Sender is len %d, %s", senderLen, sender);

   // The identification is always 4 bytes + 1 for a null
   identification = malloc(sizeof(char) * 5);
   identificationLen = 4;
   memcpy(identification, packetString + secondAsterisk + 1, 4);
   identification[identificationLen] = '\0';
   debug("Identification is len %d, %s", identificationLen, identification);

   // The data, and this will account for the null from the original string.
   headerLen = 2 + senderLen + 1 + identificationLen;
   dataLen = strlen(packetString) - headerLen;
   debug("Data length is %d", dataLen);
   data = malloc(dataLen);
   memcpy(data, packetString + headerLen, dataLen);
   debug("Data is %s", data);

   packet = allocPacket(identification, sender, data);
   free(identification);
   free(sender);
   free(data);

   return packet;
}
