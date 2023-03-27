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
#include <arpa/inet.h>

#include "fileheader.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void print_usage();

void prepare_sa(struct sockaddr_in *sa, const char * ip, int port);

int process_send(int sockfd, FILE * F, std::string file_name);

#endif