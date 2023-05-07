#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "music.h"
#include "header.h"

int header_checker(BYTE byte, WORD word, DWORD dword, unsigned int NumSamples)
{
	if(strncmp(byte.chunkID,"RIFF",4)) //RIFF
	{
		printf("\033[0;31m");
		printf("[-]");
		printf("\033[0m");
		printf("Syntax Error in bytes (1-4)\n");
		return 1;
	}
	else if(strncmp(byte.format,"WAVE",4)) //FORMAT
	{
		printf("\033[0;31m");
		printf("[-]");
		printf("\033[0m");
		printf("Syntax Error in bytes (9-12)\n");
		return 1;
	}
	else if(strncmp(byte.subChunk1ID,"fmt",3)) // SUBCHUNK1ID
	{
		printf("\033[0;31m");
		printf("[-]");
		printf("\033[0m");
		printf("Syntax Error in bytes (13-16)\n");
		return 1;
	}
	else if(dword.subChunk1Size != 16) //SUBCHUNK1SIZE
	{
		printf("\033[0;31m");
		printf("[-]");
		printf("\033[0m");
		printf("Syntax Error in bytes (17-20)\n");
		return 1;
	}
	else if(word.audioFormat != 1 ) //AUDIOFORMAT 
	{
		printf("\033[0;31m");
		printf("[-]");
		printf("\033[0m");
		printf("Syntax Error in bytes (21-22)\n");
		return 1;
	}
	else if(word.NumChannels !=1 && word.NumChannels != 2) //NUMCHANNELS
	{
		printf("\033[0;31m");
		printf("[-]");
		printf("\033[0m");
		printf("Syntax Error in bytes (23-24)\n");
		return 1;
	}
	else if (dword.byteRate != (dword.sampleRate * word.NumChannels *  word.bitsPerSample) / 8) //BYTERATE
	{	
		printf("%u %u\n", dword.byteRate, (dword.sampleRate * word.NumChannels *  word.bitsPerSample) / 8);
		printf("\033[0;31m");
		printf("[-]");
		printf("\033[0m");
		printf("Syntax Error in bytes (29-32)\n");
		return 1;
	}
	else if((double)word.blockAlign != (word.NumChannels * word.bitsPerSample)/8.0) //BLOCKALIGN
	{
		printf("%1f %1f\n",(double)word.blockAlign,(word.NumChannels * word.bitsPerSample)/8.0);
		printf("\033[0;31m");
		printf("[-]");
		printf("\033[0m");
		printf("Syntax Error in bytes (33-34)\n");
		return 1;	
	}
	unsigned short int temp = word.bitsPerSample;
	while(temp / 10 != 0)	
		temp /= 2.0;
	if (temp != 8 && temp != 4 && temp != 2 && temp != 1) // BITSPERSAMPLE
	{
		printf("\033[0;31m");
		printf("[-]");
		printf("\033[0m");
		printf("Syntax Error in bytes (35-36)\n");
		return 1;
	}
	else if (strncmp(byte.subChunk2ID,"data",4)) //SUBCHUNK2ID
	{
		printf("\033[0;31m");
		printf("[-]");
		printf("\033[0m");
		printf("Syntax Error in bytes (37-40)\n");
		return 1;
	}
	else if( dword.subChunk2Size != (NumSamples * word.NumChannels * word.bitsPerSample)/8 ) //SUBCHUNK2SIZE
	{
		printf("\033[0;31m");
		printf("[-]");
		printf("\033[0m");
		printf("Syntax Error in bytes (41-44)\n");
		printf("%u != %f * %u * %u / 8\n", dword.subChunk2Size, NumSamples, word.NumChannels, word.bitsPerSample);
		return 1;
	}
	return 0;
}
	

// Option list
void list(char *file_input)
{
	unsigned char buffer_4bytes[4], buffer_2bytes[2];
	WORD word;
	DWORD dword;
	BYTE byte;
	FILE *myFile;
	int i, j = 0;
	myFile = fopen(file_input, "rb");
	
	printf("\033[0;32m");
	printf("[+]");
	printf("\033[0m");
	printf(" Opening the file...\n\n");
	time_sleep(2.5);
	
	if( !myFile )
	{
		
		printf("\033[0;31m");
		printf("[-]");
		printf("\033[0m");
		printf(" Error opening the file \n");
		return;
	}
	j = fread(byte.chunkID, sizeof(byte.chunkID), 1, myFile); //CHUNKID
	
	j = fread(buffer_4bytes, sizeof(buffer_4bytes), 1, myFile);  //CHUNKSIZE		
	dword.chunkSize = hex_to_int(buffer_4bytes[3], buffer_4bytes[2], buffer_4bytes[1], buffer_4bytes[0], 4);		
	
	j = fread(byte.format, sizeof(byte.format), 1, myFile);  //FORMAT
	
	j = fread(byte.subChunk1ID, sizeof(byte.subChunk1ID), 1, myFile); //SUBCHUNK1ID
	
	j = fread(buffer_4bytes, sizeof(buffer_4bytes), 1, myFile);    //SUBCHUNK1SIZE
	dword.subChunk1Size = hex_to_int(buffer_4bytes[3], buffer_4bytes[2], buffer_4bytes[1], buffer_4bytes[0], 4);
	
	j = fread(buffer_2bytes, sizeof(buffer_2bytes), 1, myFile); //AUDIOFORMAT
	word.audioFormat =(unsigned short int) hex_to_int(buffer_2bytes[1], buffer_2bytes[0], 0, 0, 2);

	j = fread(buffer_2bytes, sizeof(buffer_2bytes), 1, myFile); //numChannels
	word.NumChannels =(unsigned short int) hex_to_int(buffer_2bytes[1], buffer_2bytes[0], 0, 0, 2);

	j = fread(buffer_4bytes, sizeof(buffer_4bytes), 1, myFile);  //samplerate		
	dword.sampleRate = hex_to_int(buffer_4bytes[3], buffer_4bytes[2], buffer_4bytes[1], buffer_4bytes[0], 4);		
	
	j = fread(buffer_4bytes,sizeof(buffer_4bytes),1,myFile); //byterate
	dword.byteRate = hex_to_int(buffer_4bytes[3],buffer_4bytes[2],buffer_4bytes[1],buffer_4bytes[0],4);
	
	j = fread(buffer_2bytes,sizeof(buffer_2bytes),1,myFile); //blockAlign
	word.blockAlign = (unsigned short int)hex_to_int(buffer_2bytes[1],buffer_2bytes[0],0,0,2);
	
	j = fread(buffer_2bytes,sizeof(buffer_2bytes),1,myFile); //BitsPerSample
	word.bitsPerSample =(unsigned short int)hex_to_int(buffer_2bytes[1],buffer_2bytes[0],0,0,2);
	
	j = fread(byte.subChunk2ID,sizeof(byte.subChunk2ID),1,myFile); //SubChunk2ID
	
	
	j = fread(buffer_4bytes,sizeof(buffer_4bytes),1,myFile); //SubChunk2Size 
	dword.subChunk2Size = hex_to_int(buffer_4bytes[3],buffer_4bytes[2],buffer_4bytes[1],buffer_4bytes[0],4);
	
	unsigned int  NumSamples =  dword.subChunk2Size*8/(word.NumChannels*word.bitsPerSample);
	
	
	if( !header_checker(byte, word, dword, NumSamples) )
	{
		printf("RIFF_CHUNK_HEADER\n");
		printf("=================\n");
		printf("chunkID: ");
		for(i=0;i<4;i++) printf("%c",byte.chunkID[i]);
		printf("\n");
		printf("chunkSize: %u\n", dword.chunkSize);
		printf("format: ");
		for(i=0;i<4;i++) printf("%c",byte.format[i]);
		printf("\n");	
		printf("\nFMT_SUBCHUNK_HEADER\n");
		printf("===================\n");
		printf("subChunk1ID: ");
		for(i=0;i<4;i++) printf("%c",byte.subChunk1ID[i]);
		printf("\n");
		printf("subChunk1Size: %u\n", dword.subChunk1Size);
		printf("audioFormat: %u\n", word.audioFormat);
		printf("numChannels: %u\n",word.NumChannels);
		printf("sampleRate: %u\n", dword.sampleRate);
		printf("byteRate: %u\n",dword.byteRate);
		printf("blockAlign: %u\n",word.blockAlign);
		printf("bitsPerSample: %u\n\n",word.bitsPerSample);
		printf("DATA_SUBCHUNK_HEADER\n");
		printf("====================\n");
		printf("subChunk2ID: ");
		for(i=0;i<4;i++) printf("%c",byte.subChunk2ID[i]);
		printf("\n");
		printf("subChunk2Size: %u\n",dword.subChunk2Size);
	}
	fclose(myFile);	
}

// Option reverse
void reverse(char* file_input, char* file_output)
{
	FILE *filei, *fileo;
	unsigned char buffer_4bytes[4], buffer_2bytes[2];
	WORD word;
	DWORD dword;
	BYTE byte;

	int i, j = 0;
	filei = fopen(file_input, "rb");
	
	printf("\033[0;32m");
	printf("[+]");
	printf("\033[0m");
	printf(" Opening the file...\n\n");
	time_sleep(2.5);

	if (!filei)
	{

		printf("\033[0;31m");
		printf("[-]");
		printf("\033[0m");
		printf(" Error opening the file \n");
		return;
	}
	
	j = fread(byte.chunkID, sizeof(byte.chunkID), 1, filei); //CHUNKID

	j = fread(buffer_4bytes, sizeof(buffer_4bytes), 1, filei);  //CHUNKSIZE		
	dword.chunkSize = hex_to_int(buffer_4bytes[3], buffer_4bytes[2], buffer_4bytes[1], buffer_4bytes[0], 4);

	j = fread(byte.format, sizeof(byte.format), 1, filei);  //FORMAT

	j = fread(byte.subChunk1ID, sizeof(byte.subChunk1ID), 1, filei); //SUBCHUNK1ID

	j = fread(buffer_4bytes, sizeof(buffer_4bytes), 1, filei);    //SUBCHUNK1SIZE
	dword.subChunk1Size = hex_to_int(buffer_4bytes[3], buffer_4bytes[2], buffer_4bytes[1], buffer_4bytes[0], 4);

	j = fread(buffer_2bytes, sizeof(buffer_2bytes), 1, filei); //AUDIOFORMAT
	word.audioFormat = (unsigned short int) hex_to_int(buffer_2bytes[1], buffer_2bytes[0], 0, 0, 2);

	j = fread(buffer_2bytes, sizeof(buffer_2bytes), 1, filei); //numChannels
	word.NumChannels = (unsigned short int) hex_to_int(buffer_2bytes[1], buffer_2bytes[0], 0, 0, 2);

	j = fread(buffer_4bytes, sizeof(buffer_4bytes), 1, filei);  //samplerate		
	dword.sampleRate = hex_to_int(buffer_4bytes[3], buffer_4bytes[2], buffer_4bytes[1], buffer_4bytes[0], 4);

	j = fread(buffer_4bytes, sizeof(buffer_4bytes), 1, filei); //byterate
	dword.byteRate = hex_to_int(buffer_4bytes[3], buffer_4bytes[2], buffer_4bytes[1], buffer_4bytes[0], 4);

	j = fread(buffer_2bytes, sizeof(buffer_2bytes), 1, filei); //blockAlign
	word.blockAlign = (unsigned short int)hex_to_int(buffer_2bytes[1], buffer_2bytes[0], 0, 0, 2);

	j = fread(buffer_2bytes, sizeof(buffer_2bytes), 1, filei); //BitsPerSample
	word.bitsPerSample = (unsigned short int)hex_to_int(buffer_2bytes[1], buffer_2bytes[0], 0, 0, 2);

	j = fread(byte.subChunk2ID, sizeof(byte.subChunk2ID), 1, filei); //SubChunk2ID


	j = fread(buffer_4bytes, sizeof(buffer_4bytes), 1, filei); //SubChunk2Size 
	dword.subChunk2Size = hex_to_int(buffer_4bytes[3], buffer_4bytes[2], buffer_4bytes[1], buffer_4bytes[0], 4);


	unsigned int NumSamples = (dword.subChunk2Size * 8) / (word.NumChannels * word.bitsPerSample);
	if (!header_checker(byte, word, dword, NumSamples))
	{
		int k;
		char **sound;


		sound = (char**)malloc(sizeof(char*) * NumSamples);
		for (i = 0; i < NumSamples; i++)
			sound[i] = (char*)malloc(sizeof(char) * word.blockAlign);
		
		for (i = 0; i < NumSamples; i++)
			k = fread(sound[i], sizeof(char), word.blockAlign, filei);
		
		fclose(filei);
		
		fileo = fopen(file_output, "wb");
		j = fwrite(byte.chunkID, sizeof(byte.chunkID), 1, fileo); //CHUNKID

		j = fwrite(&dword.chunkSize, sizeof(dword.chunkSize), 1, fileo);  //CHUNKSIZE

		j = fwrite(byte.format, sizeof(byte.format), 1, fileo);  //FORMAT

		j = fwrite(byte.subChunk1ID, sizeof(byte.subChunk1ID), 1, fileo); //SUBCHUNK1ID

		j = fwrite(&dword.subChunk1Size, sizeof(dword.subChunk1Size), 1, fileo);    //SUBCHUNK1SIZE

		j = fwrite(&word.audioFormat, sizeof(word.audioFormat), 1, fileo); //AUDIOFORMAT

		j = fwrite(&word.NumChannels, sizeof(word.NumChannels), 1, fileo); //numChannels

		j = fwrite(&dword.sampleRate, sizeof(dword.sampleRate), 1, fileo);  //samplerate

		j = fwrite(&dword.byteRate, sizeof(dword.byteRate), 1, fileo); //byterate

		j = fwrite(&word.blockAlign, sizeof(word.blockAlign), 1, fileo); //blockAlign

		j = fwrite(&word.bitsPerSample, sizeof(word.bitsPerSample), 1, fileo); //BitsPerSample

		j = fwrite(byte.subChunk2ID, sizeof(byte.subChunk2ID), 1, fileo); //SubChunk2ID

		j = fwrite(&dword.subChunk2Size, sizeof(dword.subChunk2Size), 1, fileo); //SubChunk2Size 
	
		
		for (i = 0; i < NumSamples; i++)
			k = fwrite(sound[NumSamples -1 -i], sizeof(char), word.blockAlign, fileo);
			
		fclose(fileo);
		printf("\033[0;32m");
		printf("[+]");
		printf("\033[0m");
		printf(" Creating the file %s\n", file_output);
		time_sleep(2.5);
		for(i = 0; i < NumSamples; i++)
			free(sound[i]);
		free(sound);
	}
}

void mono(char *file_input, char *file_output)
{
	FILE *filei, *fileo;
	unsigned char buffer_4bytes[4], buffer_2bytes[2];
	WORD word;
	DWORD dword;
	BYTE byte;
	
	int i, j = 0;
	filei = fopen(file_input, "rb");
	
	printf("\033[0;32m");
	printf("[+]");
	printf("\033[0m");
	printf(" Opening the file...\n\n");
	time_sleep(2.5);
	
	if( !filei )
	{
		
		printf("\033[0;31m");
		printf("[-]");
		printf("\033[0m");
		printf(" Error opening the file \n");
		return;
	}
	
	j = fread(byte.chunkID, sizeof(byte.chunkID), 1, filei); //CHUNKID
	
	j = fread(buffer_4bytes, sizeof(buffer_4bytes), 1, filei);  //CHUNKSIZE		
	dword.chunkSize = hex_to_int(buffer_4bytes[3], buffer_4bytes[2], buffer_4bytes[1], buffer_4bytes[0], 4);		
	
	j = fread(byte.format, sizeof(byte.format), 1, filei);  //FORMAT
	
	j = fread(byte.subChunk1ID, sizeof(byte.subChunk1ID), 1, filei); //SUBCHUNK1ID
	
	j = fread(buffer_4bytes, sizeof(buffer_4bytes), 1, filei);    //SUBCHUNK1SIZE
	dword.subChunk1Size = hex_to_int(buffer_4bytes[3], buffer_4bytes[2], buffer_4bytes[1], buffer_4bytes[0], 4);
	
	j = fread(buffer_2bytes, sizeof(buffer_2bytes), 1, filei); //AUDIOFORMAT
	word.audioFormat =(unsigned short int) hex_to_int(buffer_2bytes[1], buffer_2bytes[0], 0, 0, 2);

	j = fread(buffer_2bytes, sizeof(buffer_2bytes), 1, filei); //numChannels
	word.NumChannels =(unsigned short int) hex_to_int(buffer_2bytes[1], buffer_2bytes[0], 0, 0, 2);
	if( word.NumChannels == 1 )
	{
		printf("\033[0;31m");
		printf("[-]");
		printf("\033[0m");
		printf(" Error ");
		printf("The file %s is already mono.\n", file_input);
		return;
	}	

	j = fread(buffer_4bytes, sizeof(buffer_4bytes), 1, filei);  //samplerate		
	dword.sampleRate = hex_to_int(buffer_4bytes[3], buffer_4bytes[2], buffer_4bytes[1], buffer_4bytes[0], 4);		
	
	j = fread(buffer_4bytes,sizeof(buffer_4bytes),1,filei); //byterate
	dword.byteRate = hex_to_int(buffer_4bytes[3],buffer_4bytes[2],buffer_4bytes[1],buffer_4bytes[0],4);
	
	j = fread(buffer_2bytes,sizeof(buffer_2bytes),1,filei); //blockAlign
	word.blockAlign = (unsigned short int)hex_to_int(buffer_2bytes[1],buffer_2bytes[0],0,0,2);
	
	j = fread(buffer_2bytes,sizeof(buffer_2bytes),1,filei); //BitsPerSample
	word.bitsPerSample =(unsigned short int)hex_to_int(buffer_2bytes[1],buffer_2bytes[0],0,0,2);
	
	j = fread(byte.subChunk2ID,sizeof(byte.subChunk2ID),1,filei); //SubChunk2ID
	
	
	j = fread(buffer_4bytes,sizeof(buffer_4bytes),1,filei); //SubChunk2Size 
	dword.subChunk2Size = hex_to_int(buffer_4bytes[3],buffer_4bytes[2],buffer_4bytes[1],buffer_4bytes[0],4);


	unsigned int NumSamples = (dword.subChunk2Size * 8) / (word.NumChannels * word.bitsPerSample) ;
	
	if( !header_checker(byte, word, dword, NumSamples) )
	{
		int k;
		char **sound;
		word.NumChannels = 1;
		dword.byteRate = dword.sampleRate * word.bitsPerSample /8; 
		word.blockAlign = word.bitsPerSample / 8;
		dword.subChunk2Size = (NumSamples * word.bitsPerSample)/8;		
		sound = (char **)malloc(sizeof(char *) * NumSamples);
		
			
		for(i = 0; i < NumSamples; i++)
			sound[i] = (char *)malloc(sizeof(char) * word.blockAlign );
		for(i = 0; i < NumSamples; i++)
		{
			k = fread(sound[i], sizeof(char)*sizeof(char)*word.blockAlign, 1, filei);
			
		}
		fclose(filei);

		fileo = fopen(file_output, "wb");
			
		j = fwrite(byte.chunkID, sizeof(byte.chunkID), 1, fileo); //CHUNKID
		
		j = fwrite(&dword.chunkSize, sizeof(dword.chunkSize), 1, fileo);  //CHUNKSIZE
				
		j = fwrite(byte.format, sizeof(byte.format), 1, fileo);  //FORMAT
		
		j = fwrite(byte.subChunk1ID, sizeof(byte.subChunk1ID), 1, fileo); //SUBCHUNK1ID
		
		j = fwrite(&dword.subChunk1Size, sizeof(dword.subChunk1Size), 1, fileo);    //SUBCHUNK1SIZE
		
		j = fwrite(&word.audioFormat, sizeof(word.audioFormat), 1, fileo); //AUDIOFORMAT
		
		j = fwrite(&word.NumChannels, sizeof(word.NumChannels), 1, fileo); //numChannels
		
		j = fwrite(&dword.sampleRate, sizeof(dword.sampleRate), 1, fileo);  //samplerate
				
		j = fwrite(&dword.byteRate,sizeof(dword.byteRate),1,fileo); //byterate
		
		j = fwrite(&word.blockAlign,sizeof(word.blockAlign),1,fileo); //blockAlign
		
		j = fwrite(&word.bitsPerSample,sizeof(word.bitsPerSample),1,fileo); //BitsPerSample
		
		j = fwrite(byte.subChunk2ID,sizeof(byte.subChunk2ID),1,fileo); //SubChunk2ID
		
		j = fwrite(&dword.subChunk2Size,sizeof(dword.subChunk2Size),1,fileo); //SubChunk2Size 
	
		for(i = 0; i < NumSamples; i++)
			k = fwrite(sound[i], sizeof(char), sizeof(char)*word.blockAlign, fileo);
			
		printf("\033[0;32m");
		printf("[+]");
		printf("\033[0m");
		printf(" Creating the file %s\n", file_output);
		time_sleep(2.5);		
		fclose(fileo);
		for(i = 0; i < NumSamples; i++)
			free(sound[i]);
		free(sound);
	}
}

void crop(char *file_input, char *file_output, int *sec)
{
	FILE *filei, *fileo;
	unsigned char buffer_4bytes[4], buffer_2bytes[2];
	WORD word;
	DWORD dword;
	BYTE byte;
	
	int i, j = 0;
	filei = fopen(file_input, "rb");
	
	printf("\033[0;32m");
	printf("[+]");
	printf("\033[0m");
	printf(" Opening the file...\n\n");
	time_sleep(2.5);
	
	if( !filei )
	{
		
		printf("\033[0;31m");
		printf("[-]");
		printf("\033[0m");
		printf(" Error opening the file \n");
		return;
	}

	j = fread(byte.chunkID, sizeof(byte.chunkID), 1, filei); //CHUNKID
	
	j = fread(buffer_4bytes, sizeof(buffer_4bytes), 1, filei);  //CHUNKSIZE		
	dword.chunkSize = hex_to_int(buffer_4bytes[3], buffer_4bytes[2], buffer_4bytes[1], buffer_4bytes[0], 4);		
	
	j = fread(byte.format, sizeof(byte.format), 1, filei);  //FORMAT
	
	j = fread(byte.subChunk1ID, sizeof(byte.subChunk1ID), 1, filei); //SUBCHUNK1ID
	
	j = fread(buffer_4bytes, sizeof(buffer_4bytes), 1, filei);    //SUBCHUNK1SIZE
	dword.subChunk1Size = hex_to_int(buffer_4bytes[3], buffer_4bytes[2], buffer_4bytes[1], buffer_4bytes[0], 4);
	
	j = fread(buffer_2bytes, sizeof(buffer_2bytes), 1, filei); //AUDIOFORMAT
	word.audioFormat =(unsigned short int) hex_to_int(buffer_2bytes[1], buffer_2bytes[0], 0, 0, 2);

	j = fread(buffer_2bytes, sizeof(buffer_2bytes), 1, filei); //numChannels
	word.NumChannels =(unsigned short int) hex_to_int(buffer_2bytes[1], buffer_2bytes[0], 0, 0, 2);
	if( word.NumChannels == 1 )
	{
		printf("\033[0;31m");
		printf("[-]");
		printf("\033[0m");
		printf(" Error ");
		printf("The file %s is already mono.\n", file_input);
		return;
	}	

	j = fread(buffer_4bytes, sizeof(buffer_4bytes), 1, filei);  //samplerate		
	dword.sampleRate = hex_to_int(buffer_4bytes[3], buffer_4bytes[2], buffer_4bytes[1], buffer_4bytes[0], 4);		
	
	j = fread(buffer_4bytes,sizeof(buffer_4bytes),1,filei); //byterate
	dword.byteRate = hex_to_int(buffer_4bytes[3],buffer_4bytes[2],buffer_4bytes[1],buffer_4bytes[0],4);
	
	j = fread(buffer_2bytes,sizeof(buffer_2bytes),1,filei); //blockAlign
	word.blockAlign = (unsigned short int)hex_to_int(buffer_2bytes[1],buffer_2bytes[0],0,0,2);
	
	j = fread(buffer_2bytes,sizeof(buffer_2bytes),1,filei); //BitsPerSample
	word.bitsPerSample =(unsigned short int)hex_to_int(buffer_2bytes[1],buffer_2bytes[0],0,0,2);
	
	j = fread(byte.subChunk2ID,sizeof(byte.subChunk2ID),1,filei); //SubChunk2ID
	
	
	j = fread(buffer_4bytes,sizeof(buffer_4bytes),1,filei); //SubChunk2Size 
	dword.subChunk2Size = hex_to_int(buffer_4bytes[3],buffer_4bytes[2],buffer_4bytes[1],buffer_4bytes[0],4);


	unsigned int NumSamples = (dword.subChunk2Size * 8) / (word.NumChannels * word.bitsPerSample) ;
	if( !header_checker(byte, word, dword, NumSamples) )
	{
		int start, end, size = 45, size_for_sec = (word.NumChannels * dword.sampleRate *word.bitsPerSample)/8 ,k, count = 0, total_sec = dword.subChunk2Size/dword.byteRate;
		if( sec[0] > sec[1] || sec[0] <= 0 || sec[1] > total_sec)
		{
			printf("\033[0;31m");
			printf("[-]");
			printf("\033[0m");
			printf(" Error ");
			printf("Please enter valid second's value.\n");
			return;
		}
		end = size_for_sec * sec[1] + 44;
		if( sec[0] == 1 )
			start = 45;
		else
			start = size_for_sec * sec[0] + 44;
		
		char **sound, **new_sound;
		

		sound = (char**)malloc(sizeof(char*) * NumSamples);
		new_sound = (char**)malloc(sizeof(char*) * NumSamples);
		for (i = 0; i < NumSamples; i++)
		{
			sound[i] = (char*)malloc(sizeof(char) * word.blockAlign);
			new_sound[i] = (char*)malloc(sizeof(char) * word.blockAlign);
		}
	
		for (i = 0; i < NumSamples; i++)
			k = fread(sound[i], sizeof(char), word.blockAlign, filei);
		for ( i = 0; i < NumSamples; i++)
		{
			if( size >= start && size <= end)
			{
				new_sound[count] = sound[i];
				count++;
			}
			size += word.blockAlign;
		}
		fclose(filei);
		
		fileo = fopen(file_output, "wb");
		dword.subChunk2Size = ( (sec[1] - sec[0] + 1 )*size_for_sec);
		dword.chunkSize = dword.subChunk2Size + 36;
		j = fwrite(byte.chunkID, sizeof(byte.chunkID), 1, fileo); //CHUNKID

		j = fwrite(&dword.chunkSize, sizeof(dword.chunkSize), 1, fileo);  //CHUNKSIZE

		j = fwrite(byte.format, sizeof(byte.format), 1, fileo);  //FORMAT

		j = fwrite(byte.subChunk1ID, sizeof(byte.subChunk1ID), 1, fileo); //SUBCHUNK1ID

		j = fwrite(&dword.subChunk1Size, sizeof(dword.subChunk1Size), 1, fileo);    //SUBCHUNK1SIZE

		j = fwrite(&word.audioFormat, sizeof(word.audioFormat), 1, fileo); //AUDIOFORMAT

		j = fwrite(&word.NumChannels, sizeof(word.NumChannels), 1, fileo); //numChannels

		j = fwrite(&dword.sampleRate, sizeof(dword.sampleRate), 1, fileo);  //samplerate

		j = fwrite(&dword.byteRate, sizeof(dword.byteRate), 1, fileo); //byterate

		j = fwrite(&word.blockAlign, sizeof(word.blockAlign), 1, fileo); //blockAlign

		j = fwrite(&word.bitsPerSample, sizeof(word.bitsPerSample), 1, fileo); //BitsPerSample

		j = fwrite(byte.subChunk2ID, sizeof(byte.subChunk2ID), 1, fileo); //SubChunk2ID

		j = fwrite(&dword.subChunk2Size, sizeof(dword.subChunk2Size), 1, fileo); //SubChunk2Size
		
		for (i = 0; i < count; i++)
			k = fwrite(new_sound[i], sizeof(char), word.blockAlign, fileo);
		printf("\033[0;32m");
		printf("[+]");
		printf("\033[0m");
		printf(" Creating the file %s\n", file_output);
		time_sleep(2.5);		
		fclose(fileo);
		for(i = 0; i < NumSamples; i++)
			free(sound[i]);
		free(sound);
	}
}
