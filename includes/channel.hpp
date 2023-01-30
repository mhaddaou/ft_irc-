/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smia <smia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 01:06:51 by smia              #+#    #+#             */
/*   Updated: 2023/01/30 21:20:15 by smia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 
#include <iostream>
#include <string>
#include "client.hpp"
#include "server.hpp"
#include <map>
class Client;
class Server;
class Channel
{

    public:
        std::string _name;
        bool _pass;
        int _limit;
        bool _isInvisible;
        bool _secret;
        std::string _password;
        Channel();
        ~Channel();
        
        std::vector<int> _fds;
        std::vector<char> _modes;
        std::vector<int> _members;
        std::vector<int> _operators;
        std::vector<unsigned int> _bans;
        void kick_member(int fd, Server* server, char c);
        bool is_channel_client(int fd);
        bool is_admin(int fd);
        
};

int createNewChannel(Server *server, std::vector<std::string> buffer, int fd);
int checkChannel(Server *server, std::string name);
int joinToExistingChannel(Server *server, std::vector<std::string> buffer, int fd);
std::string getChannels(Server *server, std::string nick);
int indexChennel(std::vector<std::string> channels, std::string name);
int checkIfInvited(Server *server, std::vector<std::string> cmd, int fd);
void part(Server *server, std::string buffer, int fd);
void kick(Server* server, std::string buffer, int fd, char c);
