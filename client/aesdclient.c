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

int socket_fd, client_fd;
void daemonize(void);
void mysig(int signo)
{
	if (signo == SIGINT || signo == SIGTERM)
	{
		close(socket_fd);
		close(client_fd);

		int ret = remove(filename);

		if (ret == 0)
		{
			syslog(LOG_DEBUG, "Deleted file %s\n", filename);
		}
		else
		{
			syslog(LOG_PERROR, "Unable to Delete file %s with error code %d  \n", filename, errno);
		}
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

	struct sockaddr_in server_addr;

	int errorcode;
	int var = 0;
	const char *port = "9000";
    const char *server_ip = "x";
	char ip_addr[INET6_ADDRSTRLEN];

	openlog("aesdsocket", LOG_CONS | LOG_PID, LOG_USER);
	

	setup_signal();

	socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
	{
		syslog(LOG_PERROR, "socket creation failed with error code %d\n", errno);
		closelog();
		exit(-1);
	}
	syslog(LOG_DEBUG, "Socket Created with Socket Id %d\n", socket_fd);


	if (argc > 1 && strcmp(argv[1], "-d") == 0)
	{
		syslog(LOG_USER, "Daemonizing process\n");
		daemonize();
	}

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = PF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(port);   


	errorcode = connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (errorcode == -1)
	{
		close(socket_fd);
		syslog(LOG_PERROR, "Connect failed with error code %d and closing client socket \n", errno);
		closelog();
		exit(-1);
	}

    syslog(LOG_DEBUG, "Connection established to server %s and port %s", server_ip, port);


	while (1)
	{
        //send and recieving data
	}
	syslog(LOG_DEBUG, "Process Completed\n");
	return 0;
}

void daemonize(void)
{

	int pid = fork();
	if (pid < 0)
	{
		perror("Fork Failed\n");
		exit(-1);
	}
	if (pid > 0)
	{
		exit(EXIT_SUCCESS);
	}
	printf("Child process is created\n");

	int sid = setsid();
	if (sid < 0)
	{
		perror("setsid failed");
	}

	if (chdir("/") < 0)
	{

		perror("chdir failed\n");
		exit(-1);
	}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	int var_daemon = open("/dev/null", O_RDWR);
	if (var_daemon < 0)
	{
		perror("file redirection failed");
		exit(-1);
	}
	dup2(var_daemon, STDIN_FILENO);
	dup2(var_daemon, STDOUT_FILENO);
	dup2(var_daemon, STDERR_FILENO);
	close(var_daemon);
}