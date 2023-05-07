#include <stdio.h>

typedef struct
{
	unsigned char chunkID[4];
	unsigned char format[4];
	unsigned char subChunk1ID[4];
	unsigned char subChunk2ID[4];
}BYTE;

typedef struct
{
	unsigned short int audioFormat;
	unsigned short int NumChannels;
	unsigned short int blockAlign;
	unsigned short int bitsPerSample;	
}WORD;
typedef struct
{
	unsigned int chunkSize;
	unsigned int subChunk1Size;
	unsigned int sampleRate;
	unsigned int byteRate;
	unsigned int subChunk2Size;
}DWORD;

int header_checker(BYTE byte, WORD word, DWORD dword, unsigned int NumSamples);

void list(char *file_input);

void reverse(char *file_input, char *file_output);

void mono(char *file_input, char *file_output);

void crop(char *file_input, char *file_output, int *sec);
