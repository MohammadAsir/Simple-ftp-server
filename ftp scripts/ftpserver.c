/*
* server ftp
*
*/
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
void recv_ser(char name[BUFSIZ]){
	printf("here is the file name server side recv:%s", name);
	char buf2[BUFSIZ];
	int server2_sockfd, data_sockfd, data_len;
	struct sockaddr_in data_address, server2_address;
	ssize_t len2;
	int option =1;

	if ((server2_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("server: socket()");
		exit(1);
	}

   	if (setsockopt(server2_sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
       	&option, sizeof(option)) < 0) {
      		perror("server: setsockopt()");
      		exit(1);
   	}
		   	
	memset(&server2_address, 0, sizeof(server2_address));
	server2_address.sin_family = AF_INET;
	server2_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server2_address.sin_port = htons(5122);

	if (bind(server2_sockfd, (struct sockaddr *) &server2_address, 
	    	sizeof(server2_address)) < 0) {
		perror("server: bind()");
		close(server2_sockfd);
		exit(1);
	}
	printf("data server waiting\n");
	if (listen(server2_sockfd, 5) < 0) {
		perror("server: listen()");
		close(server2_sockfd);
		exit(1);
	}
	printf("here receive 1\n");
	
	data_len = sizeof(data_address);
	printf("here 1.5\n");
	if ((data_sockfd = accept(server2_sockfd, (struct sockaddr *) &data_address, &data_len)) < 0) {
		printf("data connection1 refused");
		perror("server: accept()");
		close(server2_sockfd);
		exit(1);
	}
	name[strlen(name)-1] = '\0';
	printf("here receive 2\n");
	
	FILE* file = fopen(name, "wb");
    if (file == NULL) {
    	perror("Failed to open file");
    	close(server2_sockfd); // Ensure the socket is closed on file open error
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
	printf("here receive 3\n");
	close(server2_sockfd);
	close(data_sockfd);
}
void connect_ser(char name[BUFSIZ]){
	char buf2[BUFSIZ];
	int server2_sockfd, data_sockfd, data_len;
	struct sockaddr_in data_address, server2_address;
	ssize_t len2;
	int option =1;
	printf("here is the file name server get side:%s", name);
	if ((server2_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("server: socket()");
		exit(1);
	}

   	if (setsockopt(server2_sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
       	&option, sizeof(option)) < 0) {
      		perror("server: setsockopt()");
      		exit(1);
   	}
		   	
	memset(&server2_address, 0, sizeof(server2_address));
	server2_address.sin_family = AF_INET;
	server2_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server2_address.sin_port = htons(5121);

	if (bind(server2_sockfd, (struct sockaddr *) &server2_address, 
	    	sizeof(server2_address)) < 0) {
		perror("server: bind()");
		close(server2_sockfd);
		exit(1);
	}
	printf("data server waiting\n");
	if (listen(server2_sockfd, 5) < 0) {
		perror("server: listen()");
		close(server2_sockfd);
		exit(1);
	}
	printf("here 1\n");
	
	data_len = sizeof(data_address);
	printf("here 1.5\n");
	if ((data_sockfd = accept(server2_sockfd, (struct sockaddr *) &data_address, &data_len)) < 0) {
		printf("data connection1 refused");
		perror("server: accept()");
		close(server2_sockfd);
		exit(1);
	}

	printf("here 2\n");

	name[strlen(name)-1] = '\0';

	FILE* file = fopen(name, "rb");
	if (file == NULL) {
    		perror("Failed to open file");
    		exit(EXIT_FAILURE);
	}

	size_t read_bytes;
	while ((read_bytes = fread(buf2, 1, BUFSIZ, file)) > 0) {
    		send(data_sockfd, buf2, read_bytes, 0);
	}

    printf("File has been received successfully.\n");

    	// Cleanup
    fclose(file);
   	
	printf("here3\n");
	close(server2_sockfd);
	close(data_sockfd);	 
}
int main(void)
{
	char buf[BUFSIZ];
	int server_sockfd, client_sockfd, client_len;
	struct sockaddr_in client_address, server_address;
	ssize_t len;
	int option = 1;
	FILE *fp;
	
	if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("server: socket()");
		exit(1);
	}

   	if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
       	&option, sizeof(option)) < 0) {
      		perror("server: setsockopt()");
      		exit(1);
   	}
   	
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(5237);

	if (bind(server_sockfd, (struct sockaddr *) &server_address, 
	    sizeof(server_address)) < 0) {
		perror("server: bind()");
		close(server_sockfd);
		exit(1);
	}
	printf("server waiting\n");
	if (listen(server_sockfd, 5) < 0) {
		perror("server: listen()");
		close(server_sockfd);
		exit(1);
	}
	client_len = sizeof(client_address);
	if ((client_sockfd = accept(server_sockfd, 
	    (struct sockaddr *) &client_address, &client_len)) < 0) {
		perror("server: accept()");
		close(server_sockfd);
		exit(1);
	}
	
	while ((len=read(client_sockfd, buf, BUFSIZ)) > 0) {
		 printf("Received command: %s\n", buf);
		//implementing ls
		if(buf[0] == 'l' && buf[1] == 's'){
			fp = popen("ls", "r");
			while ((len= fread(buf, 1, BUFSIZ, fp)) > 0) {
				write(client_sockfd, buf, len);
			}
		    	pclose(fp);
		    	memset(buf, '\0', sizeof(buf));
		}
		else if(buf[0] == 'c' && buf[1] == 'd'){
			char dirPath_server[BUFSIZ];
			strcpy(dirPath_server, buf + 3);
			int length = strlen(dirPath_server);
			dirPath_server[length-1] = '\0';
			if (chdir(dirPath_server) == 0) {
			    strcpy(buf, "Changed directory successfully\n");
			    write(client_sockfd, buf, strlen(buf));
				memset(buf, '\0', sizeof(buf));			
			} 
			else {
			    perror("cd failed");
			    strcpy(buf, "Failed to change directory\n");
			    write(client_sockfd, buf, strlen(buf));
			    memset(buf, '\0', sizeof(buf));
			}
			memset(buf, '\0', sizeof(buf));


		}
		else if(buf[0] == 'P' && buf[1] == 'A' && buf[2] == 'S' && buf[3] == 'V' && buf[4] == 'g'){
			char name[BUFSIZ];
			strcpy(name, buf+5);
			strcpy(buf, "5122");
			write(client_sockfd, buf, strlen(buf));
			//connecting
			recv_ser(name);
			//sending it to client so it can connect
			memset(buf, '\0', sizeof(buf));

			
			
		}
		else if(buf[0] == 'P' && buf[1] == 'A' && buf[2] == 'S' && buf[3] == 'V' ){
			char name[BUFSIZ];
			strcpy(name, buf+4);
			strcpy(buf, "5121");
			write(client_sockfd, buf, strlen(buf));
			//connecting
			connect_ser(name);
			//sending it to client so it can connect
			memset(buf, '\0', sizeof(buf));
			
		}
		
		
		
		if (buf[0] == 'b' && buf[1] == 'y' && buf[2] == 'e' ){
			break;
		}	
	}
	close(server_sockfd);
	close(client_sockfd);
}
