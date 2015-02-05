/**
 * teester for star_packet.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "star_packet.h"
#include "dbg.h"

int main(int argc, char* argv[]) {

   char* asciiPacket;
   star_packet* parsedPacket;
   char type;

   // Alloc a packet
   star_packet* packet = allocPacket("SRVA", "01-2345-6789", "Hello, world!");

   // Print each field of the packet
   printf("Creating packet with fields\n");
   printf("ID is %s\n", packet->identifier);
   printf("Dest is %s\n", packet->host);
   printf("Data is %s\n", packet->data);

   asciiPacket = packetString(packet);

   printf("\nStringified packet is:\n");
   printf("%s\n", asciiPacket);

   printf("\nParsing the stringified packet back to a packet.\n");
   parsedPacket = parsePacketString(asciiPacket);
   // Print each field of the packet
   printf("ID is %s\n", parsedPacket->identifier);
   printf("Source is %s\n", parsedPacket->host);
   printf("Data is %s\n", parsedPacket->data);

   type = verifyPacketType(parsedPacket, "SRV");
   printf("Type is %c\n", type);


   // Free it
   freePacket(packet);
   freePacket(parsedPacket);
   free(asciiPacket);

   return 0;
}
