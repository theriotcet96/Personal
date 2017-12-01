#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include <iostream>
#include <stdlib.h>

#define STRLEN 22

char NotesSharp[91][STRLEN] = {
  "Rest"
  "Peddle_C",
  "Peddle_C#",
  "Peddle_D",
  "Peddle_D#",
  "Peddle_E",
  "Peddle_F",
  "Peddle_F#",
  "Peddle_G",
  "Peddle_G#",
  "Peddle_A",
  "Peddle_A#",
  "Peddle_B",
  "Deep_C",
  "Deep_C#",
  "Deep_D",
  "Deep_D#",
  "Deep_E",
  "Deep_F",
  "Deep_F#",
  "Deep_G",
  "Deep_G#",
  "Deep_A",
  "Deep_A#",
  "Deep_B",
  "Low_C",
  "Low_C#",
  "Low_D",
  "Low_D#",
  "Low_E",
  "Low_F",
  "Low_F#",
  "Low_G",
  "Low_G#",
  "Low_A",
  "Low_A#",
  "Low_B",
  "Middle_C",
  "Middle_C#",
  "Middle_D",
  "Middle_D#",
  "Middle_E",
  "Middle_F",
  "Middle_F#",
  "Middle_G",
  "Middle_G#",
  "Middle_A",
  "Middle_A#",
  "Middle_B",
  "Tenor_C",
  "Tenor_C#",
  "Tenor_D",
  "Tenor_D#",
  "Tenor_E",
  "Tenor_F",
  "Tenor_F#",
  "Tenor_G",
  "Tenor_G#",
  "Tenor_A",
  "Tenor_A#",
  "Tenor_B",
  "High_C",
  "High_C#",
  "High_D",
  "High_D#",
  "High_E",
  "High_F",
  "High_F#",
  "High_G",
  "High_G#",
  "High_A",
  "High_A#",
  "High_B",
  "Double_High_C",
  "Double_High_C#",
  "Double_High_D",
  "Double_High_D#",
  "Double_High_E",
  "Double_High_F",
  "Double_High_F#",
  "Double_High_G",
  "Double_High_G#",
  "Double_High_A",
  "Double_High_A#",
  "Double_High_B",
  "Octave_C",
  "Octave_C#",
  "Octave_D",
  "Octave_D#",
  "Octave_E",
  "Octave_F"
};
char NotesFlat[91][STRLEN] = {
  "Rest"
  "Peddle_C",
  "Peddle_Db",
  "Peddle_D",
  "Peddle_Eb",
  "Peddle_E",
  "Peddle_F",
  "Peddle_Gb",
  "Peddle_G",
  "Peddle_Ab",
  "Peddle_A",
  "Peddle_Bb",
  "Peddle_B",
  "Deep_C",
  "Deep_Db",
  "Deep_D",
  "Deep_Eb",
  "Deep_E",
  "Deep_F",
  "Deep_Gb",
  "Deep_G",
  "Deep_Ab",
  "Deep_A",
  "Deep_Bb",
  "Deep_B",
  "Low_C",
  "Low_Db",
  "Low_D",
  "Low_Eb",
  "Low_E",
  "Low_F",
  "Low_Gb",
  "Low_G",
  "Low_Ab",
  "Low_A",
  "Low_Bb",
  "Low_B",
  "Middle_C",
  "Middle_Db",
  "Middle_D",
  "Middle_Eb",
  "Middle_E",
  "Middle_F",
  "Middle_Gb",
  "Middle_G",
  "Middle_Ab",
  "Middle_A",
  "Middle_Bb",
  "Middle_B",
  "Tenor_C",
  "Tenor_Db",
  "Tenor_D",
  "Tenor_Eb",
  "Tenor_E",
  "Tenor_F",
  "Tenor_Gb",
  "Tenor_G",
  "Tenor_Ab",
  "Tenor_A",
  "Tenor_Bb",
  "Tenor_B",
  "High_C",
  "High_Db",
  "High_D",
  "High_Eb",
  "High_E",
  "High_F",
  "High_Gb",
  "High_G",
  "High_Ab",
  "High_A",
  "High_Bb",
  "High_B",
  "Double_High_C",
  "Double_High_Db",
  "Double_High_D",
  "Double_High_Eb",
  "Double_High_E",
  "Double_High_F",
  "Double_High_Gb",
  "Double_High_G",
  "Double_High_Ab",
  "Double_High_A",
  "Double_High_Bb",
  "Double_High_B",
  "Octave_C",
  "Octave_Db",
  "Octave_D",
  "Octave_Eb",
  "Octave_E",
  "Octave_F"
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

void send(char* c)
{
  write(_serial, c, 1);
}
int main(int argc, char** argv)
{

  _serial = open_port();
  configure_port(_serial);
  char zero = 0;
  int delays[12] = {500000, 250000, 167000, 125000, 1000000, 2000000, 750000, 375000, 250000, 187000, 1500000, 3000000};

  for(;;)
  {
	std::cout << "Enter the filename of the song" << std::endl;
	char filename[20];
	scanf("%s", filename);
	if(strcmp(filename, "quit") == 0) {close(_serial);return 0;}
	FILE* song = fopen(filename, "r");
	while(!feof(song))
	{
		char a=0, b=0;
		int d=-1;
		char _note[STRLEN], _beat[STRLEN];
		fscanf(song, "%s", _note);
		fscanf(song, "%s", _beat);
		for(int i = 0; i < 91; i++)
		{
			if(strcmp(_note, NotesSharp[i]) == 0 ||
			   strcmp(_note, NotesFlat[i]) == 0)
			{
				a=i/256;
				b=i%256;break;
			}
		}
		for(int i = 0; i < 12; i++)
		{
			if(strcmp(_beat, Beats[i]) == 0)
			{
				d=i; break;
			}
		}
		send(&a);
		send(&b);
		usleep(delays[d]);
		send(&zero);
		send(&zero);
	}
	fclose(song);
  }


  close(_serial);
  return 0;
}

