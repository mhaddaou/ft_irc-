/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou <mhaddaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 11:10:21 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/19 21:54:44 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

Server::Server(char * port, char * passwd){
    this->_port = atoi(port);
    this->_passwd = this->encrypt(passwd);
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
        for (size_t i = 0; i < cmd.size(); i++){
            std::cout << cmd[i];
        }
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
        cmd[1].erase(std::remove(cmd[1].begin(), cmd[1].end(), '\n'), cmd[1].cend());
        cmd[1].erase(std::remove(cmd[1].begin(), cmd[1].end(), '\r'), cmd[1].cend());
        
        //check if password
        if (cmd.size() == 2)
        {
            if (server->getPass() == server->encrypt(cmd[1])){

                server->map_clients[fd].setPassword(server->encrypt(cmd[1]));
                server->map_clients[fd].incrementVerf();
            }
            
        }
    }
}

std::string Server::encrypt(std::string password){
    int key = 25;
    for (size_t i = 0; i < password.length(); i++){
        password[i] = password[i] - key;
    }
    return password;
}
std::string Server::decrypt(std::string password){
    int key = 25;
    for (size_t i = 0; i < password.length(); i++){
        password[i] += key;
    }
    return password;
}
    
void connect (Server *server,char *buffer, int fd)
{
    std::vector<std::string> cmd = server->splitCMD(buffer);
    if (server->map_clients[fd].verif == 0)
        passwd(server, cmd, fd);
    else if (server->map_clients[fd].verif == 1)
        nick(server,cmd, fd);
    else if (server->map_clients[fd].verif == 2)
        user(server, cmd, fd);
    if (server->map_clients[fd].is_verified()){
        std::string rpl = ":localhost 001 " + server->map_clients[fd].getNickName() + " : welcome to the server\r\n";
        send(fd, rpl.c_str(), rpl.size(), 0);
        std::cout << server->map_clients[fd].getNickName()<< " "<< fd << "  is connected!" << std::endl;
    }     
}
void setPrvMsg(Server *server, std::vector<std::string> cmd){
    int fd;
    for (iterator it = server->map_clients.begin(); it != server->map_clients.end(); ++it)
    {
        if (it->second.getNickName() == cmd[1]){
            fd = it->first;
        }
    }
    std::string rpl = cmd[2];
    rpl.erase(0, 1);
    // std::cout << rpl << std::endl;
    send(fd, rpl.c_str(), rpl.size() , 0 );
}
int Server::checkQuit(std::string str){
    std::vector<std::string> cmd = this->splitCMD(str);
    if (strcmp(cmd[0].c_str(), "QUIT") == 0)
            return (EXIT_SUCCESS);
    return (EXIT_FAILURE);
}
void handleCmd(Server *server, char *buffer, int fd){
    std::cout << buffer << "l" << std::endl;
    std::string hel = buffer;
    std::cout << hel.size() << std::endl;
    // std::cout << buffer << std::endl;
    std::vector<std::string> cmd = server->splitCMD(buffer);
    std::cout << cmd.size() << std::endl;
    if (cmd[0] == "QUIT")
        if (cmd[1] == ":\r\n")
            if (cmd.size() == 2){
                std::cout << "ah" << std::endl;
                return;
            }  // remove the user for map here
            
    if (cmd[0] == "PRIVMSG")
        setPrvMsg(server, cmd);
    (void)fd;
    
}