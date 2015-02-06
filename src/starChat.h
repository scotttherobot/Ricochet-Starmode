/**
 * A Starmode Chat program for Metricom Ricochet radios
 * Uses the star_packet.c libraries
 * Scott Vanderlind
 */

int initPort(char* portName);
void initRadio(int serialPort);
void sendPacket(int serialPort, star_packet* packet);
void handlePacketString(char* packetString);

void dumpb(char* buf);
