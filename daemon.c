#include "daemon.h"
void daemon_start(void)
{
    
    pid_t pid;
    int fd;
    //launching process
    if((pid=fork())== -1)
    {
        //something went wrong
        perror("error forking");
        exit(EXIT_FAILURE);
        
    }
    
    else if(pid>0)
    {
        //exit if its parent
        exit(EXIT_FAILURE);
        
    }
    //set child group leader
    if (setpgrp() == -1)
    {
        perror("error setpgrp");
        exit(EXIT_FAILURE);
    }
    //close them before second fork to make sure we do not inherent file descriptions
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    //open syslog
    setlogmask(LOG_MASK(LOG_NOTICE));
    openlog("Daemon example",LOG_CONS | LOG_PID | LOG_NDELAY, LOG_DAEMON);
    //fork again to get the child
    if((pid=fork())== -1){
        syslog(LOG_NOTICE, "ERROR FORK");
        exit(EXIT_FAILURE);
        
    }
    else if (pid>0){
        //exit if its parent
        exit(EXIT_SUCCESS);
        
    }
    
    //change diroctory
    if ((chdir("/")) < 0){
        syslog(LOG_NOTICE, "ERROR couldn't change directory");
        exit(EXIT_FAILURE);
        
    }
    // making sure daemon have access permessions
    umask(0);
    //making sure libs are open
    fd = open("/dev/null", O_RDWR);
    dup(fd);
    dup(fd);
    
    //handeling kill signal of daemon
    signal(SIGTERM, daemon_kill);
    syslog(LOG_NOTICE,"Daemon: %d is ready", getpid());
    
}



void daemon_kill(int sig_number)
{
    // message to confirm deamon exit
    syslog(LOG_NOTICE,"the daemon with pid %d killed",getpid());
    closelog();
    exit(EXIT_SUCCESS);
    
}