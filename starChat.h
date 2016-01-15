/**
 * A Starmode Chat program for Metricom Ricochet radios
 * Uses the star_packet.c libraries
 * Scott Vanderlind
 */

#define BUFFER_SIZE 2048

// The program state
typedef struct {

   // Radio serial port things
   int radio_fd;
   char radio_buf[BUFFER_SIZE];
   int radio_idx;

} state;

int initPort(char* portName);
void initRadio(int serialPort);
void handle_radio_data(state* s);
void sendPacket(int serialPort, star_packet* packet);
void handlePacketString(char* packetString);

void dumpb(char* buf);
