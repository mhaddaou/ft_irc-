
#pragma once

# include <iostream>
# include <string>
# include <cstring>
# include <netdb.h>
# include <cstdlib>
# include <sys/socket.h>
# include <netinet/in.h>
# include <fstream>
# include <unistd.h>
#include <vector>
class clients
{
    
};
class Server{
    public:
        int sockfd;
        int newsockfd;
        int port;
        int n;
        std::string password;
        std::ifstream inputFile;
        char buffer[255];
        std::string bfr;
        struct sockaddr_in ser_add;
        struct sockaddr_in cli_add;
        std::vector<int> clients_sock;
        socklen_t clilen;
        Server(){}
        ~Server(){}
};

void error_handler(std::string s)
{
    std::cerr << s << std::endl;
}