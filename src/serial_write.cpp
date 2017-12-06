#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include <iostream>
#include <stdlib.h>

#define STRLEN 22

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


void nextInt(FILE* fp, int* buff)
{
  fscanf(fp,"%d", buff);
}
void nextDouble(double* buff)
{
  scanf("%lf", buff);
}
void nextStr(FILE* fp, char* buff)
{
  fscanf(fp,"%s", buff);
}
char Beats[12][STRLEN] = {"1/4", "1/8", "1/16", "1/32", "1/2", "1",
			"1/4.", "1/8.", "1/16.", "1/32.", "1/2.", "1."};
int delays[12] = {500000, 250000, 167000, 125000, 1000000, 2000000, 750000, 375000, 250000, 187000, 1500000, 3000000};
int main(int argc, char** argv)
{
	_serial = open_port();
	configure_port(_serial);

	char a=0,b=0,beat[STRLEN];
	int i=-1,j=-1;
	bool staccato = false;
	FILE* fp = fopen(argv[1], "r");
	while(1){
	nextInt(fp,&i);nextStr(fp,beat);
	std::cout << i << " " << beat << std::endl;
	for(int k = 0; k < 12; k++)
	{
		if(strcmp(beat, Beats[k]) == 0) {j=k;break;}
	}
	std::cout << i << " " << j << std::endl;
	a=i/256;
	b=i%256;
	send(&zero);
	send(&a);
	send(&b);
	usleep(delays[j]);
	send(&zero);
	send(&zero);
	send(&zero);
	usleep(1000);
	}
	fclose(fp);
	send(&zero);
	send(&zero);
	send(&zero);
	send(&one);
	send(&zero);
	send(&zero);
	close(_serial);
 	return 0;
}

