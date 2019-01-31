#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main(){
        int sock, snew;

	char *reply = 
		"HTTP/1.0 200 OK\n"
		"Date: Wed, 17 April 2018 12:27:04 GMT\n"
		"Server: Apache/2.2.3\n"
		"Last-Modified: Wed, 18 Jun 2003 16:05:58 GMT\n"
		"ETag: \"56d-9989200-1132c580\"\n"
		"Content-Type: text/html\n"
		"Accept-Ranges: bytes\n"
		"Connection: keep-alive\n"
		"\n";

	char *data = "a";

        struct sockaddr_in server, client;

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

        for(int i = 0; i < 10; i++){
		send(snew, data, 1, 0);
		sleep(1);
	}

	close(snew);


}
