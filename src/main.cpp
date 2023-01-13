/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou < mhaddaou@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 12:54:55 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/13 09:37:41 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/server.hpp"
// #include <typeinfo>

int main (int ac, char **av)
{
    if (ac < 3)
        error_handler(0);    
    else{
        Server server;
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
            
        if (listen(server.sockfd, 100))
            error_handler(3);
        server.clilen = sizeof(server.cli_add);
        server.newsockfd = accept(server.sockfd, (struct sockaddr *)&server.cli_add, &server.clilen);
        if (server.newsockfd < 0)
            error_handler(4);
        while (true){
            memset((void*)server.buffer, 0, 255);
            server.n = read(server.newsockfd, server.buffer, 255);
            if (server.n < 0)
                error_handler(5);
            std::cout << "Client: " << server.buffer << std::endl;
            memset((void*)server.buffer, 0, 255);
            std::fgets(server.buffer, 255, stdin);
            server.n = write(server.newsockfd, server.buffer ,sizeof(server.buffer) );
            if (server.n < 0)
                error_handler(6);
            if (std::strcmp("Bye", server.buffer) == 0)
                break;            
        }
        close(server.newsockfd);
        close(server.sockfd);
        return (0);
    }
}