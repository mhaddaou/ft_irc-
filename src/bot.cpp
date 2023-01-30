/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smia <smia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 21:28:06 by smia              #+#    #+#             */
/*   Updated: 2023/01/30 21:31:48 by smia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"
#include <ctime>
void botHelp(Server *server, std::string buffer, int fd)
{
    std::vector<std::string>cmd = server->splitCMD(buffer, ' ');
    std::string rpl;

    if (cmd[1] == "!help")
    {
        rpl = "300 * BOT: Available commands: !time.\r\n";
        send(fd, rpl.c_str(), rpl.size(), 0);
        return ;
    }
    if (cmd[1] == "!time")
    {
        // timeval currTime;
        // gettimeofday(&currTime, NULL);
        // double  timeSpend = currTime.tv_sec - server->map_clients[fd].timejoin.tv_sec;
        // std::string time = std::to_string(timeSpend);
        // rpl = "300 * BOT: Time in second you spend in Server: "  + time  + ".\r\n";
        // send(fd, rpl.c_str(), rpl.size(), 0);
        // return ;
    }
    if (cmd[1] == "!date")
    {
        
        // rpl = "300 * BOT: Current time: "  + time  + ".\r\n";
        send(fd, rpl.c_str(), rpl.size(), 0);
    }
    
}