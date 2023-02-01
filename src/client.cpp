/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou <mhaddaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 16:17:29 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/02/01 23:43:47 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/client.hpp"


void Client::setNickName(std::string n)
{
    this->_nickname = n;
}

void Client::setName(std::string n)
{
    this->_name = n;
}


void Client::setChannel(std::string n)
{
    this->_channel = n;
}

std::string Client::getName() const
{
    return this->_name;
}

std::string Client::getChannel() const
{
    return this->_channel;
}

std::string Client::getNickName() const
{
    return this->_nickname;
}
void Client::setRealName(std::string n)
{
    this->_realName = n;
}
std::string Client::getRealName() const
{
    return this->_realName;
}

void Client::setPassword(std::string n)
{
    n.erase(std::remove(n.begin(), n.end(), '\n'), n.end());
    this->_password= n;
}

std::string Client::getPassword() const
{
    return this->_password;
}

Client::Client() : verif(0), is_verified(false) , isClient(false), _ban(false){}