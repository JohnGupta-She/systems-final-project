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
      struct message_data buff;
      mkfifo("Lobby", 0644);
      int door = open("Lobby", O_RDWR);
      int fval;
      char room[2];
      room[1]=0;

      for (int i = 0; i < 10; i++){
          if(fval){
              fval = fork();
              room[0] = 'a' + i;
          }
      }

      if(!fval){
          mkfifo(room, 0644);
          int readwkp = open(room, O_RDWR);
          int subsrvrfds[MAXCLIENTS];
          int inroom[MAXCLIENTS];
          for(int i = 0; i < MAXCLIENTS; i++){
              inroom[i] = 0;
          }
          int currid = 0;
          fd_set read_fds;
          FD_ZERO(&read_fds);
          FD_SET(readwkp, &read_fds);

          printf("Jazz has been set up\n");
          while(1){
              
              read(readwkp, &buff, sizeof(struct message_data));
              if(buff.id == -1){//if it's the lobby, someone new is here!
                  sprintf(buff.message, "%d", currid);
                  subsrvrfds[currid] = open(buff.message, O_WRONLY);
                  currid++;
                  printf("Room %s has accounted for new client\n", room);
              }
              else if(!inroom[buff.id]){//if they aren't in the room, add them
                  inroom[buff.id] = 1;
                  printf("Added client with id %d to room %s\n", buff.id, room);
              }
              else{//if they are in the room, they are talking, send it to everyone else in the room
                  if(strcmp(buff.message, "/exit") == 0){
                      inroom[buff.id] = 0;
                      printf("Successfully Removed!!!\n");
                  }
                  for(int i = 0; i < MAXCLIENTS; i++){
                      if(inroom[i] /*&& (i != buff.id)*/){
                          write(subsrvrfds[i], &buff, sizeof(struct message_data));
                      }
                  }
              }
          }
      }

      else{
          int roomwkp[10];
          for (int i = 0; i < 10; i++){
              room[0]='a'+i;
              roomwkp[i] = open(room, O_WRONLY);
          }
          strcpy(buff.name, "Lobby");
          buff.id = -1;
          while(1){
              //wait for next client
              printf("Waiting for connection\n");
              int client_socket = accept(listen_socket,(struct sockaddr *)&client_address, &sock_size);

              fval = fork();
              //WHILE SUBSRVR
              if (!fval){
                  struct message_data client_data;
                  int location = 0;//0 for lobby, 1 for room
                  client_data.id = currid;//setting ID
                  printf("Waiting to read\n");
                  read(client_socket, &(client_data.name), 32);//setting Name
                  printf("Read, name: %s\n", client_data.name);

                  sprintf(buff.message, "%d", currid);
                  printf("%s\n", buff.message);
                  mkfifo(buff.message, 0644);
                  printf("Fifo existen\n");
                  int readwkp = open(buff.message, O_RDONLY);//setting up pipes

                  fd_set read_fds;
                  FD_ZERO(&read_fds);
                  FD_SET(client_socket, &read_fds);
                  FD_SET(readwkp, &read_fds);


                  while(1){
                      int i = select(readwkp+1, &read_fds, NULL, NULL, NULL);
                      if (FD_ISSET(client_socket, &read_fds)){
                          if (location == 0){
                              read(client_socket, client_data.message, 1024);
                              location = client_data.message[0] - 'a' + 1;
                              write(roomwkp[location-1], &client_data, sizeof(struct message_data));
                          }
                          else{
                              read(client_socket, client_data.message, 1024);
                              printf("Message received from client: %s\n", client_data.message);
                              write(roomwkp[location-1], &client_data, sizeof(struct message_data));
                              if (strcmp(client_data.message, "/exit") == 0){
                                  location = 0;
                              }
                          }
                          FD_SET(readwkp, &read_fds);
                      }
                      else{
                          read(readwkp, &buff, sizeof(struct message_data));
                          printf("Message received from room %d: %s\n", location, buff.message);
                          write(client_socket, &buff, sizeof(struct message_data));
                          FD_SET(client_socket, &read_fds);
                      }
                  }

              }
              else{
                  for(int i = 0; i < 10; i++){
                      write(roomwkp[i], &buff, sizeof(struct message_data));
                  }
              }

          currid++;
          }
          if(fval){
              int clientfds[MAXCLIENTS];
              sprintf(buff.message, "%d", currid);
              clientfds[currid] = open(buff.message, O_WRONLY);
              }
      }
      free(hints);
      freeaddrinfo(results);
      return 0;
  }
