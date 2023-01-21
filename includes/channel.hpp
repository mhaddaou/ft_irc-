/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smia <smia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 01:06:51 by smia              #+#    #+#             */
/*   Updated: 2023/01/21 00:46:49 by smia             ###   ########.fr       */
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
        std::string _name;
        std::string _role;
        Channel(std::string n, int id)
        {
            _id = id;
            this->_name = n;
            _pass.first = false;
        }
        Channel(std::string n, std::string pass, int id)
        {
            _id = id;
            this->_name = n;
            this->_pass.first = true;
            this->_pass.second = pass;
        }
};