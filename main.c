/*
 work done by:
 Luis Valenzuela
 Abdulaziz Alhawas
 Ahmad Meer

 */



#include "daemon.c"
#define MAXSIZE 100

struct stack
{
    int stk[MAXSIZE];
    int top;
};
typedef struct stack STACK;
STACK s;
int storage [100];

void push(int);
int  pop(void);

// Create a mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


int main(int argc, char** argv) {

    // start the daemon
    daemon_start();
    struct sockaddr_in server_address;
    int server_socket;


    bzero(&server_address, sizeof(server_address));
    //create unammed socket and name it
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    // create addr struct
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    server_address.sin_port = htons(PORT);


    //create, initialize, and set the thread attribute
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_t threads[100];


    //binding unnamed socket
    if (bind(server_socket, (struct sockaddr * )&server_address, sizeof(server_address)) ==-1){
        syslog(LOG_NOTICE, "server with PID %d: error binding socket\n", getpid() );
        exit(EXIT_FAILURE);
    }
    // wait for client connections and put pending connections in queue
    if(listen(server_socket, NUM_CONNECTION) !=0){
        syslog(LOG_NOTICE,"server with PID %d: error listing on socket\n", getpid());
        exit(EXIT_FAILURE);
    }
    while(1){
        int client_socket = accept(server_socket, NULL, NULL);
        syslog(LOG_NOTICE,"server with PID %d: accepted client\n", getpid());

        //create the thread to handel
        pthread_t threads [1];
        // Launch threads
        for(int i=0; i<sizeof(threads)/sizeof(threads[0]); ++i) {
            if(pthread_create(&threads[i], NULL,handel_client,(void*)&client_socket)!= 0) {
                syslog(LOG_NOTICE,"server with PID %d: error creating the thread\n", getpid());
                exit(EXIT_FAILURE);
            }
        }

        // Wait for threads to finish
        for(int i=0; i<sizeof(threads)/sizeof(threads[0]); ++i) {
            if(pthread_join(threads[i],NULL) != 0) {
                perror("Thread join failed");
                exit(EXIT_FAILURE);
            }
        }

        // Release the mutex
        if(pthread_mutex_destroy(&mutex) != 0) {
            perror("Destroy mutex");
            exit(EXIT_FAILURE);
        }


    }
}

//handel client
void* handel_client(void* arg){

    pthread_mutex_lock(&mutex);
    //connect socket to client
    int client_socket = *((int*)arg);
    char input;
    int counter =0;
    int num =0;

    // read char from client
    read(client_socket,&input,sizeof(char));
    num = atoi(&input);

    while (num != 1) {
        syslog(LOG_NOTICE,"input is %d \n",num);
        if (num % 2 == 0) { //if even
            num = num / 2;
        } else { //if odd
            num = num * 3 + 1;
        }
        
        push(num);
        
    }
    
        
    while ((num =pop())!= -1) {
        storage[num] = counter;
        counter+=1;
        
    }


    // send result to client
    syslog(LOG_NOTICE,"result is %d \n",counter-1);
    write(client_socket,&counter,sizeof(char));

    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);

    if (close(client_socket)==-1){
        perror("Error Colsing socket");
        exit(EXIT_FAILURE);
    }else{
        syslog(LOG_NOTICE,"Closed socket to client");

    }


    
}


/*  Function to add an element to the stack */
void push (int num)
{

    if (s.top == (MAXSIZE - 1))
    {
        syslog(LOG_NOTICE,"STACK IS FULL!! \n");
        exit(EXIT_FAILURE);
    }
    else
    {
        s.top = s.top + 1;
        s.stk[s.top] = num;
    }
    return;
}
/*  Function to delete an element from the stack */
int pop ()
{
    int num;
    if (s.top == - 1)
    {
        perror("Stack is Empty\n");
        return (s.top);
    }
    else
    {
        num = s.stk[s.top];

        s.top = s.top - 1;
    }
    return(num);
}

