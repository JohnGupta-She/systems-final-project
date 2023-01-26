#include "headers.h"


int main(int argc, char *argv[]){
    if(argc < 3) {
        printf("Provide server address and port as a parameters\n");
        exit(1);
    }
    printf("connecting to %s:%s\n",argv[1],argv[2]);


    struct addrinfo hints, *results;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int err;
    //         getaddrinfo(address,port,...)
    if ((err = getaddrinfo(argv[1], argv[2], &hints, &results)) != 0) {
        printf("error %d : %s\n", err, strerror(err));
        return 1;
    }

    int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    if(sd < 0){
        printf("%s\n",strerror(errno));
        exit(1);
    }

    if ( connect(sd, results->ai_addr, results->ai_addrlen) < 0) {
        printf("Failed to connect\n");
        exit(1);
    }

    //DO STUFF
    char name[32];
    fgets(name, 31, stdin);
    name[strlen(name)-1] = 0;
    write(sd, name, 32);//setting Name

    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(sd, &read_fds);
    FD_SET(STDIN_FILENO, &read_fds);//setting up select

    char message[1024];
    while(1){
        printf("Message!!!!!::::!!!\n");
        int i = select(sd+1, &read_fds, NULL, NULL, NULL);
        if (FD_ISSET(STDIN_FILENO, &read_fds)){
            fgets(message, 1023, stdin);
            message[strlen(message)-1] = 0;
            write(sd, message, 1024);
            FD_SET(sd, &read_fds);
        }
        else{
            read(sd, message, 1024);
            printf("%s\n", message);
            FD_SET(STDIN_FILENO, &read_fds);
        }
    }
    freeaddrinfo(results);
    return 0;
}
