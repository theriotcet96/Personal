#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include <iostream>
#include <stdlib.h>

#define SERIAL_PORT "/dev/ttyACM0"

int open_port(void)
{
  int location;
  location = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
  if(location == -1)
  {
    printf("open_port: Unable to open %s.\n", SERIAL_PORT);
  } else {
    fcntl(location, F_SETFL, 0);
    printf("port is open.\n");
  }

  return location;
}

int configure_port(int port)
{
  struct termios settings;
  cfsetispeed(&settings, B9600);
  cfsetospeed(&settings, B9600);
  settings.c_cflag &= ~PARENB;
  settings.c_cflag &= ~CSTOPB;
  settings.c_cflag &= ~CSIZE;
  settings.c_cflag |= CS8;

  tcsetattr(port, TCSANOW, &settings);
  return port;
}

int _serial;

void send(char* c)
{
  write(_serial, c, 1);
}

char zero = 0;
char one = 1;


int main(int argc, char** argv)
{
	_serial = open_port();
	configure_port(_serial);

	send(&zero);
	send(&zero);
	send(&zero);
	send(&one);
	send(&zero);
	send(&zero);

	close(_serial);
 	return 0;
}

