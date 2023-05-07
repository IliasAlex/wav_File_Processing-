/**********************
**Rantses Vinse
**AM:2022201900196
**dit19196@uop.gr
**
**Alexandropoulos Ilias
**AM:2022201900007
**dit19007@uop.gr
***********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "music.h"
#include "header.h"



int main(int arg_size, char *arg_str[])
{	
	arg_size--;
	char *str[arg_size], *file_input, *file_output;
	int i, sec[2];
	for(i = 0; i < arg_size; i++)
	{
		str[i] = (char *)malloc(sizeof(char) * strlen(arg_str[i+1]) );
		strncpy(str[i], arg_str[i+1], strlen(arg_str[i+1]));
		
	}
	if( !strcmp(str[0], "-list") )
	{
		file_input = (char *)malloc(sizeof(char) * strlen(str[1]) );
		strcpy(file_input, str[1]);
		list(file_input);
	}
	else if( !strcmp(str[0], "-reverse") )
	{
		file_input = (char *)malloc(sizeof(char) * strlen(str[1]) );
		file_output = (char *)malloc(sizeof(char) * strlen(str[2]) );
		strcpy(file_input, str[1]);	
		strcpy(file_output, str[2]);
		reverse(file_input, file_output);
	}
	else if( !strcmp(str[0], "-mono") )
	{
		file_input = (char *)malloc(sizeof(char) * strlen(str[1]) );
		file_output = (char *)malloc(sizeof(char) * strlen(str[2]) );
		strcpy(file_input, str[1]);	
		strcpy(file_output, str[2]);
		mono(file_input, file_output);
	}
	else if( !strcmp(str[0], "-crop") )
	{
		file_input = (char *)malloc(sizeof(char) * strlen(str[1]) );
		file_output = (char *)malloc(sizeof(char) * strlen(str[4]) );
		strcpy(file_input, str[1]);
		strcpy(file_output, str[4]);
		sec[0] = string_to_int(str[2], strlen(str[2]) );
		sec[1] = string_to_int(str[3], strlen(str[3]) );
		crop(file_input, file_output, sec);
	}
	else
	{
		time_sleep(1.5);
		printf("\033[0;31m");
		printf("[-]");
		printf("\033[0m");
		printf(" Unknown command\n");
	}
	for(i = 0; i < arg_size; i++)
		free(str[i]);
	return 0;
}

int string_to_int( char *Str, int max)
{
	int i, div = 1, x = 0;
	for(i = 0; i < max-1; i++)
		div *= 10;
	for( i = 0; i <= max-1; i++)
	{
		x += (Str[i] - '0')*div;
		div /= 10;
	}
	return x;
}

void time_sleep (double secs) 
{
	int kathisterish;	
	kathisterish = time(NULL) + secs;  
	while( time(NULL) < kathisterish );
}

unsigned int hex_to_int(unsigned char chunk1, unsigned char chunk2, unsigned char chunk3, unsigned char chunk4, int type)
{
	if(type == 4)
	{
		unsigned int chunk;
		chunk = chunk4 | (chunk3<<8) | (chunk2<<16) | (chunk1<<24);
		return chunk;
	}
	else if( type == 2)
	{
		unsigned short int chunk;
		chunk = (chunk1<<8) | chunk2;
		return chunk;
	}
}
