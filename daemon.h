#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/wait.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


#define PORT 23001
#define SERVER_ADDR "127.0.0.1"
void* handel_client(void* arg);

#define NUM_CONNECTION 5


//function prototype
void daemon_kill(int sig_number);
void daemon_start(void);

