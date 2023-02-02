/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou <mhaddaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 12:54:55 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/02/02 19:09:06 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/server.hpp"
// #include <typeinfo>

#include <typeinfo>
#include <arpa/inet.h>
#include <fstream>                        


// const int BUF_SIZE = 1024;

int main(int ac, char **av) {
    if (ac < 3)
        std::cerr << "invalid arguments" << std::endl;
    else{
        Server server(av[1], av[2]);
        // Create socket
        server._socket();
        // Set server address information
        server.setaddrinfo();
        server.lencli = sizeof(server.client_address);
       

        // Bind socket to address and port
        // Start listening for incoming connections
        if (server._bind() == EXIT_FAILURE || server._listen() == EXIT_FAILURE)
            return (EXIT_FAILURE);
        while (true) {
            // Clear the file descriptor set
            FD_ZERO(&server.readfds);
            FD_ZERO(&server.writefds);
            // Add the socket and connected server.clients to the file descriptor set
            FD_SET(server.serverfd, &server.readfds);
            FD_SET(server.serverfd, &server.writefds);
            for (size_t i = 0; i < server.fds.size(); ++i)
            {
                FD_SET(server.fds[i], &server.readfds);
                FD_SET(server.serverfd, &server.writefds);
            }
            // Set up timeout for select()
            server.setTime();
            // Wait for activity on any file descriptor
            if (server._select() == EXIT_FAILURE)
                return (EXIT_FAILURE);
            // Check if there is a new incoming connection
            if (FD_ISSET(server.serverfd, &server.readfds))
            {
                int clientfd = accept(server.serverfd, (struct sockaddr *)&server.client_address, &server.lencli);
                if (clientfd < 0) 
                {
                    std::cerr << "Error accepting connection" << std::endl;
                    continue;
                }
                // Add new client to connected server.clients map
                
                server.fds.push_back(clientfd);
                server.map_clients[clientfd];
                // server.map_clients[clientfd].fd = clientfd;
                server.map_clients[clientfd]._ip = server.client_address.sin_addr.s_addr;
            }

            // Check if any connected server.clients have sent data
            for (size_t i = 0; i < server.fds.size(); ++i ) 
            {   
                if (FD_ISSET(server.fds[i], &server.readfds))
                {
                    memset(server.map_clients[server.fds[i]].buffer, 0, BUF_SIZE);
                    int recev_bytes = recv(server.fds[i], server.map_clients[server.fds[i]].buffer, BUF_SIZE, 0);
                    if (recev_bytes && server.checkQuit(server.map_clients[server.fds[i]].buffer) == EXIT_SUCCESS)
                        recev_bytes = 0;
                    if (server.map_clients[server.fds[i]]._ban == true)
                        recev_bytes = 0;
                    if (recev_bytes == 0)
                        desconectedClient(&server, server.fds[i], i);
                    else if (recev_bytes < 0)
                        std::cout << "error to read " << std::endl;
                    else
                    {
                        if (server.map_clients[server.fds[i]].is_verified == false)
                            connect(&server, server.map_clients[server.fds[i]].buffer, server.fds[i], i);
                        else{
                            handleCmd(&server, server.map_clients[server.fds[i]].buffer, server.fds[i]);
                            
                        }
                    }
                }
            }
        }
    }
    return 0;
}

