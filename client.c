#include "pipe_networking.h"

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  char input[BUFFER_SIZE], output[BUFFER_SIZE];
  while(1){
    printf("[Client] Enter data: ");
    fgets(input, BUFFER_SIZE, stdin);
    write(to_server, input, BUFFER_SIZE);
    while(!read(from_server, output, BUFFER_SIZE));
    printf("[Client] New and improved data: %s\n\n", output);
  }
}
