/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smia <smia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 10:23:54 by smia              #+#    #+#             */
/*   Updated: 2023/01/28 10:51:54 by smia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/channel.hpp"

Channel::Channel(){}
Channel::~Channel(){}

bool Channel::is_channel_client(int fd)
{
    for (size_t i = 0; i < this->_fds.size(); i++)
    {
        if (this->_fds[i] == fd)
            return true;
    }
    return false;
}

bool Channel::is_admin(int fd)
{
    for (size_t i = 0; i < this->_operators.size(); i++)
    {
        if (this->_operators[i] == fd)
            return true;
    }
    return false;
}

void Channel::kick_member(int fd, Server* server)
{
    Client client = server->map_clients[fd];
    // if channel empty 
    if (this->_fds.size() == 1)
    {
        std::string rpl = ":" + client.client_info() + " PART " + this->_name + "\r\n";
        send(fd, rpl.c_str(), rpl.size(), 0);
        this->_fds.erase(this->_fds.begin());
        return ;
    }
    for (size_t i = 0; i < this->_fds.size(); ++i)
    {
        if (this->_fds[i] == fd)
            this->_fds.erase(this->_fds.begin() + i);
    }
    for (size_t i = 0; i < this->_members.size(); ++i)
    {
        if (this->_members[i] == fd)
            this->_members.erase(this->_members.begin() + i);
    }
    for (size_t i = 0; i < this->_operators.size(); ++i)
    {
        if (this->_operators[i] == fd)
            this->_operators.erase(this->_operators.begin() + i);
    }
    std::string rpl = ":" + client.client_info() + " PART " + this->_name + "\r\n";
    send(fd, rpl.c_str(), rpl.size(), 0);
    rpl.clear();
    rpl = client.client_info() + " PART " + this->_name + "\r\n";
    // send to all memebres that one client PART
    for (size_t i = 0; i < this->_fds.size(); ++i)
    {
        send(this->_fds[i], rpl.c_str(), rpl.size(), 0);
    }
}

int checkIfThereIsPass(std::vector<std::string> buffer)
{
    if (buffer.size() > 2)
        return (EXIT_SUCCESS);
    return EXIT_FAILURE;
}

int createNewChannel(Server *server, std::vector<std::string> buffer, int fd)
{
    std::string rpl;
    
    Channel Ch;
    if (checkIfThereIsPass(buffer) == EXIT_SUCCESS){
        Ch._name = buffer[1];
        Ch._password = buffer[2];
        Ch._pass = true;
        Ch._limit = 10;
        Ch._operators.push_back(fd);
        Ch._fds.push_back(fd);
        server->map_clients[fd].id_channels.push_back(server->_id_channel);
        server->map_clients[fd].Name_Channels.push_back(buffer[1]);
    }
    else{
        Ch._name = buffer[1];
        Ch._pass = false;
        Ch._limit = 10;
        Ch._operators.push_back(fd);
        Ch._fds.push_back(fd);
        server->map_clients[fd].id_channels.push_back(server->_id_channel);
        server->map_clients[fd].Name_Channels.push_back(buffer[1]);
    }
    server->map_channels.insert(std::make_pair(buffer[1] , Ch));
    server->Channels.push_back(Ch._name);
    server->_id_channel++;
    //send to the client that the channel is created
    rpl = ":" +server->map_clients[fd].client_info() + " JOIN " + buffer[1] + "\r\n"
        + ":loclahost" + " MODE " + buffer[1] + " +nt\r\n"
        + ":localhost" + " 353 " + server->map_clients[fd].getNickName() + " = " + buffer[1] + " :" + server->map_clients[fd].getNickName() + "\r\n"
        + ":localhost" + " 366 " + server->map_clients[fd].getNickName() + " " + buffer[1] + " :End of /NAMES list\r\n";
        send(fd, rpl.c_str(), rpl.size(), 0);
    return (EXIT_SUCCESS);
}

int checkChannel(Server *server, std::string name)
{
    for (size_t i = 0; i < server->Channels.size(); i++){
        if (server->Channels[i] == name)
            return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

int joinToExistingChannel(Server *server, std::vector<std::string> buffer, int fd)
{
    Channel channel = server->map_channels[buffer[1]];

     // check if password is correct in case channel has one
    if (channel._pass == true)
    {
        if (channel._password != buffer[2])
        {
            std::string rpl = "475 * " + channel._name + " :Cannot join channel (+k)\r\n";
            send(fd, rpl.c_str(), rpl.size(), 0);
            return EXIT_FAILURE;
        }
    }
    // check if fd client is already joined the channel  
    for (size_t i = 0; i < channel._fds.size(); i++)
    {
        if (channel._fds[i] == fd) 
        {
            std::string rpl = "443 * " + server->map_clients[fd].getNickName() + " " + channel._name + " :is already on channel\r\n";
            send(fd, rpl.c_str(), rpl.size(), 0);
            return (EXIT_FAILURE);
        }
    }
    // check if channel riched max size
    if (channel._fds.size() > (size_t)channel._limit) 
    {
        std::string rpl = "471 * " + channel._name + " :Cannot join channel (+l)\r\n";
        send(fd, rpl.c_str(), rpl.size(), 0);
        return (EXIT_FAILURE);
    }
    // add fd client to the channel

    std::string rpl;
    rpl = ":" + server->map_clients[fd].client_info()+ " JOIN " + buffer[1] + "\r\n"
    ":localhost 332 " + server->map_clients[fd].getNickName() + " " + buffer[1] + " :This is my cool channel! https://irc.com\r\n"
    ":localhost 333 " + server->map_clients[fd].getNickName() + " " + buffer[1] + " " + server->map_clients[fd].getNickName() +"!" +server->map_clients[fd].getChannel() +"@localhost"  " 1547691506\r\n"
    ":localhost 353 " + server->map_clients[fd].getNickName() + " @ " + buffer[1] + " :" + server->map_clients[fd].getNickName() + " @"+ server->map_clients[fd].getNickName() + "\r\n"
    ":localhost 366 " + server->map_clients[fd].getNickName() + " " + buffer[1] + " :End of /NAMES list\r\n";
    send(fd, rpl.c_str(), rpl.size(), 0);
    for (IteratorChannel it = server->map_channels.begin(); it != server->map_channels.end(); it++)
    {
        if (it->second._name == buffer[1])
        {
            it->second._fds.push_back(fd);
            it->second._members.push_back(fd);
            rpl = ":" + server->map_clients[fd].client_info()+ " JOIN " + buffer[1] + "\r\n";
            for(size_t j = 0; j < it->second._fds.size(); j++)
            {
                if (it->second._fds[j] != fd)
                    send(it->second._fds[j], rpl.c_str(), rpl.size(), 0);
            }
        }
    }
    return (EXIT_SUCCESS);
}