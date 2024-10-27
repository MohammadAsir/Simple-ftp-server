/*
* client ftp
*
*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//for get implementation
//checkpoint
int send_FTP(int newPort, char serverIP[] ,char name[] ){
	char buf2[BUFSIZ];
	int data_sockfd;
	ssize_t len_data;
	struct sockaddr_in data_address;
	printf("here1\n");
	data_address.sin_family = AF_INET;
    	inet_pton(AF_INET, serverIP, &data_address.sin_addr);
    	data_address.sin_port = htons(newPort);

	if ((data_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("client: socket()");
		exit(1);
	}
	printf("here2\n");
	if (connect(data_sockfd, (struct sockaddr *)&data_address, 
   	    	sizeof(data_address)) < 0) {
		perror("client: connect()");
		exit(1);
	}
	printf("here is the filenamecleint side:%s", name);
	FILE* file = fopen(name, "rb");
    	if (file == NULL) {
        	perror("Failed to open file");
        	close(data_sockfd); // Ensure the socket is closed on file open error
        	exit(EXIT_FAILURE);
    	}
	size_t read_bytes;
	while ((read_bytes = fread(buf2, 1, BUFSIZ, file)) > 0) {
    		send(data_sockfd, buf2, read_bytes, 0);
	}

    printf("File has been sent successfully.\n");

    	// Cleanup
    fclose(file);
   	
	printf("here3\n");
	close(data_sockfd);
	return 0;

}
int connect_FTP(int newPort, char serverIP[] ,char name[] ){
    char buf2[BUFSIZ];
	int data_sockfd;
	ssize_t len_data;
	struct sockaddr_in data_address;

	data_address.sin_family = AF_INET;
    	inet_pton(AF_INET, serverIP, &data_address.sin_addr);
    	data_address.sin_port = htons(newPort);

	if ((data_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("client: socket()");
		exit(1);
	}
	
	if (connect(data_sockfd, (struct sockaddr *)&data_address, 
   	    	sizeof(data_address)) < 0) {
		perror("client: connect()");
		exit(1);
	}
	
	FILE* file = fopen(name, "wb");
    	if (file == NULL) {
        	perror("Failed to open file");
        	close(data_sockfd); // Ensure the socket is closed on file open error
        	exit(EXIT_FAILURE);
    	}

    	ssize_t bytesReceived;
   	while ((bytesReceived = read(data_sockfd, buf2, BUFSIZ)) > 0) {
        	fwrite(buf2, 1, bytesReceived, file);
    	}

    	if (bytesReceived < 0) {
        	perror("Read error");
    	} 
    	else {
        	printf("File has been received successfully.\n");
    	}


    	if (fclose(file) != 0) perror("fclose error");
    	if (close(data_sockfd) != 0) perror("close error");
    	
    	

    	return 0;
}

int main(int argc, char *argv[])
{
	char buf[BUFSIZ];
	int server_sockfd, newPort;
	ssize_t len;
	struct sockaddr_in server_address;
	struct hostent *host;		/* the host (server) */

	if (argc != 2) {
		fprintf(stderr, "usage: %s <server>\n", argv[0]);
		exit(1);
	}
	host = gethostbyname(argv[1]);
	if (host == (struct hostent *) NULL) {
		perror("client: gethostbyname()");
		exit(1);
	}

	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	memcpy(&server_address.sin_addr, host->h_addr, host->h_length);
	server_address.sin_port = htons(5237);

	if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("client: socket()");
		exit(1);
	}

	if (connect(server_sockfd, (struct sockaddr *) &server_address, 
   	    sizeof(server_address)) < 0) {
		perror("client: connect()");
		exit(1);
	}
	char serverIP[INET_ADDRSTRLEN]; 

	inet_ntop(AF_INET, &server_address.sin_addr, serverIP, INET_ADDRSTRLEN);
	//up to here just iniializing initianl connecion after indivisual conection for transfers
	
	do {
		write(STDOUT_FILENO, "ftp> ", 5);
		memset(buf, '\0', sizeof(buf));
		if ((len=read(STDIN_FILENO, buf, BUFSIZ)) > 0) {
			if(buf[0] == 'l' && buf[1] == 'c' && buf[2] == 'd'){
				char dirPath[BUFSIZ];
				strcpy(dirPath, buf + 4);
				int length = strlen(dirPath);
				dirPath[length-1] = '\0';
				if (chdir(dirPath) == 0) {
				    strcpy(buf, "Changed directory successfully\n");
				    write(STDOUT_FILENO, buf, strlen(buf));
				} 
				else {
				    perror("cd failed");
				    strcpy(buf, "Failed to change directory\n");
				    write(STDOUT_FILENO, buf, strlen(buf));
				}
				memset(buf, '\0', sizeof(buf));	
			}
			//move if sent move send passv and wait for port
			else if(buf[0] == 'g' && buf[1] == 'e' && buf[2] == 't'){
				char name[BUFSIZ];
				strcpy(name, buf+4);
				char pasvCmd[] = "PASV";
				strncat(pasvCmd, buf + 4, BUFSIZ - strlen(pasvCmd) - 2);
			    send(server_sockfd, pasvCmd, strlen(pasvCmd), 0);
				//PASV with file name has been sent, after receivng the port number will connect
				memset(buf,'\0', sizeof(buf));
				read(server_sockfd, buf, BUFSIZ);
				newPort = atoi(buf);
			    name[strlen(name)-1] = '\0';

			    connect_FTP(newPort, serverIP , name);
				memset(buf, '\0', sizeof(buf));
				continue;
			    	
			}
			else if(buf[0] == 'p' && buf[1] == 'u' && buf[2] == 't'){
				char name2[BUFSIZ];
				strcpy(name2, buf+4);
				char pasvCmd[BUFSIZ] = "PASVg";
				strncat(pasvCmd, name2, BUFSIZ - strlen(pasvCmd) - 1);
			    send(server_sockfd, pasvCmd, strlen(pasvCmd), 0);
				//PASV with file name has been sent, after receivng the port number will connect
				memset(buf,'\0', sizeof(buf));
				read(server_sockfd, buf, BUFSIZ);
				newPort = atoi(buf);
			    name2[strlen(name2)-1] = '\0';
				printf("name2:%s", name2);
			    send_FTP(newPort, serverIP , name2);
				memset(buf, '\0', sizeof(buf));
				continue;
			    	
			}
			
			
			else{
				write(server_sockfd, buf, len);
				if ((len=read(server_sockfd, buf, BUFSIZ)) > 0) 
					write(STDOUT_FILENO, buf, len);
			}
		}
		
	} while (buf[0] != 'b' && buf[1] != 'y' && buf[0] != 'e');
	close(server_sockfd);
	exit(0);
}

