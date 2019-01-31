#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char *buffer;
    int segSize;
    int curPointer;
    int maxPointer;
}bufferC_t;

void initBuffer(bufferC_t *buffer);
void addToBuffer(bufferC_t *buffer, char buff[2]);

int main(){
	// init buffer
	bufferC_t joinBuffer;
    initBuffer(&joinBuffer);
    addToBuffer(&joinBuffer, "aa");
    addToBuffer(&joinBuffer, "bb");
    addToBuffer(&joinBuffer, "cc");
    addToBuffer(&joinBuffer, "dd");
    addToBuffer(&joinBuffer, "ee");
    for(int i = 0; i < 6; i++){
    	printf("%c", joinBuffer.buffer[i]);
    }





}

void initBuffer(bufferC_t *buffer){
	buffer->buffer = malloc(6 * sizeof(char));
	buffer->segSize = 2;
    buffer->curPointer = 0;
    buffer->maxPointer = 3;
}

void addToBuffer(bufferC_t *buffer, char buff[2]){
	if(buffer->curPointer < buffer->maxPointer){
		 memmove(buffer->buffer + (buffer->curPointer * buffer->segSize), buff, buffer->segSize);
		 buffer->curPointer++;
	}else{
		memmove(buffer->buffer,  buffer->buffer + buffer->segSize, (buffer->curPointer - 1) * buffer->segSize);
		memmove(buffer->buffer + (buffer->curPointer - 1) * buffer->segSize, buff, buffer->segSize);
	}
}
