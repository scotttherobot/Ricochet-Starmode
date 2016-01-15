/**
 * A multiprocess version of the ricochet starmode packet driver.
 */
#include<stdio.h>
#include<poll.h>
#include<signal.h>
#include<iostream>
#include<fstream>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/epoll.h>
#include<sys/types.h>
#include<sys/ioctl.h>

#include "star_packet.h"

#define BUF_SIZE 50

void watchPipe(int pipe);
void touchPipe(int pipe);


int main() {

   int pipefd[2];

   if (pipe(pipefd)) {
      perror("Pipe!");
      exit(-1);
   }

   if (fork() == 0) {
   // If we're the child
      close(pipefd[0]);
      sleep(2);
      touchPipe(pipefd[1]);
   } else {
   // If we're the parent
      close(pipefd[1]);
      watchPipe(pipefd[0]);
   }
}

void watchPipe(int pipe) {
   struct pollfd fds[1];
   fds[0].fd = pipe;
   fds[0].events = POLLIN;
   int len = 0;
   star_packet* packet = (star_packet*)calloc(1, sizeof(star_packet));
   printf("Listening on %d\n", pipe);

   while (1) {
      poll(fds, 1, -1);
      ioctl(pipe, FIONREAD, &len);
      if (len == 0)
         continue;
      printf("%d bytes availanle\n", len);
      len = read(pipe, packet, sizeof(star_packet));
      printf("%d bytes read\n", len);

      printf("id len: %d\n", packet->identifier_len);
      printf("id: %s\n", packet->identifier);
      printf("host len: %d\n", packet->host_len);
      printf("host: %s\n", packet->host);
      printf("data len: %d\n", packet->data_len);
      printf("data: %s\n", packet->data);
   }
}

void touchPipe(int pipe) {
   int i = 0;

   const char* id = "SRVA";
   const char* host = "01-2345-6789";
   const char* data = "Hello, world!";
   static star_packet* packet = allocPacket((char*)id, (char*)host, (char*)data);

   for (i = 0; i < 1; i++) {
      printf("Sending packet: %s\n", packetString(packet));
      write(pipe, packet, sizeof(star_packet));
   }

   //freePacket(packet);
}

