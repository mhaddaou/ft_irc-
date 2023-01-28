/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smia <smia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 01:06:51 by smia              #+#    #+#             */
/*   Updated: 2023/01/28 10:52:12 by smia             ###   ########.fr       */
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
        bool is_channel_client(int fd);
        bool is_admin(int fd);
        
};

int createNewChannel(Server *server, std::vector<std::string> buffer, int fd);
int checkChannel(Server *server, std::string name);
int joinToExistingChannel(Server *server, std::vector<std::string> buffer, int fd);