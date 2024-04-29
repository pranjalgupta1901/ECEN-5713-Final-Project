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
#include "../gpio/gpio.h"



#define BUTTON_PAUSE "5"
#define BUTTON_PLAY "6"
#define BUTTON_PLAY_NEXT "13"
#define BUTTON_PLAY_PREVIOUS "14"



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


void pause_event(void){

		int sent_actual;
		
		sent_actual = send(client_fd, "1", 1, 0);
		if (sent_actual != 1)
		{
			close(client_fd);
			
			printf("error found\n");
			perror("error in sending data to socket");
		}
		
		printf(" send actual : %d\n", sent_actual);	
		
}


void play_event(void){

		int sent_actual;
		
		sent_actual = send(client_fd, "2", 1, 0);
		if (sent_actual != 1)
		{
			close(client_fd);
			
			printf("error found\n");
			perror("error in sending data to socket");
		}
		
		printf(" send actual : %d\n", sent_actual);	
		
}

void play_previous_event(void){

		int sent_actual;
		
		sent_actual = send(client_fd, "4", 1, 0);
		if (sent_actual != 1)
		{
			close(client_fd);
			
			printf("error found\n");
			perror("error in sending data to socket");
		}
		
		printf(" send actual : %d\n", sent_actual);	
		
}


void play_next_event(void){

		int sent_actual;
		
		sent_actual = send(client_fd, "3", 1, 0);
		if (sent_actual != 1)
		{
			close(client_fd);
			
			printf("error found\n");
			perror("error in sending data to socket");
		}
		
		printf(" send actual : %d\n", sent_actual);	
		
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
	struct addrinfo hints, *result;
	int errorcode;
	char *token;
	const char *port = "9000";
	char ip_addr[INET6_ADDRSTRLEN];
	openlog("aesdsocket", LOG_CONS | LOG_PID, LOG_USER);
	memset(&hints, 0, sizeof(hints));

	setup_signal();
	lcd_init();
	
	
	export_gpio(BUTTON_PAUSE);
	export_gpio(BUTTON_PLAY);
	export_gpio(BUTTON_PLAY_NEXT);
	export_gpio(BUTTON_PLAY_PREVIOUS);
	
	gpio_set_direction(BUTTON_PAUSE, DIRECTION_IN);
	gpio_set_direction(BUTTON_PLAY, DIRECTION_IN);
	gpio_set_direction(BUTTON_PLAY_NEXT, DIRECTION_IN);
	gpio_set_direction(BUTTON_PLAY_NEXT, DIRECTION_IN);
	
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
					
				}
			}
			
		
		}

	lcd_clear();
	token = strtok(file_array, "#");
	int row = 0;
	lcd_move_and_write(row++,0, token);
	token = strtok(NULL, "#");
	lcd_move_and_write(row++,0, token);
	token = strtok(NULL, "#");
	lcd_move_and_write(row++,0, token);
	token = strtok(NULL, "#");
	
	
	if(read_gpio_state(BUTTON_PAUSE) == 1)
		pause_event();
	else if(read_gpio_state(BUTTON_PLAY) == 1)
		play_event();
	else if(read_gpio_state(BUTTON_PLAY_NEXT) == 1)
		play_next_event();
	else if(read_gpio_state(BUTTON_PLAY_PREVIOUS) == 1)
		play_previous_event();
		
		
	close(client_fd);
	rec_complete = false;
	
	}	
	
				
	syslog(LOG_DEBUG, "Process Completed\n");
	return 0;
}




