/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smia <smia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 01:06:51 by smia              #+#    #+#             */
/*   Updated: 2023/01/25 11:32:31 by smia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 
#include <iostream>
#include <string>
#include "client.hpp"
#include "server.hpp"
#include <map>
class Client;
class Channel
{

    public:
        std::pair<bool, std::string> _pass;
        int _id;
        std::string _name;
        Channel(std::string n, int id)
        {
            _id = id;
            this->_name = n;
            _pass.first = false;
        }
        std::map<std::string, Client> _memebers; // key NickName
        std::map<std::string,Client>  _operators; // key NickName
        std::string channel_membres(void);
};