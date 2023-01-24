/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou <mhaddaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 01:06:51 by smia              #+#    #+#             */
/*   Updated: 2023/01/23 18:52:04 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 
#include <iostream>
#include <string>

class Channel
{
    private:
        // std::vector<std::string> membres;
    public:
        std::pair<bool, std::string> _pass;
        int _id;
        bool is_connected;
        int _owner; // fd
        std::string _name;
        std::string _role;
        Channel() : is_connected(false) {}
        Channel& operator=(Channel const &cp)
        {
            _id = cp._id;
            is_connected = cp.is_connected;
            _name = cp._name;
            _role = cp._role;
            _pass = cp._pass;
            return (*this);
        }
        Channel(int fd, std::string n, int id)
        {
            _owner = fd;
            _id = id;
            is_connected = false;
            this->_name = n;
            _pass.first = false;
        }

};