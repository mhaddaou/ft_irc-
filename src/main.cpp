/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou < mhaddaou@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 12:54:55 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/12 14:47:12 by mhaddaou         ###   ########.fr       */
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
            
        
    }
}