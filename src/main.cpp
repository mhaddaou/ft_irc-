/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou < mhaddaou@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 12:54:55 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/14 17:55:35 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/server.hpp"
// #include <typeinfo>

#include <typeinfo>
int main (int ac , char **av){
    
    if (ac < 3){
        std::cerr << "error in the argument" << std::endl; return 1;
    }
    else{
        // std::cout << typeid(av[1]).name() << std::endl;
        Server server(av[1], av[2]);
         server.serverfd = socket(AF_INET, SOCK_STREAM, 0);
            if (server.serverfd < 0) {
            std::cerr << "Error creating socket" << std::endl;
        return 1;
    }
        server.setaddrinfo();
        if (server._bind() == EXIT_FAILURE){
            return (EXIT_FAILURE);
        }
        if (server._listen() == EXIT_FAILURE){
            return (EXIT_FAILURE);
        }
        // server.listOfSockets();
        server.setTime();
        while (true){
            // clear the file descriptor set
            FD_ZERO(&server.readfds);
            //add the socket and connected clients to the file descriptor set
            FD_SET(server.serverfd, &server.readfds);
            for(int i=0; i < server.clients.size(); i++){
                FD_SET(server.clients[i], &server.readfds);
            }
            //wait for activity on any file descriptor
            if (server._select() == EXIT_FAILURE)
                return (EXIT_FAILURE);
            // check if there is a new incoming connection
            // and add new client to connected clients vector
            if (FD_ISSET(server.serverfd, &server.readfds)){
                if (server._accept() == EXIT_FAILURE)
                    continue;
            }
            //check if any connected clients have sent data
            // for (int i = 0; i < server.clients.size(); i++){
            //     if (FD_ISSET(server.serverfd, &server.readfds)){
                    
            //     }
                
            // }
        }
        
    }
}