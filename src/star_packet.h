/**
 * A Metricom Ricochet Starmode Packet Utility
 * Creates ASCII packets to be transmitted via Ricochet Radios
 * Scott Vanderlind
 *
 * The packet structure for modems in starmode is as follows
 * (where <CR> is a carriage return)
 *
 * Outgoing:
 * <CR>*<Destination>*<Identifier><Data>
 * 
 * Incoming:
 * <CR>*<Source>*<Identifier><Data>
 *
 * <Destination> is the address of the destination modem.
 *    This can either be a handle (returned from at~la) or an absolute
 *    hardware address (like 02-0041-0202).
 *
 * <Source> is the address of the sending modem.
 *    This will always be the absolute address (02-0041-0202).
 *
 * <Identifier> is the application identifier composed of four ASCII chars,
 *    in the format MMMT.
 *
 *    The first three chars (M) are the manufacturer ID shared by all packets
 *       of an application.
 *    The final char (T) is the packet type within that application.
 *    For example, SRVA, SRVB, SRVC are valid identifiers for the manufacturer
 *       SRV with packet types {A, B, C}.
 *    Allegedly "reserved" identifiers include ERR_, INFO, ACK_, METC, and ATR.
 *
 */

#include <stdint.h>

/* The maximum length of the packet header
 */
#define HEADER_SIZE 15

/* The maximum length of the data after the second * of the packet
 */
#define DATA_SIZE 1183

/* A packet structure for passing around all the required data.
 */
typedef struct {
   char* identifier;
   uint16_t identifier_len;
   char* host;
   uint16_t host_len;
   char* data;
   uint16_t data_len;
} star_packet;

/* The core functions for packet manipulation
 */

// Alloc and return a packet.
star_packet* allocPacket(char* identifier, char* destination, char* data);
// Free a packet and its contents.
void freePacket(star_packet* packet);
// Convert a packet struct to a string to send to the modem
char* packetString(star_packet* packet);
// Convert a packet string to packet.
star_packet* parsePacketString(char* packetString);

// Misc helpers

// Finds the second asterisk delimiter and returns its index
int findSecondAsterisk(char* packetString);
