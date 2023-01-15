/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou < mhaddaou@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 11:10:21 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/14 21:43:26 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

Server::Server(char * port, char * passwd){
    this->_port = atoi(port);
    this->_passwd = passwd;
}

void Server::setaddrinfo(){
    
    memset(&this->server_address, 0, sizeof(this->server_address));
    this->server_address.sin_family = AF_INET;
    this->server_address.sin_addr.s_addr = INADDR_ANY;
    this->server_address.sin_port = htons(this->_port);
}
int Server::_socket(){
    this->serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}
int Server::_bind(){
    if (bind(this->serverfd, (struct sockaddr *)&this->server_address, sizeof(this->server_address)) < 0){
        std::cerr << "bind failed" << std::endl;
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}
int Server::_listen(){
    if (listen(this->serverfd, MAX_CLIENT) < 0){
        std::cerr << "listen failed" << std::endl;
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}
void Server::listOfSockets(){
    FD_ZERO(&this->sockets_list);
    FD_SET(this->serverfd, &this->sockets_list);
}
void Server::setTime(){
    this->timeout.tv_sec = 1;
    this->timeout.tv_usec = 0;
}
int Server::_select(){
    if (select(FD_SETSIZE, &this->readfds,NULL, NULL, &this->timeout ) < 0){
        std::cerr << "error in select" << std::endl;
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}
std::vector<std::string> Server::splitCMD(std::string msg){
    
    std::vector<std::string> cmd;
    std::stringstream ss(msg);
    std::string word;
    while (getline(ss, word, ' ')) { 
    cmd.push_back(word);
    }
    return (cmd);
}
int Server::getPort(){
    return (this->_port);
}

int Server::_accept(){
    int newfd = accept(this->serverfd, NULL, NULL);
    if (newfd < 0){
        std::cerr << "error in accept" << std::endl;
        return (EXIT_FAILURE);
    }
    // send(newfd, "Please enter first name", 23, 0);
    // while(true){
    //     int bytes_received = recv(newfd, this->buffer, BUF_SIZE, 0);
    //     if (bytes_received > 0){
    //         std::vector<std::string> cmd = this->splitCMD(this->buffer);
    //         memset((void *)this->buffer, 0, BUF_SIZE);
    //         std::cout << "`" << cmd[2] << "`" << std::endl;
    //         std::cout << cmd.size() << std::endl;
    //     }
    // }
    this->clients.push_back(newfd);
    std::cout << "new client connected" << std::endl;
    return (EXIT_SUCCESS);
}