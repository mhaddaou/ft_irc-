/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smia <smia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 12:54:55 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/19 01:42:21 by smia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/server.hpp"
// #include <typeinfo>

#include <typeinfo>
#include <arpa/inet.h>


// const int BUF_SIZE = 1024;

int main(int ac, char **av) {
    if (ac < 3)
        std::cerr << "invalid arguments" << std::endl;
    else{
        
        // char syn_ack[] = "SYN-ACK";
        
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
            // Add the socket and connected server.clients to the file descriptor set
            FD_SET(server.serverfd, &server.readfds);
            for (iterator it = server.map_clients.begin(); it != server.map_clients.end(); ++it)
            {
                FD_SET(it->first, &server.readfds);
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
                server.map_clients[clientfd];
            }

            // Check if any connected server.clients have sent data
            for (iterator it = server.map_clients.begin(); it != server.map_clients.end(); ++it) 
            {
                if (FD_ISSET(it->first, &server.readfds)) 
                {
                    memset(it->second.buffer, 0, BUF_SIZE);
                    int x = recv(it->first, it->second.buffer, BUF_SIZE, 0);
                    if (x <= 0)
                    {
                        std::cout << it->second.getName() <<" disconnected" << std::endl;
                        x = it->first;
                        ++it;
                        close(x);
                        server.map_clients.erase(x);
                    } 
                    else 
                    {
                        if (it->second.verified == false)
                            connect(&server, it->second.buffer, it->first);
                        else
                        {
                            // std::cout << it->second.buffer << std::endl;
                            // if (server.isCmd(it->second.buffer))
                            // {
                            //     // execute commande
                            // }
                            // else
                            // {
                                
                            // }
                        }
                            // for (iterator it1; it1 != server.map_clients.end(); it1++) {
                            //     if (it1 != it) {
                            //         send(it1->first, it1->second.buffer, x, 0);
                            //     }
                        // std::cout << it->second.buffer << std::endl;
                        // send(it->first,":localhost CAP * LS :multi-prefix sasl", sizeof(":localhost CAP * LS :multi-prefix sasl"), 0);  
                        // std::cout << "--" << it->second.buffer<< "--" << std::endl;
                        // if (strcmp(it->second.buffer,syn_ack) == 0)
                        //     send (it->first, syn_ack, sizeof(syn_ack), 0);

                        // Add code here to handle the received message and send it to other server.clients as necessary

                        }
                    }
                }
            }
        }
    return 0;
    }

