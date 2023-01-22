/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou <mhaddaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 11:10:21 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/22 13:25:18 by mhaddaou         ###   ########.fr       */
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

int nick(Server *server,std::vector<std::string> cmd, int fd, int i){
    bool check = false;
    std::string rpl;
    if (server->isClient(cmd[0]) == EXIT_SUCCESS)
        check = true;
    if (cmd[0] == "NICK"){
        if (cmd.size() == 2)
            cmd[1].erase(std::remove(cmd[1].begin(), cmd[1].end(), '\n'), cmd[1].cend());
            cmd[1].erase(std::remove(cmd[1].begin(), cmd[1].end(), '\r'), cmd[1].cend());
            if (server->map_clients.size() > 0){
                for (iterator it = server->map_clients.begin(); it != server->map_clients.end(); it++){
                    if (it->second.getNickName() == cmd[1]){
                        if (check == true)
                            rpl = ":localhost 433 " + it->second.getNickName() + ": Nickname is already in use\r\n";
                        else
                            rpl = "Nickname is already in use";
                        send(fd, rpl.c_str(), rpl.size(), 0);
                        desconectedClient(server, fd, false);
                        server->fds.erase(server->fds.begin() + i);
                        std::cout << "NICK IS USED" << std::endl;
                        return (EXIT_FAILURE);
                    }
                }
            }
    server->map_clients[fd].setNickName(cmd[1]);
    std::cout << "NICK OK" << std::endl; 
    } 
    return (EXIT_SUCCESS);
}  

int user(Server *server, std::vector<std::string> cmd, int fd, int i){
    (void)i;
    if (cmd[0] == "USER"){
        if (cmd.size() == 5){
            server->map_clients[fd].setName(cmd[1]);
            server->map_clients[fd].setRealName(cmd[4]);
            server->map_clients[fd].is_verified = true;
            std::cout << "USER OK" << std::endl;
            return (EXIT_SUCCESS);
        }
        else{
            std::cout << "USER ERROR" << std::endl;
            std::string rpl = ":localhost 461 . : Not enough parameters \r\n";
            send(fd, rpl.c_str(), rpl.size(), 0);
        }
    }
    return (EXIT_FAILURE);
}

int passwd(Server *server, std::vector<std::string> cmd,  int fd, int i)
{
    int check = false;
    std::string rpl;
    if (server->isClient(cmd[1]) == EXIT_SUCCESS)
        check = true;
    if (cmd[0] == "PASS")
    {
        cmd[1].erase(std::remove(cmd[1].begin(), cmd[1].end(), '\n'), cmd[1].cend());
        cmd[1].erase(std::remove(cmd[1].begin(), cmd[1].end(), '\r'), cmd[1].cend());
        //check if password
        if (cmd.size() == 2)
        {
            if (server->getPass() == server->encrypt(cmd[1]))
                std::cout << "PASS OK" << std::endl;
            else{
                if (check == true)
                    rpl = ":localhost 464 . : Password incorrect\r\n";
                else
                    rpl = "Password incorrect\n";
                send(fd, rpl.c_str(), rpl.size(), 0);
                desconectedClient(server, fd, false);
                server->fds.erase(server->fds.begin() + i);
                std::cout << "PASS INCORRECT" << std::endl;
                return (EXIT_FAILURE);
            }
        }
    }
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
    
int connect (Server *server,char *buffer, int fd, int i)
{
    std::vector<std::string> cmd = server->splitCMD(buffer);
    if (passwd(server, cmd, fd, i) == EXIT_FAILURE)
        return (EXIT_FAILURE);
    if (nick(server,cmd, fd, i) == EXIT_FAILURE)
        return (EXIT_FAILURE);
    if (user(server, cmd, fd, i) == EXIT_FAILURE)
        return (EXIT_FAILURE);
    if (server->map_clients[fd].is_verified == true){
        std::string rpl = ":localhost 001 " + server->map_clients[fd].getNickName() + " : welcome to the server \r\n";
        send(fd, rpl.c_str(), rpl.size(), 0);
    }
    return (EXIT_SUCCESS); 
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

void joinChannel(Server* server, std::vector<std::string> cmd)
{
    static int id = -1;
    
    int idChannel = -1;
    for (size_t i = 0; i < server->channels.size(); ++i)
    {
        if (server->channels[i]._name == cmd[1])
        {
            idChannel = server->channels[i]._id;
        }
    }
    if (idChannel == -1)
    {
        ++id;
        Channel ch(cmd[1], id);
        server->channels.push_back(ch);
    }
    else
    {
        for (iterator it = server->map_clients.begin(); it != server->map_clients.end(); ++it)
        {
            for (size_t i = 0; i < it->second.channels.size(); ++i)
            {
                if (idChannel == it->second.channels[i]._id)
                {
                    // send msg to client it;
                }
            }
        }
    }
}

void handleCmd(Server *server, char *buffer, int fd)
{
    std::cout << buffer << std::endl;
    std::string hel = buffer;
    std::vector<std::string> cmd = server->splitCMD(buffer);
    std::cout << cmd.size() << std::endl;
    if (cmd[0] == "QUIT" && cmd[1] == ":\r\n" && cmd.size() == 2)
    {
        // server->map_clients.erase(fd);
        // remove the user for map here
        return;   
    }
    (void)fd;
    if (cmd[0] == "PRIVMSG")
        setPrvMsg(server, cmd);
    if (cmd[0] == "JOIN")
        joinChannel(server, cmd);
        

}

int Server::isClient(std::string str){
     for (size_t i = 0; i < str.size(); i++){
        if ( str[i]== '\r')
            return EXIT_SUCCESS;
    }
    return (EXIT_FAILURE);
}