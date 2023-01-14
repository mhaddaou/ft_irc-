/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou <mhaddaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 12:54:55 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/14 04:00:54 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/server.hpp"
// #include <typeinfo>
#include <sys/select.h>
// #include "winsock.h"
// #pragma comment(lib, "Ws2_32.lib")

std::vector<Server> fd_clinets;
int n_MaxFD;
Server server;
int ncli[5] = {0, };
fd_set fr, fw, fe;

void handlerNewConnection(){
    int nNewClient = accept(server.sockfd, (struct sockaddr *)&server.cli_add, &server.clilen);
    if (nNewClient < 0)
        std::cerr << "error in accept()" << std::endl;
    else{
        int nindex;
        for (nindex = 0; nindex <5; nindex++){
            if (ncli[nindex] == 0){
                ncli[nindex] =  nNewClient;
                if (nNewClient > n_MaxFD)
                    n_MaxFD =  nNewClient + 1;
                break;
            }
            
        }
        if (nindex == 5)
            std::cerr << "server cannot accept any more" << std::endl;
    }
}
void handeldatafromclient(){
    for (int i = 0; i < 5; i++){
        if (ncli[i] > 0){
            if (FD_ISSET(ncli[i], &fr)){
                char sbuf[255] = {0, };
                int nret = recv(ncli[i], sbuf, 255, 0);
                if (nret < 0){
                    std::cerr << "error at client socket" << std::endl;
                    // closesocket(ncli[i]);
                    int closesocket( ncli[i]);
                    // closesockt(ncli[i]) = 0;
                    closesocket = 0;
                }
                else{
                    std::cout << "received data from :" << ncli[i] << "message : " << sbuf << std::endl;
                    break;
                }
            }
        }
    }
    
}
int main (int ac, char **av)
{
    if (ac < 3)
        error_handler(0);    
    else{
        server.sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (server.sockfd < 0)
            error_handler(1);
        memset((void *)&server.ser_add, 0, sizeof(server.ser_add));
        server.port = atoi(av[1]);
        server.password = av[2];
        server.ser_add.sin_family = AF_INET;
        server.ser_add.sin_addr.s_addr = INADDR_ANY;
        server.ser_add.sin_port = htons(server.port);
        
        if (bind(server.sockfd, ((struct sockaddr *) &server.ser_add), sizeof(server.ser_add)) < 0)
            error_handler(2);
        if (listen(server.sockfd, 5))
            error_handler(3);
        n_MaxFD = server.sockfd + 1;
        int rd;

        while (true){
            FD_ZERO(&fr);
            FD_SET(server.sockfd, &fr);
            for ( int i = 0; i < 5; i++){
                if (ncli[i] > 0)
                    FD_SET(ncli[i], &fr);
            }
            rd = select(n_MaxFD, &fr, NULL, NULL, NULL);
            if (rd < 0)
                std::cerr << " error in select " << std::endl;
            if (rd == 0)
                std::cout << "no client in this port" << std::endl;
            else {
                if (FD_ISSET(server.sockfd, &fr))
                    handlerNewConnection();
                else
                    handeldatafromclient(); 
            }
            
        }
        
        // server.clilen = sizeof(server.cli_add);
        // server.newsockfd = accept(server.sockfd, (struct sockaddr *)&server.cli_add, &server.clilen);
        // if (server.newsockfd < 0)
        //     error_handler(4);
        // while (true){
        //     memset((void*)server.buffer, 0, 255);
        //     server.n = read(server.newsockfd, server.buffer, 254);
        //     if (server.n < 0)
        //         error_handler(5);
        //     std::cout << "Client: " << server.buffer << std::endl;
        //     // memset((void*)server.buffer, 0, sizeof(server.buffer));
        //     // std::fgets(server.buffer, 254, stdin);
        //     // server.n = write(server.newsockfd, server.buffer ,sizeof(server.buffer) );
        //     // if (server.n < 0)
        //     //     error_handler(6);
        //     if (std::strcmp("Bye", server.buffer) == 0)
        //         break;            
        // }

        return (0);
    }
}