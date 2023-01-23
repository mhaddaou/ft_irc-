/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou <mhaddaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 19:02:06 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/23 12:24:53 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string.h>
#include <iostream>
#include "channel.hpp"
# include <sys/socket.h>
# include <netinet/in.h>
#include <vector>

class Client{
    private:
        std::string _nickname;
        std::string _name;
        std::string _channel;
        std::string _realName;
        std::string _password;
    public:
        char buffer[1024];
        int verif;
        // int fd;
        int id;
        Client& operator=(Client const &cp)
        {
            _nickname = cp.getNickName();
            _name = cp.getName();
            _password = cp.getPassword();
            is_verified = cp.is_verified;
            verif = cp.verif;
            return (*this);
        }
        Client(Client const &cp)
        {
            *this = cp;   
        }
        Client();
        ~Client() {}
        void setName(std::string);
        struct sockaddr_in client_address;
        void setChannel(std::string);
        void setPassword(std::string);
        void setNickName(std::string);
        void setRealName(std::string);
        std::string getName() const;
        int inco;
        std::string getPassword()  const;
        std::string getRealName() const;
        std::string getChannel() const;
        std::string getNickName() const;
        void incrementVerf(void) { ++verif;}
        bool is_verified;
        bool isClient;
        std::vector<Channel>  channels;
};