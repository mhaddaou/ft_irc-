/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smia <smia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 11:10:21 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/19 01:39:19 by smia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

Server::Server(char * port, char * passwd){
    this->_port = atoi(port);
    this->_passwd = passwd;
}

void Server::setaddrinfo()
{
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

void nick(Server *server,std::vector<std::string> cmd, int fd){
    if (cmd[0] == "NICK")
        if (cmd.size() == 2){
            server->map_clients[fd].setNickName(cmd[1]);
            server->map_clients[fd].incrementVerf();
        //    std::cout <<"fd: " << fd << "  verif : "<< server->map_clients[fd].verif << " :" <<  "PASS OK" << std::endl;
        }
}

void user(Server *server, std::vector<std::string> cmd, int fd){
    if (cmd[0] == "USER"){
        server->map_clients[fd].setName(cmd[1]);
        // server->map_clients[fd].setRealName(cmd[4]);
        server->map_clients[fd].incrementVerf();
        // server->map_clients[fd].id = atoi(cmd[2].c_str());
        // std::cout <<"fd: " << fd << "  verif : "<< server->map_clients[fd].verif << " :" <<  "PASS OK" << std::endl;
    }
}

void passwd(Server *server, std::vector<std::string> cmd,  int fd)
{    
    if (cmd[0] == "PASS")
    {
        //check if password
        if (cmd.size() == 2)
        { 
                server->map_clients[fd].setPassword(cmd[1]);
                server->map_clients[fd].incrementVerf();
                // std::cout <<"fd: " << fd << "  verif : "<< server->map_clients[fd].verif << " :" <<  "PASS OK" << std::endl;
        }
    }
}

void connect (Server *server,char *buffer, int fd)
{
    std::vector<std::string> cmd = server->splitCMD(buffer);
    nick(server,cmd, fd);
    user(server, cmd, fd);
    passwd(server, cmd, fd);
    if (server->map_clients[fd].is_verified())
        std::cout << server->map_clients[fd].getName() << "  is connected!" << std::endl;
}