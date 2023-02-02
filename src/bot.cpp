/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou <mhaddaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 21:28:06 by smia              #+#    #+#             */
/*   Updated: 2023/02/02 19:07:56 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"
#include <ctime>
void botHelp(Server *server, std::string buffer, int fd)
{
    std::vector<std::string>cmd = server->splitCMD(buffer, ' ');
    std::string rpl;
    if (cmd.size() == 2){
        if (cmd[1] == "!help")
        {
            rpl = "300 * BOT: Available commands: BOT + {!time , !list}.\r\n";
            send(fd, rpl.c_str(), rpl.size(), 0);
            return ;
        }
        if (cmd[1] == "!time")
        {
            timeval currTime;
            gettimeofday(&currTime, NULL);
            int  timeSpend = (int)(currTime.tv_sec - server->map_clients[fd].timejoin.tv_sec);
            std::string time = std::to_string(timeSpend);
            rpl = "300 * BOT: Time  you spend in Server: "  + time  + " secound.\r\n";
            send(fd, rpl.c_str(), rpl.size(), 0);
            return ;
        }
         if (cmd[1] == "!list")
        {
            std::vector<std::string>  channels = server->map_clients[fd].Name_Channels;
            std::string ch  = "";
            for (size_t i = 0; i < channels.size(); i++)
            {
                ch += channels[i] + " ";
            }
            rpl = "300 * BOT: Channels  you have joined are: "  + ch  + ".\r\n";
            send(fd, rpl.c_str(), rpl.size(), 0);
        }
        else{
            rpl = "300 * BOT: Available commands: BOT + {!time , !list}.\r\n";
            send(fd, rpl.c_str(), rpl.size(), 0);
        }
    }
    else
    {
        rpl = "300 * BOT: Available commands: BOT + {!time , !list}.\r\n";
        send(fd, rpl.c_str() , rpl.size(), 0);
    }
        
}