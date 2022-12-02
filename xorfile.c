/*
 *  xorfile - xor's two input file to one output file
 *  Copyright (C) 2022 Resilience Theatre
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * This work is based on gstreamer example applications and references
 * indicated in README.
 * 
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "log.h"
#define BLOCKSIZE	1024

long int get_file_size (char *filename) {
	struct stat st;
	long int size=0;
	stat(filename, &st);
	size = st.st_size;
	return size;
}

int main(int argc, char *argv[])
{
	int c=0;	
	int log_level=LOG_INFO;
	char *inputfile_1=NULL;
	char *inputfile_2=NULL;
	char *outputfilename=NULL;
	long int filesize_1, filesize_2;
	long int fileindex=0;
	char *keybuffer;
	char *databuffer;
	char *outputbuffer;
	long int stepvalue=1000000;
	
	FILE *keyfile;
	FILE *datafile;
	FILE *outputfile;
	
	while ((c = getopt (argc, argv, "dhi:j:o:")) != -1)
	switch (c)
	{
		case 'd':
			log_level=LOG_DEBUG;
			break;
		case 'i':
			inputfile_1 = optarg;
			break;
		case 'j':
			inputfile_2 = optarg;
			break;
		case 'o':
			outputfilename = optarg;
			break;
		case 'h':
			log_info("[%d] xorfile",getpid());
			log_info("[%d] xor",getpid());
			log_info("[%d] -d enable debug logging",getpid());
			log_info("[%d] -i [input file 1] ",getpid());
			log_info("[%d] -j [input file 2] ",getpid());
			log_info("[%d] -o [output file] ",getpid());
			return 1;
		break;
			default:
			break;
	}
		
	log_set_level(log_level);
	
	filesize_1=get_file_size(inputfile_1);
	filesize_2=get_file_size(inputfile_2);
	
	if ( filesize_1 == filesize_2 ) 
	{
		log_info("[%d] Filesize 1: %ld ",getpid(),filesize_1);
		log_info("[%d] Filesize 2: %ld ",getpid(),filesize_2);
		
		keyfile = fopen(inputfile_1, "rb");
		datafile = fopen(inputfile_2, "rb");
		outputfile = fopen(outputfilename, "wb");
			
			fileindex=0;
			while (fileindex < filesize_1 - BLOCKSIZE)
			{
				if ( fileindex > stepvalue) {
					printf("File index: %ld \n",fileindex);
					stepvalue = stepvalue + 1000000;
				}
				
				keybuffer=malloc(BLOCKSIZE);
				databuffer=malloc(BLOCKSIZE);
				outputbuffer=malloc(BLOCKSIZE);
				fseek(keyfile, fileindex, SEEK_SET);
				fread(keybuffer, BLOCKSIZE,1,keyfile); 	
				fseek(datafile, fileindex, SEEK_SET);
				fread(databuffer, BLOCKSIZE,1,datafile);
			
				int blockloop=0;
				for (blockloop=0; blockloop < BLOCKSIZE; blockloop++) {
					outputbuffer[blockloop] = databuffer[blockloop] ^ keybuffer[blockloop];
					if ( log_level == LOG_DEBUG ) {				
						printf("[%d] %x ^",blockloop,databuffer[blockloop] & 0xff );
						printf(" %x =",keybuffer[blockloop] & 0xff );
						printf(" %x \n",outputbuffer[blockloop] & 0xff);
					}
					
				}
				 if ( log_level == LOG_DEBUG )
				 		printf("\n");
				
				fwrite(outputbuffer,BLOCKSIZE,1,outputfile);		
				free(keybuffer);
				free(databuffer);
				free(outputbuffer);	
				fileindex=fileindex+BLOCKSIZE;
			 }
	 
		fclose(datafile);
		fclose(outputfile);
		fclose(keyfile);
	} else {
		log_info("[%d] Files not equal lenght: ( %ld != %ld ) ",getpid(),filesize_1,filesize_2);
	}
}


