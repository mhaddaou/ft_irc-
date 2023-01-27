/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smia <smia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 01:06:51 by smia              #+#    #+#             */
/*   Updated: 2023/01/27 16:39:57 by smia             ###   ########.fr       */
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
        std::string _password;
        Channel();
        ~Channel();
        
        std::vector<int> _fds;
        std::vector<int> _members;
        std::vector<int> _operators;
        void kick_member(int fd, Server* server);
        
};

int createNewChannel(Server *server, std::vector<std::string> buffer, int fd);
int checkChannel(Server *server, std::string name);
void joinToExistingChannel(Server *server, std::vector<std::string> buffer, int fd);