/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smia <smia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 16:17:29 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/17 18:47:16 by smia             ###   ########.fr       */
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
    this->_password= n;
}

std::string Client::getPassword() const
{
    return this->_password;
}

Client::Client() : verified(false), verif(0) 
{
}