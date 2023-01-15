/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou < mhaddaou@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 12:54:55 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/15 10:32:35 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/server.hpp"
// #include <typeinfo>

#include <typeinfo>

const int MAX_CLIENTS = 10;
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
       

        // Bind socket to address and port
        // Start listening for incoming connections
        if (server._bind() == EXIT_FAILURE || server._listen() == EXIT_FAILURE)
            return (EXIT_FAILURE);
        memset(server.buffer, 0, BUF_SIZE);
        while (true) {
            // Clear the file descriptor set
            FD_ZERO(&server.readfds);

            // Add the socket and connected server.clients to the file descriptor set
            FD_SET(server.serverfd, &server.readfds);
            for (unsigned int i = 0; i < server.clients.size(); i++) {
                FD_SET(server.clients[i], &server.readfds);
            }

            // Set up timeout for select()
            server.setTime();
    
           
            // Wait for activity on any file descriptor
            if (server._select() == EXIT_FAILURE)
                return (EXIT_FAILURE);

            // Check if there is a new incoming connection
            if (FD_ISSET(server.serverfd, &server.readfds)) {
                int clientfd = accept(server.serverfd, NULL, NULL);
                if (clientfd < 0) {
                    std::cerr << "Error accepting connection" << std::endl;
                    continue;
                }
                // send(clientfd,"name :", 6, 0);
                // Add new client to connected server.clients vector
                server.clients.push_back(clientfd);
                std::cout << "New client connected" << std::endl;
            }

            // Check if any connected server.clients have sent data
            for ( unsigned int i = 0; i < server.clients.size(); i++) {
                if (FD_ISSET(server.clients[i], &server.readfds)) {
                    int bytes_received = recv(server.clients[i], server.buffer, BUF_SIZE, 0);
                    if (bytes_received <= 0) {
                        close(server.clients[i]);
                        server.clients.erase(server.clients.begin() + i);
                        std::cout << "Client disconnected" << std::endl;
                    } else {
                        std::cout << "Received message: " << server.buffer << std::endl;
                        memset(server.buffer, 0,  BUF_SIZE);
                        // Add code here to handle the received message and send it to other server.clients as necessary
                        // for (int j = 0; j < server.clients.size(); j++) {
                        //     if (j != i) {
                        //         send(server.clients[j], buf, bytes_received, 0);
                        //     }
                        // }
                    }
                }
            }
        }
    }
    return 0;
}
