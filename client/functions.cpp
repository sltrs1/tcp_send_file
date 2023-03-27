#include "functions.h"

#include <stdlib.h>
#include <string>
#include <sstream>

void print_usage() {

    std::cout << "Client usage:" << std::endl;
    std::cout << "arg 1 - Server's IP adress" << std::endl;
    std::cout << "arg 2 - Server's port" << std::endl;
    std::cout << "arg 3 - Path to file for sending to server" << std::endl;

}

void prepare_sa(struct sockaddr_in *sa, const char * ip, int port) {

    bzero(sa, sizeof(*sa));
    sa->sin_family = AF_INET;
    sa->sin_port= htons(port);
    inet_pton(AF_INET, ip, &(sa->sin_addr));

}

int process_send(int sockfd, FILE * F, std::string file_name) {

    fh header;
    int file_name_len = 0;
    char file_buf[FILE_BUF_SIZE];
    int n;
    int retcode = 0;

    fseek(F, 0, SEEK_END);
    fseek(F, 0,  SEEK_SET);

    bzero(&header, sizeof(header));
    file_name_len = file_name.size();
    if (file_name_len > HEADER_BUF_SIZE-2) {
        file_name_len = HEADER_BUF_SIZE-2;
    }
    memcpy(&(header.buf), file_name.c_str(), file_name_len);

    retcode = send(sockfd, &header, sizeof(header), 0);

    if(retcode == -1) {
        return -1;
    }

    bzero(file_buf, FILE_BUF_SIZE);

    while (!feof(F)) {
        n = fread(file_buf, 1, FILE_BUF_SIZE, F);
        retcode = send(sockfd, file_buf, n, 0);
        if(retcode == -1) {
            return -2;
        }
    }
    return 0;
}