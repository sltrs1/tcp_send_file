#include "functions.h"

int main(int argc, char *argv[]) {

    int port;
    struct sockaddr_in	addr;
    FILE * F = nullptr;
    int retcode = 0;
    int	sockfd;

    if (argc < 4) {
        print_usage();
        return 1;
    }
    
    if ( !(inet_pton(AF_INET, argv[1], &addr.sin_addr) > 0) )
    {
        std::cerr << "Invalid IP-address" << std::endl;
        return 2;
    }

    std::istringstream ( std::string(argv[2]) ) >> port;
    if (port <= 0) {
        std::cerr << "Invalid port" << std::endl;
        return 3;
    }

    F = fopen(argv[3], "rb");

    if (!F) {
        std::cerr << "Cannot open file, errno = " << errno << std::endl;
        return 4;
    }

    std::string path(argv[3]);
    std::vector<std::string> elems;
    std::stringstream ss(path);
    std::string item;
    while (std::getline(ss, item, '/')) {
        elems.push_back(item);
    }
    std::string file_name = elems.back();

    prepare_sa(&addr, argv[1], port);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1 ) {
        std::cerr << "socket error, errno = " << errno << std::endl;
        return 5;
    }

    retcode = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));

    if (retcode != 0) {
        std::cerr << "connect error, errno = " << errno << std::endl;
        return 6;
    }

    retcode = process_send(sockfd, F, file_name);

    switch (retcode)
    {
    case -1:
        std::cerr << "Error sending header" << std::endl;
        return 7;
        break;
    case -2:
        std::cerr << "Error sending file" << std::endl;
        return 8;
        break;
    case 0:
        std::cout << "File sent successfully" << std::endl;
        break;
    default:
        std::cerr << "Unknown return code" << std::endl;
        break;
    }

    fclose(F);

    return 0;
}

