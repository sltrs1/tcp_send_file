#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/un.h>
#include <errno.h>
#include <sys/signal.h>
#include <wait.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "fileheader.h"

#include <iostream>

void print_usage();

int parse_port(int argc, char ** argv);

void daemonize();

void stop_server(int i);

void prepare_sa(struct sockaddr_in *sa, int port);

void process_connect(int connfd);

#endif