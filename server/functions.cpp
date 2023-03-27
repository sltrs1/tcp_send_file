#include "functions.h"

#include <stdlib.h>
#include <string>
#include <sstream>

void print_usage() {

    std::cout << "Server usage:" << std::endl;
    std::cout << "arg 1 - Port to listen" << std::endl;

}

int parse_port(int argc, char ** argv) {

    if (argc < 2) {
        std::cerr << "No port provided" << std::endl;
        return -1;
    }

    int port;

    std::istringstream ( std::string(argv[1]) ) >> port;

    if (port <= 0) {
        std::cerr << "Invalid port number" << std::endl;
        return -2;
    }

    return port;
}


void daemonize()
{
    pid_t pid;

    switch(pid = fork())
    {
        case 0:
            setsid();
            chdir("/");
            break;
        case -1:
            perror("Failed to fork daemon\n");
            exit(1);
            break;
        default:
            exit(0);
    }
}

void stop_server(int i) {
        kill(0, SIGKILL);
        exit(0);
}

void prepare_sa(struct sockaddr_in *sa, int port) {

    bzero(sa, sizeof(*sa));
    sa->sin_family  = AF_INET;
    sa->sin_addr.s_addr = htonl(INADDR_ANY);
    sa->sin_port = htons(port);

}

void process_connect(int connfd) {

    int n = 0;
    fh header;
    char buf[FILE_BUF_SIZE];
    FILE * F = nullptr;

    bzero( &header, sizeof(fh) );

    std::cout << "ENTER HANDLER" << std::endl;

    n = recv(connfd, &header, sizeof(fh), 0);

    std::cout << n << std::endl;

    std::string name(header.buf);

    if ( name.empty() ) {
        std::cerr << "Invalid file name" << std::endl;
        close(connfd);
        return;
    }

    std::string savename = "/tmp/" + name + "\0";
    std::cout << savename << std::endl;
    F = fopen( savename.c_str(), "w" );

    if (F == nullptr) {
        std::cerr << "Cannot open file to write" << std::endl;
        std::cout << errno << std::endl;
        close(connfd);
        return;
    }

    while (1) {

        n = recv(connfd, buf, FILE_BUF_SIZE, 0);
        if (n <= 0) {
            break;
        }
        fwrite(buf, 1, n, F);

    }

    fclose(F);
    close(connfd);
    return;

}