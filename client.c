/* 
 work done by:
 Luis Valenzuela
 Abdulaziz Alhawas
 Ahmad Meer

*/




#include "daemon.h"


int main(int argc, char** argv) {
    char input[100];
    int client_socket;
    struct sockaddr_in client_address;
    
    //create unammed socket and name it
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    // create addr struct
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    client_address.sin_port = htons(PORT);
    
    // connect to server
    if (connect(client_socket, (struct sockaddr*) &client_address,sizeof(client_address)) == -1) {
        perror("error connecting to server\n");
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        printf("input : ");
        
        // get the string
        fgets(input, sizeof(input), stdin);
        int i =0;
        
        while (*(input + i)) {
            //request from server
            write(client_socket, input, sizeof(char));
            // handle exit request
            if (*input == 'q') {
                close(client_socket);
                printf("Done \n");
                exit(EXIT_SUCCESS);
                
            }
            read(client_socket, input , sizeof(char));
            i++;
            
        }
        
        
    }
    
}