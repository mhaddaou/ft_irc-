/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smia <smia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 10:23:54 by smia              #+#    #+#             */
/*   Updated: 2023/01/25 11:28:59 by smia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/channel.hpp"

std::string Channel::channel_membres(void)
{
    std::string ret = "";
    for (std::map<std::string,  Client>::const_iterator it = _memebers.begin(); it != _memebers.end(); ++it) 
    {
            ret.append(it->first);
            ret.append(" ");

    }
    for (std::map<std::string, Client>::const_iterator it = _operators.begin(); it != _operators.end(); ++it)
    {
        ret +=  "@" ;
        ret.append(it->first);
            ret += " ";
    }
    ret.erase(ret.begin() + ret.size() - 1);
    return ret;
}
        