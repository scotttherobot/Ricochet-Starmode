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
   char *buf = (char *)calloc(BUF_SIZE, sizeof(char));
   int num = 0;
   printf("Listening on %d\n", pipe);

   while (1) {
      num = poll(fds, 1, -1);
      //printf("selected %d\n", num);
      ioctl(pipe, FIONREAD, &len);
      if (len > 0) {
         printf("%d bytes availanle\n", len);
         memset(buf, 0, BUF_SIZE);

         len = read(pipe, buf, len);
         printf("%d bytes read\n", len);
         printf("got: %s\n", buf);
      }
   }
}

void touchPipe(int pipe) {
   int i = 0;
   int written = 0;
   char* buf;

   star_packet* packet = allocPacket("SRVA", "01-2345-6789", "Hello, world!");
   buf = packetString(packet);

   for (i = 0; i < 100; i++) {
      written = write(pipe, buf, strlen(buf));
   }

   freePacket(packet);
   free(buf);
}

