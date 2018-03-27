#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long getFileLen(FILE *file);
int unpackSizeTag(char *buffer);

int main(){

	FILE *file;
	char *buffer;
	long fileLen;
	int tagSize;

	file = fopen("Suspense_420819_010_The_Cave_of_Ali_Baba_-128-44-_27789_29m33s.mp3", "rb");
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
