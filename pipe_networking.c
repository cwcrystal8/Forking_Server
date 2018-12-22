#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client
  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
void server_begin_handshake(int *from_client) {
  printf("Server: begin making Well Known Pipe\n");
  mkfifo("WKP", 0644);
  printf("Server: finished making Well Known Pipe\n\n");

  printf("Server: begin opening Well Known Pipe\n");
  *from_client = open("WKP", O_RDONLY);
  printf("Server: finished opening Well Known Pipe\n\n");
}

void server_finish_handshake(int upstream, int *to_client, int *from_client){
    printf("Subserver [%d]: begin reading from Well Known Pipe to get name of private pipe\n", getpid());
    char name[HANDSHAKE_BUFFER_SIZE];
    read(*from_client, name, HANDSHAKE_BUFFER_SIZE);
    printf("Subserver [%d]: finished reading from Well Known Pipe to get name of private pipe, value read: [%s]\n\n", getpid(), name);

    printf("Subserver [%d]: begin opening private pipe with name [%s]\n", getpid(), name);
    *to_client = open(name, O_WRONLY);
    printf("Subserver [%d]: finished opening private pipe with name [%s]\n\n", getpid(), name);

    printf("Subserver [%d]: begin writing to private pipe, value to write: [%s]\n", getpid(), ACK);
    write(*to_client, ACK, HANDSHAKE_BUFFER_SIZE);
    printf("Subserver [%d]: finished writing to private pipe, value written: [%s]\n\n", getpid(), ACK);

    printf("Subserver [%d]: begin reading from Well Known Pipe to verify acknowledgement\n", getpid());
    read(upstream, name, HANDSHAKE_BUFFER_SIZE);
    printf("Subserver [%d]: finished reading from Well Known Pipe to verify acknowledgement, value read: [%s]\n\n", getpid(), name);

    printf("Subserver [%d]: begin verifying values\n", getpid());
    if(strcmp(name, ACK)) printf("Error verifying acknowledgement\n");
    else printf("Subserver [%d]: acknowledgement was successfully verified!\n", getpid());
    printf("Subserver [%d]: finished verifying values\n\n", getpid());
}
/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int pid = getpid();
  char buf[HANDSHAKE_BUFFER_SIZE], *count = buf;
  while(pid){
    *count = (pid % 10) + 48;
    pid /= 10;
    count++;
  }
  *count = '\0';
  printf("Client [%d]: begin making private pipe\n", getpid());
  mkfifo(buf, 0644);
  printf("Client [%d]: finished making private pipe\n\n", getpid());

  printf("Client [%d]: begin opening Well Known Pipe to send name of private pipe\n", getpid());
  *to_server = open("WKP",O_WRONLY);
  printf("Client [%d]: finished opening Well Known Pipe to send name of private pipe\n\n", getpid());

  printf("Client [%d]: begin writing name of private pipe to Well Known Pipe, value to write: [%s]\n", getpid(), buf);
  write(*to_server, buf, HANDSHAKE_BUFFER_SIZE);
  printf("Client [%d]: finished writing name of private pipe to Well Known Pipe, value written: [%s]\n\n", getpid(), buf);

  char acknowledge[HANDSHAKE_BUFFER_SIZE];
  printf("Client [%d]: begin opening private pipe\n", getpid());
  int downstream = open(buf, O_RDONLY);
  printf("Client [%d]: finished opening private pipe\n\n", getpid());

  printf("Client [%d]: begin reading acknowledgement from private pipe\n", getpid());
  read(downstream, acknowledge, HANDSHAKE_BUFFER_SIZE);
  printf("Client [%d]: finished reading acknowledgement from private pipe, value read: [%s]\n\n", getpid(), acknowledge);

  printf("Client [%d]: begin removing private pipe\n", getpid());
  remove(buf);
  printf("Client [%d]: finished removing private pipe\n\n", getpid());

  printf("Client [%d]: begin writing received message to Well Known Pipe, value to write: [%s]\n", getpid(), acknowledge);
  write(*to_server, acknowledge, HANDSHAKE_BUFFER_SIZE);
  printf("Client [%d]: finished writing received message to Well Known Pipe, value written: [%s]\n\n", getpid(), acknowledge);

  return downstream;
}
