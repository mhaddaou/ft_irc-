/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smia <smia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 01:06:51 by smia              #+#    #+#             */
/*   Updated: 2023/01/23 16:07:30 by smia             ###   ########.fr       */
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
        Channel(std::string n, int id)
        {
            _id = id;
            is_connected = false;
            this->_name = n;
            _pass.first = false;
        }
        Channel(std::string n, std::string pass, int id)
        {
            _id = id;
            this->_name = n;
            is_connected = false;
            this->_pass.first = true;
            this->_pass.second = pass;
        }
};