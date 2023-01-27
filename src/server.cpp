/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smia <smia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 11:10:21 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/27 18:22:18 by smia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

Server::Server(char * port, char * passwd){
    this->_port = atoi(port);
    this->_passwd = this->encrypt(passwd);
    this->_id_channel = 0;
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
    int reuse = 1;
    setsockopt(this->serverfd, SOL_SOCKET, SO_REUSEPORT, (const void *)&reuse, sizeof(int));
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

std::vector<std::string> Server::splitCMD(std::string msg, char l){
    
    std::vector<std::string> cmd;
    std::stringstream ss(msg);
    std::string word;
    while (getline(ss, word, l)) { 
    cmd.push_back(word);
    }
    return (cmd);
}

int Server::getPort(){
    return (this->_port);
}

int nick(Server *server,std::vector<std::string> cmd, int fd, int i){
    std::string rpl;
    if (cmd[0] == "NICK"){
        if (cmd.size() == 2)
            // cmd[1].erase(std::remove(cmd[1].begin(), cmd[1].end(), '\n'), cmd[1].cend());
            // cmd[1].erase(std::remove(cmd[1].begin(), cmd[1].end(), '\r'), cmd[1].cend());
            if (server->map_clients.size() > 0){
                for (Iterator it = server->map_clients.begin(); it != server->map_clients.end(); it++){
                    if (it->second.getNickName() == cmd[1]){
                        rpl = ":localhost 433 " + it->second.getNickName() + ": Nickname is already in use\r\n";
                        send(fd, rpl.c_str(), rpl.size(), 0);
                        desconectedClient(server, fd, i);
                        std::cout << "NICK IS USED" << std::endl;
                        return (EXIT_FAILURE);
                    }
                }
            }
    server->map_clients[fd].setNickName(cmd[1]);
    std::cout << "NICK OK" << std::endl; 
    server->map_clients[fd].incrementVerf();
    } 
    
    return (EXIT_SUCCESS);
}  

int user(Server *server, std::vector<std::string> cmd, int fd, int i){
    if (cmd[0] == "USER"){
        if (cmd.size() == 5){
            server->map_clients[fd].setName(cmd[1]);
            server->map_clients[fd].setRealName(cmd[4]);
            server->map_clients[fd].is_verified = true;
            // server->map_clients[server->fds[i]].verified = true;
            std::cout << "USER OK" << std::endl;
            server->map_clients[fd].incrementVerf();
            return (EXIT_SUCCESS);
        }
        else{
            std::cout << "USER ERROR" << std::endl;
            std::string rpl = ":localhost 461 . : Not enough parameters \r\n";
            send(fd, rpl.c_str(), rpl.size(), 0);
            desconectedClient(server, fd, i);
        }
    }
    return (EXIT_FAILURE);
}

int passwd(Server *server, std::vector<std::string> cmd,  int fd, int i)
{
    std::string rpl;
    if (cmd[0] == "PASS")
    {
        // cmd[1].erase(std::remove(cmd[1].begin(), cmd[1].end(), '\n'), cmd[1].cend());
        // cmd[1].erase(std::remove(cmd[1].begin(), cmd[1].end(), '\r'), cmd[1].cend());
        //check if password
        if (cmd.size() == 2)
        {
            if (server->getPass() == server->encrypt(cmd[1]))
                std::cout << "PASS OK" << std::endl;
            else{
                rpl = ":localhost 464 . : Password incorrect\r\n";
                send(fd, rpl.c_str(), rpl.size(), 0);
                desconectedClient(server, fd, i);
                std::cout << "PASS INCORRECT" << std::endl;
                return (EXIT_FAILURE);
            }
        }
    }
    server->map_clients[fd].incrementVerf();
    return (EXIT_SUCCESS);
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
    
int connect (Server *server,std::string buffer, int fd, int i)
{
    std::string rpl;
    if (server->isClient(buffer) == EXIT_SUCCESS)
        server->map_clients[fd].isClient = true;
    buffer.erase(std::remove(buffer.begin(), buffer.end(), '\n'), buffer.cend());
    buffer.erase(std::remove(buffer.begin(), buffer.end(), '\r'), buffer.cend());
    std::vector<std::string> cmd = server->splitCMD(buffer, ' ');
    checkIsRoot(server, server->map_clients[server->fds[i]].buffer, server->fds[i]);
    if (server->map_clients[fd].verif == 0)
        passwd(server, cmd, fd, i);
    if (server->map_clients[fd].verif == 1)
        nick(server, cmd, fd, i);
    if (server->map_clients[fd].verif == 2)
        user(server, cmd, fd, i);
    if (server->map_clients[fd].verif == 3){
        std::cout << "fd "<<fd << std::endl;
        if (server->map_clients[fd].isClient == true)
            rpl = ":localhost 001 " + server->map_clients[fd].getNickName() + " : welcome to the server \r\n";
        else
            rpl = "welcome to the server\n";
        send(fd, rpl.c_str(), rpl.size(), 0);
        server->map_clients[server->fds[i]].is_verified = true;
        std::cout <<"• " << server->map_clients[fd].getNickName() << " is connected" << std::endl;
    }
    return (EXIT_SUCCESS); 
}

int setPrvMsg(Server *server, std::vector<std::string> cmd, int fd){
    int fdTarget;
    std::string rpl;
    std::string msg;
    if (cmd[1][0] == '#')
    {
        std::cout << "hani" << std::endl;
        for (size_t i = 0; i < server->Channels.size(); i++)
        {
            if (server->Channels[i] == cmd[1]){
                
                for (IteratorChannel it = server->map_channels.begin(); it != server->map_channels.end(); it++)
                {
                    if (it->second._name == cmd[1])
                    {
                        msg = handlemsg(cmd);
                        rpl = ":" + server->map_clients[fd].getNickName() + " PRIVMSG " + it->second._name+ " : " + msg;
                        for(size_t j = 0; j < it->second._fds.size(); j++)
                        {
                            if (it->second._fds[j] != fd)
                                send(it->second._fds[j], rpl.c_str(), rpl.size(), 0);   
                        }
                    }
                }
            }
        }
        return 1;
    }
    if (cmd.size() < 3){
        if (cmd.size() < 2){
            if (server->map_clients[fd].isClient == true)
                rpl = ":localhost 461 "+ server->map_clients[fd].getNickName() +": Not enough parameters \r\n";
            else
                rpl = "Not enough parameters\n";
        }
        else{
            if (server->map_clients[fd].isClient == true)
                rpl = ":localhost 412 " + server->map_clients[fd].getNickName()+ ": No text to send\r\n";
            else
                rpl = "No text to send\n";
        }
        send(fd, rpl.c_str() , rpl.size(), 0);
        return (EXIT_FAILURE);
    }
    for (Iterator it = server->map_clients.begin(); it != server->map_clients.end(); ++it)
    {
        if (it->second.getNickName() == cmd[1]){
            fdTarget = it->first;
            // type of msgprv = :client PRIVMSG sender : message
            msg = handlemsg(cmd);
            if (server->map_clients[fdTarget].isClient == true)
                rpl = ":" + server->map_clients[fd].getNickName() + " PRIVMSG " + it->second.getNickName()+ " : " + msg;
            else
                rpl = "FROM "+ server->map_clients[fd].getNickName() + ": " + msg;
            send(fdTarget, rpl.c_str(), rpl.size() , 0 );
            return (EXIT_SUCCESS);
        }
    }
    rpl = ":localhost 401 " + cmd[1] + " : No such nick/channel\r\n";
    send(fd, rpl.c_str(), rpl.size(), 0);
    return (EXIT_FAILURE);
}
int Server::checkQuit(std::string str){
    std::vector<std::string> cmd = this->splitCMD(str, ' ');
    if (strcmp(cmd[0].c_str(), "QUIT") == 0)
            return (EXIT_SUCCESS);
    return (EXIT_FAILURE);
}


void part(Server *server,std::vector<std::string> cmd, int fd)
{
    if (server->map_channels.find(cmd[1]) != server->map_channels.end())
        server->map_channels[cmd[1]].kick_member(fd, server);
    else
        ; // channel you tryin to PART doesn't exist 
}

void handleCmd(Server *server, std::string buffer, int fd)
{
    std::cout << buffer << std::endl;
    if (server->isClient(buffer) == EXIT_SUCCESS)
        server->map_clients[fd].isClient = true;
    buffer.erase(std::remove(buffer.begin(), buffer.end(), '\n'), buffer.cend());
    buffer.erase(std::remove(buffer.begin(), buffer.end(), '\r'), buffer.cend());
    std::vector<std::string> cmd = server->splitCMD(buffer, ' ');
    if (cmd[0] == "PRIVMSG")
        setPrvMsg(server, cmd, fd);
    if (cmd[0] == "NOTICE")
        setNoticeMsg(server, cmd, fd);
    if (cmd[0] == "WHOIS")
        whoIs(server, cmd, fd);
    if (cmd[0] == "NICK")
        Nick(server, cmd, fd);
    if (cmd[0] == "JOIN")
        join(server, buffer, fd);
    if (cmd[0] == "PART")
        part(server, cmd, fd);
}

int Server::isClient(std::string str){
     for (size_t i = 0; i < str.size(); i++){
        if ( str[i]== '\r')
            return EXIT_SUCCESS;
    }
    return (EXIT_FAILURE);
}