#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include <iostream>

#define STRLEN 22

char NotesSharp[91][STRLEN] = {
  "Rest"
  "Peddle C",
  "Peddle C#",
  "Peddle D",
  "Peddle D#",
  "Peddle E",
  "Peddle F",
  "Peddle F#",
  "Peddle G",
  "Peddle G#",
  "Peddle A",
  "Peddle A#",
  "Peddle B",
  "Deep C",
  "Deep C#",
  "Deep D",
  "Deep D#",
  "Deep E",
  "Deep F",
  "Deep F#",
  "Deep G",
  "Deep G#",
  "Deep A",
  "Deep A#",
  "Deep B",
  "Low C",
  "Low C#",
  "Low D",
  "Low D#",
  "Low E",
  "Low F",
  "Low F#",
  "Low G",
  "Low G#",
  "Low A",
  "Low A#",
  "Low B",
  "Middle C",
  "Middle C#",
  "Middle D",
  "Middle D#",
  "Middle E",
  "Middle F",
  "Middle F#",
  "Middle G",
  "Middle G#",
  "Middle A",
  "Middle A#",
  "Middle B",
  "Tenor C",
  "Tenor C#",
  "Tenor D",
  "Tenor D#",
  "Tenor E",
  "Tenor F",
  "Tenor F#",
  "Tenor G",
  "Tenor G#",
  "Tenor A",
  "Tenor A#",
  "Tenor B",
  "High C",
  "High C#",
  "High D",
  "High D#",
  "High E",
  "High F",
  "High F#",
  "High G",
  "High G#",
  "High A",
  "High A#",
  "High B",
  "Double High C",
  "Double High C#",
  "Double High D",
  "Double High D#",
  "Double High E",
  "Double High F",
  "Double High F#",
  "Double High G",
  "Double High G#",
  "Double High A",
  "Double High A#",
  "Double High B",
  "Octave C",
  "Octave C#",
  "Octave D",
  "Octave D#",
  "Octave E",
  "Octave F"
};
char Beats[12][STRLEN] = {
  "1",
  "1/8",
  "1/16",
  "1/32",
  "2",
  "4",
  "1.",
  "1/8.",
  "1/16.",
  "1/32.",
  "2.",
  "4."
};

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

int main(int argc, char** argv)
{

  _serial = open_port();
  configure_port(_serial);
  char zero = 0;

  char turkish_march_notes[][STRLEN] =
  {
   "Middle B", "Middle A", "Middle G#", "Middle A",
   "Tenor C", "Rest", "Tenor D", "Tenor C", "Middle B", "Tenor C",
   "Tenor E", "Rest", "Tenor F", "Tenor E", "Tenor D#", "Tenor E",
   "Tenor B", "Tenor A", "Tenor G#", "Tenor A", "Tenor B", "Tenor A", "Tenor G#", "Tenor A",
   "High C", "Tenor A", "High C",
   "Tenor B", "Tenor A", "Tenor G", "Tenor A",
   "Tenor B", "Tenor A", "Tenor G", "Tenor A",
   "Tenor B", "Tenor A", "Tenor G", "Tenor F#",
   "Tenor E"
  };
  bool turkish_march_staccato[]=
  {
	false, false, false, false,
	true, false, false, false, false, false,
	true, false, false, false, false, false,
	false, false, false, false, false, false, false, false,
	false, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	false
  };
  char turkish_march_beats[][STRLEN] =
  {
    "1/16", "1/16", "1/16", "1/16",
    "1/8", "1/8", "1/16", "1/16", "1/16", "1/16",
    "1/8", "1/8", "1/16", "1/16", "1/16", "1/16",
    "1/16", "1/16", "1/16", "1/16", "1/16", "1/16", "1/16", "1/16",
    "1", "1/8", "1/8",
    "1/8", "1/8", "1/8", "1/8",
    "1/8", "1/8", "1/8", "1/8",
    "1/8", "1/8", "1/8", "1/8",
    "1"
  };


  int delays[12] = {500000, 250000, 167000, 125000, 1000000, 2000000, 750000, 375000, 250000, 187000, 1500000, 3000000};

  for(int i = 0; i < 40; i++)
  {
    char a=0, b=0, c=0, d=0;
    for(int j = 0; j < 91; j++)
    {
      if(strcmp(_notes[i], NotesSharp[j]) == 0)
      {
        a=j/256;
	b=j%256;
        break;
      }
    }
    for(int j = 0; j < 12; j++)
    {
      if(strcmp(_beats[i], Beats[j]) == 0)
      {
	c=j/256;
	d=j%256;
	break;
      }
    }
    write(_serial, &a, 1);
    write(_serial, &b, 1);
    write(_serial, &c, 1);
    write(_serial, &d, 1);
    int spare = _staccato[i] ? 100000 : 0;
    usleep(delays[c * 256 + d] - spare);
    if(_staccato[i])
    {
      write(_serial, &zero, 1);
      write(_serial, &zero, 1);
      write(_serial, &zero, 1);
      write(_serial, &zero, 1);
      usleep(spare);
    }
  }

  write(_serial, &zero, 1);
  write(_serial, &zero, 1);
  write(_serial, &zero, 1);
  write(_serial, &zero, 1);
  close(_serial);
  return 0;
}

