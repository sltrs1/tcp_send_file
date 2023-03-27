#include "functions.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        print_usage();
    }

    int port = parse_port(argc, argv);

    if (port <= 0) {
        return -1;
    }

    daemonize();

    signal(SIGTERM, stop_server);
    signal(SIGHUP,  stop_server);

    int					listenfd, connfd;
    pid_t				childpid;
    socklen_t			clilen;
    struct sockaddr_in	cliaddr, servaddr;
    int retcode = 0;

    // Prepare socket structure
    prepare_sa(&servaddr, port);

    // Create socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        std::cerr << "socket error" << std::endl;
        exit(2);
    }

    retcode = bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    if (retcode != 0) {
        std::cerr << "bind error, errno = " << errno << std::endl;
        exit(3);
    }

    // Listen to socket
    retcode = listen(listenfd, 1024);

    if (retcode != 0) {
        std::cerr << "listen error, errno = " << errno << std::endl;
        exit(4);
    }

    // Accept and process 
    for ( ; ; ) {

        //std::cout << "MAIN LOOP" << std::endl;

        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, ( struct sockaddr *) &cliaddr, &clilen);

        //std::cout << "ACCEPTED" << std::endl;

        if ( (childpid = fork()) == 0) {	// child process 
            close(listenfd);	            // close listening socket 
            process_connect(connfd);	    // process the request 
            exit(0);
        }

        //close(connfd);			// parent closes connected socket 
    }

    return 0;
}

