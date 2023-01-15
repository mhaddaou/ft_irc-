/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smia <smia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 17:47:09 by smia              #+#    #+#             */
/*   Updated: 2023/01/15 17:53:14 by smia             ###   ########.fr       */
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