/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smia <smia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 19:02:06 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/15 17:55:02 by smia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string.h>
#include <iostream>

class Client{
    private:
        std::string _nickname;
        std::string _name;
        std::string _channel; 
    public:
        char buffer[1024];
        Client(){}
        ~Client() {}
        void setName(std::string);
        void setChannel(std::string);
        void setNickName(std::string);
        std::string getName() const;
        std::string getChannel() const;
        std::string getNickName() const;

};