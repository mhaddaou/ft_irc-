
#include "server.hpp"
// #include <typeinfo>
#include <signal.h>

void handler(int)
{
    std::cout << "type Bye to quit\n";
}

int main (int ac, char **av)
{
    signal(SIGINT, handler);
    signal(SIGKILL, handler);
    if (ac < 3)
        error_handler("ERROR:\t Port Or Password Not Provided");    
    else{
        Server server;
        server.sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (server.sockfd < 0)
            error_handler("ERROR:\t Opening Socket");
        memset((void *)&server.ser_add, 0, sizeof(server.ser_add));
        server.port = atoi(av[1]);
        server.password = av[2];
        server.ser_add.sin_family = AF_INET;
        server.ser_add.sin_addr.s_addr = INADDR_ANY;
        server.ser_add.sin_port = htons(server.port);
        
        if (bind(server.sockfd, ((struct sockaddr *) &server.ser_add), sizeof(server.ser_add)) < 0)
            error_handler("ERROR:\t Sending Failed");
                
        if (listen(server.sockfd, 100))
            error_handler("ERROR:\t Listening failed");
        server.clilen = sizeof(server.cli_add);
        
        server.newsockfd = accept(server.sockfd, (struct sockaddr *)&server.cli_add, &server.clilen);
        if (server.newsockfd < 0)
            error_handler("ERROR:\t Accepting failed");
        server.clients_sock.push_back(server.newsockfd);

        while (true){
            memset((void*)server.buffer, 0, 255);
            server.n = read(server.clients_sock[server.clients_sock.size() - 1], server.buffer, 254);
            if (server.n < 0)
                error_handler("ERROR:\t Reading failed");
            std::cout << "Client: " << server.buffer << std::endl;
            // memset((void*)server.buffer, 0, 255);
            // std::fgets(server.buffer, 255, stdin);
            // server.n = write(server.newsockfd, server.buffer ,sizeof(server.buffer) );
            // if (server.n < 0)
            //     error_handler("ERROR:\t Writing failed");

            if (std::strncmp("Bye", server.buffer, 3) == 0)
                break;            
        }
        close(server.newsockfd);
        close(server.sockfd);
        return (0);
    }
}