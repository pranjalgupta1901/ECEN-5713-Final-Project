#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <asm/types.h>
#include <errno.h>
#include <netdb.h>
#include <syslog.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include "../lcd_display/lcd_display.h"

#define FILE_SIZE 1024
char file_array[FILE_SIZE];
int socket_fd, client_fd;
void mysig(int signo)
{
	if (signo == SIGINT || signo == SIGTERM)
	{
		close(socket_fd);
		close(client_fd);

		closelog();
		exit(EXIT_SUCCESS);
	}
}
void setup_signal()
{

	struct sigaction sa;

	sa.sa_handler = &mysig;
	sigemptyset(&(sa.sa_mask));
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		close(socket_fd);
		close(client_fd);
		syslog(LOG_PERROR, "Unable to initialise SIGINT the signals with error code %d\n", errno);
		exit(-1);
	}

	if (sigaction(SIGTERM, &sa, NULL) == -1)
	{
		close(socket_fd);
		close(client_fd);
		syslog(LOG_PERROR, "Unable to initialise SIGTERM the signals with error code %d\n", errno);
		exit(-1);
	}
}

int main(int argc, char *argv[])
{
//struct sockaddr_in server_addr;
	struct addrinfo hints, *result;
	int errorcode;
	const char *port = "9000";
	char ip_addr[INET6_ADDRSTRLEN];
//const char *server_ip = "10.0.0.221";
	openlog("aesdsocket", LOG_CONS | LOG_PID, LOG_USER);
	memset(&hints, 0, sizeof(hints));

	setup_signal();

	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	errorcode = getaddrinfo(NULL, port, &hints, &result);
	if (errorcode != 0)
	{
		syslog(LOG_PERROR, "Error in gettng address suing getaddrinfo with error code %d\n", errno);
		closelog();
		exit(-1);
	}

	socket_fd = socket(result->ai_family, result->ai_socktype, 0);
	if (socket_fd == -1)
	{
		syslog(LOG_PERROR, "socket creation failed with error code %d\n", errno);
		closelog();
		exit(-1);
	}
	syslog(LOG_DEBUG, "Socket Created with Socket Id %d\n", socket_fd);

	int var_setsockopt = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &var_setsockopt,
				   sizeof(int)) == -1)
	{
		syslog(LOG_PERROR, "Socket resuse address failed with error code %d\n", errno);
		closelog();
		exit(-1);
	}

	errorcode = bind(socket_fd, result->ai_addr, result->ai_addrlen); // check what should be the sockaddr for it?
	if (errorcode == -1)
	{
		close(socket_fd);
		syslog(LOG_PERROR, "Socket bind failed with error code %d and closing server socket \n", errno);
		closelog();
		exit(-1);
	}

	syslog(LOG_DEBUG, "Bind Successful\n");
	freeaddrinfo(result);

	errorcode = listen(socket_fd, 10);
	if (errorcode == -1)
	{
		close(socket_fd);
		syslog(LOG_PERROR, "Listen failed with error code %d and closing server socket \n", errno);
		closelog();
		exit(-1);
	}
	
	int count  = 0;

	while (1)
	{

	struct sockaddr_in client_addr;
	socklen_t client_addr_size = sizeof(client_addr);
	
	client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_addr_size);
	if (client_fd == -1)
	{
		close(socket_fd);
		syslog(LOG_PERROR, "connection failed with error code %d and closing server socket \n", errno);
		closelog();
	}
	
	if(count == 0){
	struct sockaddr_in *ip_addr_ptr = (struct sockaddr_in *)&client_addr;

	inet_ntop(AF_INET, &ip_addr_ptr->sin_addr, ip_addr, INET_ADDRSTRLEN);

	syslog(LOG_DEBUG, "Accepted connection from %s\n", ip_addr);
	printf("Accepted connection from %s\n", ip_addr);
	}
	count++;



		syslog(LOG_DEBUG, "Recieve Started\n");

		int bytes_rec;
		bool rec_complete = false;
		char *ptr = NULL;
		while (rec_complete == false){
		
			memset(file_array, 0, 1024);
			bytes_rec = recv(client_fd, file_array, FILE_SIZE, 0);
			printf("%d\n",bytes_rec);
			if (bytes_rec < 0)
			{
				syslog(LOG_DEBUG, "Closed connection from %s\n", ip_addr);
				close(client_fd);
				syslog(LOG_PERROR, "recieve unsuccessful with error code %d\n", errno);
				closelog();
			}
			else
			{
				ptr = memchr(file_array, '$', bytes_rec);
				if (ptr != NULL)
				{	
					printf("%s\n", file_array);
					rec_complete = true;
					break;
					//lcd_clear(); 
					
	        			//lcd_move_and_write(0,1, file_array);
					//break;
				}
			}
			
		
		}

/*
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = PF_INET;
    server_addr.sin_addr.s_addr = ip_addr;
    server_addr.sin_port = htons(port);   
	//close(socket_fd);
	errorcode = connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (errorcode == -1)
	{
		close(socket_fd);
		syslog(LOG_PERROR, "Connect failed with error code %d and closing client socket \n", errno);
		printf("error while connecting\n");
		closelog();
		exit(-1);
	}
	printf("connection accepted\n");

*/

		int bytes_send = bytes_rec;
		int sent_actual;
		
		sent_actual = send(client_fd, file_array, bytes_send, 0);
		if (sent_actual != bytes_send)
		{
			close(client_fd);
			
			printf("error found\n");
			perror("error in sending data to socket");
		}
		
		printf(" send actual : %d\n", sent_actual);
		close(client_fd);
		rec_complete = false;
	}
				
	syslog(LOG_DEBUG, "Process Completed\n");
	return 0;
}


