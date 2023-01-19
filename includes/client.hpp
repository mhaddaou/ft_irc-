/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smia <smia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 19:02:06 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/17 18:49:02 by smia             ###   ########.fr       */
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
        std::string _realName;
        std::string _password;
    public:
        char buffer[1024];
        bool verified;
        int verif;
        // int fd;
        int id;
        Client();
        ~Client() {}
        // char buffer[1024];
        void setName(std::string);
        void setChannel(std::string);
        void setPassword(std::string);
        void setNickName(std::string);
        void setRealName(std::string);
        std::string getName() const;
        std::string getPassword()  const;
        std::string getRealName() const;
        std::string getChannel() const;
        std::string getNickName() const;
        void incrementVerf(void) { ++verif;}
        bool is_verified(void)
        {
            if (verif == 3 && !verified)
            {
                verified = true;
                return true;
            }
            return false;
        }
        Client& operator=(Client const &cp)
        {
            _nickname = cp.getNickName();
            _name = cp.getName();
            _password = cp.getPassword();
            verified = cp.verified;
            verif = cp.verif;
            return (*this);
        }
        Client(Client const &cp)
        {
            *this = cp;   
        }
        
};