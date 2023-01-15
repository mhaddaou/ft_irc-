/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou < mhaddaou@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 19:02:06 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/14 20:03:11 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string.h>
#include <iostream>

class Client{
    private:
        std::string _nickname;
        std::string _name;
        int _fd;
        std::string _channel; 
    public:
        void setName(std::string);
        void setChannel(std::string);
        void setNickName(std::string);
        void setFd(int fd);
        std::string getName();
        std::string getChannel();
        std::string getNickName();
        int getFd();

};