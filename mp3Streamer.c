#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

long getFileLen(FILE *file);
int unpackSizeTag(char *buffer);
long extractMp3(FILE *formatFile, FILE *file);
void streamMedia(FILE *formatFile, long fileLen, int snew, char *reply);
void parseSchedule(char *str[4]);

/*
	program to stream mp3 audio, tcp server not implemented yet
*/

int main(int argc, char* argv[]){

	FILE *file;
	FILE *formatFile;
	long fileLen;
	char *str[4];
	char buffer[400];
	int sock, snew;

	struct sockaddr_in server, client;

	char *reply = 
		"HTTP/1.1 200 OK\n"
                "Date: Wed, 17 April 2018 12:27:04 GMT\n"
                "Server: Apache/2.2.3\n"
                "Last-Modified: Wed, 18 Jun 2003 16:05:58 GMT\n"
                "ETag: \"56d-9989200-1132c580\"\n"
                "Content-Type: audio/mpeg\n"
               // "Accept-Ranges: bytes\n"
                "Connection: keep-alive\n"
		"\n";
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock < 0){
                perror("Server: cannot open master socket");
                exit(1);
        }

	server.sin_family = AF_INET;
        server.sin_addr.s_addr = inet_addr("0.0.0.0");
        server.sin_port = htons(2224);

        if (bind(sock, (struct sockaddr*) &server, sizeof(server))) {
                perror ("Server: cannot bind master socket");
                exit (1);
        }

        listen(sock, 5);

        int clientLength = sizeof(client);

        snew = accept(sock, (struct sockaddr*) &client, &clientLength);
        if (snew < 0) {
                perror ("Server: accept failed");
                exit (1);
        }
        printf("Client connected!\n");
        send(snew, reply, strlen(reply), 0);
	//read(snew, buffer, 400);
	//printf("%s\n", buffer);
	


	// allocate room for 4 100 byte strings for file titles
	for(int i = 0; i < 4; i++){
        	str[i] = malloc(sizeof(char) * 100);
        }

	// create a string of file names to be opened throughout program
	parseSchedule(str);

	for(int i = 0; i < 4; i++){
		file = fopen(str[i], "rb");
		formatFile = fopen("formattedMp3.mp3", "wb");

		fileLen = extractMp3(formatFile, file);

		formatFile = fopen("formattedMp3.mp3", "rb");
		sleep(5);
		streamMedia(formatFile, fileLen, snew, reply);
	}

	return 0;
}

// get the length in bytes of the mp3 file
long getFileLen(FILE *file){
	fseek(file, 0, SEEK_END);
	long fileLen = ftell(file);
	rewind(file);

	return fileLen;			
}

// unpack the size of the id tag for extracting the mp3 data
int unpackSizeTag(char *buffer){
	int a, b, c, d;	

	a = buffer[6] & 0x0000007f;
	b = buffer[7] & 0x0000007f;
	c = buffer[8] & 0x0000007f;
	d = buffer[9] & 0x0000007f;

	return (a << 24) | (b << 14 ) | (c << 7) | (d);
}

// extract the mp3 data from the tag, return the new size in bytes of the file
long extractMp3(FILE *formatFile, FILE *file){
	char *buffer;
	int fileLen, tagSize;	

	fileLen = getFileLen(file);

        buffer = (char *)malloc(fileLen * sizeof(char));
        fread(buffer, fileLen, 1, file);
        fclose(file);

        if(buffer[0] == 'I' && buffer[1] == 'D'){
                tagSize = unpackSizeTag(buffer);
                memmove(buffer, buffer + tagSize, fileLen - tagSize);
                fwrite(buffer, 1, fileLen - tagSize, formatFile);
                fclose(formatFile);
   	}

	return (fileLen - tagSize);
}

// stream the file in packets of 1400 bytes and output to stdout
void streamMedia(FILE *formatFile, long fileLen, int snew, char *reply){
	int i;
	char c;
	char *streamBuffer;
	FILE *stream;
	
	streamBuffer = (char *)malloc(1400 * sizeof(char));

	//stream = popen("mpg123 -", "w");	

	while((ftell(formatFile) + 1) < fileLen){
                i = 0;
                c = fgetc(formatFile);
                while (i < 1400){
                        streamBuffer[i] = c;
                        c = fgetc(formatFile);
                        i++;
                }
                fseek(formatFile, -1, SEEK_CUR);
                send(snew, streamBuffer, 1400, 0);
                usleep(150000);
        }
	printf("done");
}

// parse schedule.txt to get todays n show playlist
void parseSchedule(char *str[4]){
	 FILE * schedule, *show;

	schedule = fopen("schedule.txt", "r");

        for(int i = 0; i < 4; i++){
                fgets(str[i], 100, schedule);

                char* nl = strchr(str[i], '\n');
                if (nl != NULL){
                        *nl = '\0';
                }

                printf("Show is: %s\n", str[i]);
        }
        fclose(schedule);
}
