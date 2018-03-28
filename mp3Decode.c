#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

long getFileLen(FILE *file);
int unpackSizeTag(char *buffer);

int main(){

	FILE *file;
	FILE *formatFile;
	char *buffer;
	char *sentBuffer;
	char c;
	long fileLen;
	int tagSize, i;

	file = fopen("Suspense_420819_010_The_Cave_of_Ali_Baba_-128-44-_27789_29m33s.mp3", "rb");
	formatFile = fopen("formattedMp3.mp3", "wb");
	fileLen = getFileLen(file);

	buffer = (char *)malloc(fileLen * sizeof(char));
	fread(buffer, fileLen, 1, file);
	fclose(file);

	if(buffer[0] == 'I' && buffer[1] == 'D'){
		tagSize = unpackSizeTag(buffer);
		printf("tag size: %d\n", tagSize); 
		memmove(buffer, buffer + tagSize, fileLen - tagSize);
		for(int i = 0; i < 40; i++){
			printf("%c", buffer[i]);
		}	
		fwrite(buffer, 1, fileLen - tagSize, formatFile);
	}

	sentBuffer = (char *)malloc(1400 * sizeof(char));
	 while(1){
                i = 0;
                c = fgetc(formatFile);
                while (i < 1400){
                        sentBuffer[i] = c;
                        c = fgetc(formatFile);
                        i++;
                }
		printf("%.*s", 1400, sentBuffer);
                fseek(formatFile, -1, SEEK_CUR);
		sleep(2);
	}

	

	

}

long getFileLen(FILE *file){
	fseek(file, 0, SEEK_END);
	long fileLen = ftell(file);
	rewind(file);

	return fileLen;			
}

int unpackSizeTag(char *buffer){
	int a, b, c, d;	

	a = buffer[6] & 0x0000007f;
	printf("a: %d\n", a);
	b = buffer[7] & 0x0000007f;
	printf("b: %d\n", b);
	c = buffer[8] & 0x0000007f;
	printf("c: %d\n", c);
	d = buffer[9] & 0x0000007f;

	return (a << 24) | (b << 14 ) | (c << 7) | (d);
	

}
