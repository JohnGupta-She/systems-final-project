#include "headers.h"

int main(){
      struct addrinfo * hints, * results;
      hints = calloc(1,sizeof(struct addrinfo));

      hints->ai_family = AF_INET;
      hints->ai_socktype = SOCK_STREAM; //TCP socket
      hints->ai_flags = AI_PASSIVE; //only needed on server
      getaddrinfo(NULL, "23456" , hints, &results);  //NULL is localhost or 127.0.0.1

      //create socket
      int listen_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
      bind(listen_socket, results->ai_addr, results->ai_addrlen);
      listen(listen_socket, 10);//10 clients can wait to be processed

      socklen_t sock_size;
      struct sockaddr_storage client_address;
      sock_size = sizeof(client_address);

      struct timeval time;
      time.tv_sec = 2;
      time.tv_usec = 0;
      fd_set listen_fd;

      int currid = 0;
      char buff[1024];
      mkfifo("Lobby", 0644);
      int door = open("Lobby", O_RDWR);
      int fval = 1;
      while(1){
          //wait for next client
          FD_ZERO(&listen_fd);
          FD_SET(listen_socket, &listen_fd);
          int i = select(listen_socket+1, &listen_fd, NULL, NULL, &time);
          if(FD_ISSET(listen_socket, &listen_fd)){
          printf("Waiting for connection\n");
          int client_socket = accept(listen_socket,(struct sockaddr *)&client_address, &sock_size);

          fval = fork();
          //WHILE SUBSRVR
          if (!fval){
          struct message_data client_data;
          client_data.id = currid;//setting ID
          printf("Waiting to read\n");
          read(client_socket, &(client_data.name), 32);//setting Name
          printf("Read, name: %s\n", client_data.name);

          sprintf(buff, "%d", currid);
          printf("%s\n", buff);
          mkfifo(buff, 0644);
          printf("Fifo existen\n");
          int readwkp = open(buff, O_RDONLY);//setting up pipes

          fd_set read_fds;
          FD_ZERO(&read_fds);
          FD_SET(client_socket, &read_fds);
          FD_SET(readwkp, &read_fds);


          while(1){
              int i = select(readwkp+1, &read_fds, NULL, NULL, NULL);
              if (FD_ISSET(client_socket, &read_fds)){
                  read(client_socket, buff, 1024);
                  printf("%s\n", buff);
                  write(door, buff, 1024);
                  FD_SET(readwkp, &read_fds);
              }
              else{
                  read(readwkp, buff, 1024);
                  printf("%s\n", buff);
                  FD_SET(client_socket, &read_fds);
              }
          }
          }
          currid++;
      }
      if(fval){
              int clientfds[MAXCLIENTS];
              sprintf(buff, "%d", currid);
              clientfds[currid] = open(buff, O_WRONLY);
              //while(1){
                  read(door, buff, 1024);
                  printf("The doorman says %s\n", buff);
                  for (int i = 0; i < currid + 1; i++){
                      write(clientfds[i], buff, 1024);
              //    }
              }
          }
      }

      free(hints);
      freeaddrinfo(results);
      return 0;
  }
