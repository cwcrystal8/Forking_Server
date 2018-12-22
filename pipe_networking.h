#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#ifndef NETWORKING_H
#define NETWORKING_H
#define ACK "HOLA"

#define HANDSHAKE_BUFFER_SIZE 10
#define BUFFER_SIZE 1000

void server_begin_handshake(int *from_client);

void server_finish_handshake(int upstream, int *to_client, int *from_client);

int client_handshake(int *to_server);

#endif
