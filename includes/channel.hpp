/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou <mhaddaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 01:06:51 by smia              #+#    #+#             */
/*   Updated: 2023/01/25 18:14:26 by mhaddaou         ###   ########.fr       */
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
        
};

int createNewChannel(Server *server, std::vector<std::string> buffer, int fd);
int checkChannel(Server *server, std::string name);
void joinToExistingChannel(Server *server, std::vector<std::string> buffer, int fd);