#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <pthread.h>

#define STRLEN 22
int TEMPO = 200;
#define BEAT (60000000/TEMPO)
#define SERIAL_PORT_1 "/dev/ttyACM0"
#define SERIAL_PORT_2 "/dev/ttyACM1"

int open_port(const char* port)
{
  int location;
  location = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
  if(location == -1)
  {
    printf("open_port: Unable to open %s.\n", port);
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

int treble_serial, bass_serial;
char zero = 0;
char one = 1;
double delays[18] = {
 1, 0.5, 0.25, 0.125, 2, 4,
 1.5, 0.75, 0.375, 0.1875, 3, 6,
 0.666666, 0.333333, 0.1666666, 0.08333333, 1.333333, 2.666666
};

void set(int port, int n)
{
  char a=n/256,b=n%256;
  write(port, &a, 1);
  write(port, &b, 1);
}
char Beats[18][STRLEN] = {"1/4", "1/8", "1/16", "1/32", "1/2", "1",
			"1/4.", "1/8.", "1/16.", "1/32.", "1/2.", "1.",
			"triplet/4", "triplet/8", "triplet/16", "triplet/32", "triplet/2", "triplet"};
int closed_threads = 0;
typedef struct data {
 char* string;
 int port;
} data;
void* play(void* arg)
{
  const char* argv = ((struct data*)arg)->string;
  int port = ((struct data*)arg)->port;

   FILE* fp = fopen(argv, "r");

 while(1)
 {
  int n;
  char ch[STRLEN];
  fscanf(fp, "%d", &n);
  set(port, n);
  fscanf(fp, "%s", ch);
  if(strcmp(ch, "END") == 0) break;
  for(int i = 0; i < 18; i++)
  {
    if(strcmp(ch, Beats[i]) == 0)
    {
      usleep(delays[i] * BEAT);break;
    }
  }
 }

 fclose(fp);

 closed_threads++;
  return NULL;
}
int main(int argc, char** argv)
{
 if(argc > 3)
 {
   TEMPO = atoi(argv[3]);
 }
 treble_serial = open_port(SERIAL_PORT_1);
 bass_serial = open_port(SERIAL_PORT_2);
 configure_port(treble_serial);
 configure_port(bass_serial);

 data datums[2];
 datums[0].string = argv[1];
 datums[1].string = argv[2];
 datums[0].port = treble_serial;
 datums[1].port = bass_serial;

 pthread_t threads[2];
 pthread_create( &threads[0], NULL, play, &datums[0] );
 pthread_create( &threads[1], NULL, play, &datums[1] );
 pthread_join( threads[0], NULL );
 pthread_join( threads[1], NULL );

 while(closed_threads < 2);
 close(treble_serial);
 close(bass_serial);
 return 0;
}
