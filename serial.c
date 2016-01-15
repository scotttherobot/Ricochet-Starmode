/**
 * Serial port helper functions
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/stat.h>

int serial_get_baud(int baud) {
   // TODO: DON'T HARDCODE THIS BAUD RATE!
   return B4800;
}

int serial_set_baud(int fd, int baud, int hwflow) {
   struct termios term;
   int ret;

   // Get the deets of the character device
   ret = tcgetattr(fd, &term);
   if (ret < 0) {
      perror("tcgetattr");
   }

   cfmakeraw(&term);
   cfsetspeed(&term, serial_get_baud(baud));

   if (hwflow) {
      term.c_cflag |= CRTSCTS;
   } else {
      term.c_cflag &= ~CRTSCTS;
   }

   ret = tcsetattr(fd, TCSAFLUSH, &term);
   if (ret < 0) {
      perror("tcsetattr");
   }

   return ret;
}

int serial_open(const char* portname, int baud, int hwflow) {
   int fd;
   int ret;
   struct stat s;

   fd = open(portname, O_RDWR);
   // This is an error. Bubble it up.
   if (fd < 0) {
      return fd;
   }
   fstat(fd, &s);

   // If it's a character special file (like a tty)
   // we should set the baud rate.
   if (S_ISCHR(s.st_mode)) {
      ret = serial_set_baud(fd, baud, hwflow);
      // If it wasn't successful, abort.
      if (ret) {
         close(fd);
         return ret;
      }
   }

   return fd;
}
