#include "pipe_networking.h"

void process_data(char *data){
  while(*data){
    *data = 25 - (*data) + 2 * 'a';
    data++;
  }
}

static void sighandler(int signo){
  if(signo == SIGINT){
    remove("WKP");
    exit(0);
  }
}

int main() {
  signal(SIGINT,sighandler);

  int to_client;
  int from_client;

  while(1){
    server_begin_handshake(&from_client);
    int cpid = fork();
    if(cpid){
      close(from_client);

      printf("Server: begin removing Well Known Pipe\n");
      remove("WKP");
      printf("Server: finished removing Well Known Pipe\n\n");
    }
    else{
      server_finish_handshake(from_client, &to_client, &from_client);
      char output[BUFFER_SIZE];
      while(read(from_client, output, BUFFER_SIZE)){
          output[strlen(output) - 1] = '\0';
          printf("Subserver [%d]: processing data of value [%s]\n", getpid(), output);
          process_data(output);
          write(to_client, output, BUFFER_SIZE);
          printf("Subserver [%d]: data processed with value [%s] and sent to client\n\n", getpid(), output);
      }
      return 0;
    }
  }
}
