#include "headers.h"


int main(){
    struct addrinfo hints, *results;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int err;
    //         getaddrinfo(address,port,...)
    if ((err = getaddrinfo("149.89.161.100", "23456", &hints, &results)) != 0) {
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
    printf("NOMBRE DEL SENOR(A)?!?!!?:\n");
    char name[32];
    fgets(name, 31, stdin);
    name[strlen(name)-1] = 0;
    write(sd, name, 32);//setting Name

    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(sd, &read_fds);
    FD_SET(STDIN_FILENO, &read_fds);//setting up select

    struct message_data buff;
    int location = 0;
    while(1){
        if (location == 0){
            printf("Choose a room, a through j!\n");
        }
        int i = select(sd+1, &read_fds, NULL, NULL, NULL);
        if (FD_ISSET(STDIN_FILENO, &read_fds)){
            if (location == 0){
                fgets(buff.message, 1023, stdin);
                buff.message[strlen(buff.message)-1] = 0;
                if (buff.message[0] < 'a' || buff.message[0] > 'j' || strlen(buff.message) != 1){
                    printf("Not a valid room!\n");
                }
                else{
                    printf("\e[1;1H\e[2J");
                    printf("Now in room %s\n", buff.message);
                    location = buff.message[0] - 'a' + 1;
                    write(sd, buff.message, 1024);
                }
                FD_SET(sd, &read_fds);
            }
            else{
                fgets(buff.message, 1023, stdin);
                buff.message[strlen(buff.message)-1] = 0;
                if (strcmp(buff.message, "/exit") == 0){
                    location = 0;
                    printf("\e[1;1H\e[2J");
                }
                write(sd, buff.message, 1024);
                FD_SET(sd, &read_fds);
            }
        }

        else{
            read(sd, &buff, sizeof(struct message_data));
            printf("\x1B[32m%s: \x1B[0m%s\n", buff.name, buff.message);
            FD_SET(STDIN_FILENO, &read_fds);
        }
    }
    freeaddrinfo(results);
    return 0;
}
