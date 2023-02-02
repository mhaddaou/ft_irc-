/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou <mhaddaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 13:47:47 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/02/02 18:53:05 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/mode.hpp"
#include "../includes/server.hpp"

int modes(char mode){
    if (mode == 'i' || mode == 'm' || mode == 'n' || mode == 'p' || mode == 's' || mode == 't' || mode == 'l' || mode == 'b' || mode == 'o' || mode == 'v' || mode == '-' || mode == '+')
        return (EXIT_SUCCESS);
    return (EXIT_FAILURE);
}

std::string getMode(std::vector<std::string> cmd){
    int i = 0;
         while (cmd[2][i]){
            if (modes(cmd[2][i]) == EXIT_FAILURE)
                return ("UnknownMODE");
            if ((cmd[2][i] == '+' && cmd[2][i + 1] == '-') || (cmd[2][i] == '-' && cmd[2][i+ 1] == '-') || (cmd[2][i] == '+' && cmd[2][i + 1] == '+') || (cmd[2][i] == '-' && cmd[2][i + 1] == '+'))
                return ("UnknownMODE");
            i++;
         }
    return (cmd[2]);
}
int position(std::string cmd, char c){
    int i = 0;
    while (cmd[i]){
        if (cmd[i] == c)
            return i;
        i++;
    }
    return (-1);
}
void rmModes(Server *server, std::string channel, char rm){
    for (size_t i = 0; i < server->map_channels[channel]._modes.size(); i++)
    {
        if (server->map_channels[channel]._modes[i] == rm)
            server->map_channels[channel]._modes.erase(server->map_channels[channel]._modes.begin() + i);
    }
}
int checkMode(std::vector<char> _modes, char mode)
{
    if (mode == 'b')
        return (EXIT_SUCCESS);
    for(size_t i = 0; i < _modes.size();i++){
        if (_modes[i] == mode)
            return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

void banUser(Server* server, std::vector<std::string> cmd, int fd)
{
    std::string rpl;
    int target = -1;
    if (cmd.size() == 4){
    for(Iterator it = server->map_clients.begin(); it != server->map_clients.end(); ++it){
        if (it->second.getNickName() == cmd[3])
        {
            target = it->first;
            // :server.name KICK #channel username :reason
            rpl = ":localhost KICK " + cmd[1] + " " + it->second.getNickName() + " :has been banned from this channel.\r\n";
            send(it->first, rpl.c_str(), rpl.size(), 0);
            server->map_channels[cmd[1]]._bans.push_back(it->second._ip);
            server->map_channels[cmd[1]].kick_member(it->first, server, 'b');
            
        }
    }   
    }
    else{
        rpl = ":localhost 461 "+ server->map_clients[fd].getNickName() +": Not enough parameters \r\n";
        send(fd, rpl.c_str() , rpl.size(), 0);
    }
    if (target == -1)
    {
        rpl = ":localhost 401 " + server->map_clients[fd].getNickName() + " : No such nick\r\n";
            send(fd, rpl.c_str(), rpl.size(), 0);
    }
}
int addLimit(Server *server, std::vector<std::string> cmd, int fd){

    (void)fd;
    (void)server;
    for(size_t i = 0; i < cmd[3].size(); i++){
        if (!isdigit(cmd[3][i]))
            return (EXIT_FAILURE);
    }
    server->map_channels[cmd[1]]._limit = atoi(cmd[3].c_str());
    return (EXIT_SUCCESS);
    
}
void rmBan(Server *server, std::vector<std::string> cmd){
    unsigned int ip;
    
    for(Iterator it = server->map_clients.begin(); it != server->map_clients.end(); ++it){
        if (it->second.getNickName() == cmd[3])
        {
            ip = it->second._ip;
            break ;
        }
    }
    for(size_t i = 0; i < server->map_channels[cmd[1]]._bans.size();i++){
        if (ip == server->map_channels[cmd[1]]._bans[i]){
            server->map_channels[cmd[1]]._bans.erase(server->map_channels[cmd[1]]._bans.begin() + i);
            break;
        }
    }
}

void checkMode(Server *server, std::vector<std::string> cmd, int fd){
    std::string mode;
    std::string rpl;
    IteratorChannel it1 = server->map_channels.find(cmd[1]);
    if (it1 == server->map_channels.end())
    {
        rpl = ":localhost 401 " + cmd[1] + " : No such nick/channel\r\n";
        send(fd, rpl.c_str(), rpl.size(), 0);
        return ;
    }
    if (server->map_channels[cmd[1]].is_admin(fd) == false)
    {
        rpl = ":localhost 482 " + server->map_clients[fd].getNickName() + ":You're not channel operator\r\n";
        send(fd, rpl.c_str(), rpl.size(), 0);
        return ;
    }
    if (cmd.size() == 4 || cmd.size() == 3)
    {
        mode = getMode(cmd);
        if (mode == "UnknownMODE"){
            rpl = ":localhost 501 " + server->map_clients[fd].getNickName() + ": Unknown MODE flag \r\n";
            send(fd, rpl.c_str(), rpl.size(), 0);
            return ;
        }
        else{

            for(size_t i = 0; i < mode.size(); i++)
            {
                if (mode[i] == '+')
                {
                    ++i;
                    while(mode[i] != '-' && mode[i] != '\0')
                    {

                        if (checkMode(server->map_channels[cmd[1]]._modes, mode[i]) == EXIT_FAILURE){
                            rpl = ":localhost 500 " + server->map_clients[fd].getNickName() + ": Duplicate modes \r\n";
                            send(fd, rpl.c_str(), rpl.size(), 0);
                            return ;
                        }
                        else
                        {
                            if (mode[i] != 'b' && mode[i] != 'o')
                                server->map_channels[cmd[1]]._modes.push_back(mode[i]);
                            if (mode[i] == 'p')
                                server->map_channels[cmd[1]]._isInvisible = true;
                            if (mode[i] == 's')
                                server->map_channels[cmd[1]]._secret = true;
                            if (mode[i] == 'b')
                                banUser(server, cmd, fd);
                            if (mode[i] == 'o')
                                setUserOperator(server, cmd, fd);
                            if (mode[i] == 'l'){
                                if (addLimit(server, cmd, fd) == EXIT_FAILURE){
                                    rpl = ":localhost 501 " + server->map_clients[fd].getNickName() + ": Unknown MODE flag \r\n";
                                    send(fd, rpl.c_str(), rpl.size(), 0);
                                    return ;
                                }
                            }
                        }
                        ++i;
                    }   
                }
                if (mode[i] == '-')
                {
                    ++i;
                    while(mode[i] != '+' && mode[i] != '\0')
                    {

                        if (checkMode(server->map_channels[cmd[1]]._modes, mode[i]) == EXIT_FAILURE){
                            rmModes(server,cmd[1], mode[i]);
                            if (mode[i] == 'p')
                                server->map_channels[cmd[1]]._isInvisible = false;
                            if (mode[i] == 's')
                                server->map_channels[cmd[1]]._secret = false;
                        }
                        else if (mode[i] == 'b')
                            rmBan(server, cmd);
                        else if (mode[i] == 'o')
                            rmUserOperator(server, cmd, fd);
                        else{
                            rpl = ":localhost 476 " + server->map_clients[fd].getNickName() + ": Unknown MODE flag \r\n";
                            send(fd, rpl.c_str(), rpl.size(), 0);
                            return ;
                        }
                        ++i;
                    }
                }
            }
        }
           
    }
    else if (cmd.size() == 2)
        return ;
    else{
        rpl = ":localhost 461 "+ server->map_clients[fd].getNickName() +": Not found mode \r\n";
        send(fd, rpl.c_str(), rpl.size(),0);
    }
}
int checkModesChannel(Server *server, std::vector<std::string> cmd, int fd){
   std::string rpl;
   std::string modes = "+";
    for (size_t i=0; i<server->map_channels[cmd[1]]._modes.size(); i ++){
        if (server->map_channels[cmd[1]]._modes[i] == 'i')
            modes += 'i';
        if (server->map_channels[cmd[1]]._modes[i] == 's')
            modes += 's';
    }
    if (modes.size() != 1){
        if (checkIfInvited(server, cmd, fd) == EXIT_SUCCESS)
            return (EXIT_SUCCESS);
        rpl = ":localhost 473 " + cmd[1] + " :Cannot join channel (" + modes + ")\r\n";
        send(fd, rpl.c_str(), rpl.size(), 0);
        return (EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}
int setUserOperator(Server *server, std::vector<std::string> cmd, int fd){
    // MODE #channel +o username
    std::string rpl;
    int fdtarget = -1;
    Iterator it = server->map_clients.begin();
    for (; it != server->map_clients.end(); it++)
    {
        if (it->second.getNickName() == cmd[3])
        {
            fdtarget = it->first;
            break ;       
        }
    }
    if (fdtarget == -1)
    {
        rpl = ":localhost 401 " + cmd[3] + " : No such nick/channel\r\n";
        send(fd, rpl.c_str(), rpl.size(), 0);
        return (EXIT_FAILURE); 
    }
    IteratorChannel it1 = server->map_channels.find(cmd[1]);
    if (!it1->second.is_channel_client(fdtarget))
    {
        // :server.name 441 sender recipient #channel :They aren't on that channel
        rpl = ":localhost 441 " + it->second.getNickName() + " " + cmd[3] + " " + cmd[1] + " :They aren't on that channel\r\n";
        send(fd, rpl.c_str(), rpl.size(), 0);
        return (EXIT_FAILURE);
    }
    if (it1->second.is_admin(fdtarget))
    {
        // 485 sender #channel :User is already channel operator
        rpl = ":localhost 485 " + it->second.getNickName() + " " + cmd[1] + " :User is already channel operator\r\n";
        send(fd, rpl.c_str(), rpl.size(), 0);
        return (EXIT_FAILURE);
    }
    server->map_channels[cmd[1]]._operators.push_back(fdtarget);
    for (size_t i = 0; i < server->map_channels[cmd[1]]._members.size(); i++)
    {
        if (server->map_channels[cmd[1]]._members[i] == fdtarget)
        {
            server->map_channels[cmd[1]]._members.erase(server->map_channels[cmd[1]]._members.begin() + i);
            break ;
        }
    }
    // :server.name MODE #channel +o recipient
    rpl = ":localhost MODE " + cmd[1] + " +o " + cmd[3] + "\r\n";
    send(fd, rpl.c_str(), rpl.size(), 0);
    return (EXIT_FAILURE);
}
int rmUserOperator(Server *server, std::vector<std::string> cmd, int fd){
    // MODE #channel -o username
    std::string rpl;
    int fdtarget = -1;
    Iterator it = server->map_clients.begin();
    for (; it != server->map_clients.end(); it++)
    {
        if (it->second.getNickName() == cmd[3])
        {
            fdtarget = it->first;
            break ;       
        }
    }
    if (fdtarget == -1)
    {
        rpl = ":localhost 401 " + cmd[3] + " : No such nick/channel\r\n";
        send(fd, rpl.c_str(), rpl.size(), 0);
        return (EXIT_FAILURE); 
    }
    IteratorChannel it1 = server->map_channels.find(cmd[1]);
    if (!it1->second.is_channel_client(fdtarget))
    {
        // :server.name 441 sender recipient #channel :They aren't on that channel
        rpl = ":localhost 441 " + it->second.getNickName() + " " + cmd[3] + " " + cmd[1] + " :They aren't on that channel\r\n";
        send(fd, rpl.c_str(), rpl.size(), 0);
        return (EXIT_FAILURE);
    }
    if (!it1->second.is_admin(fdtarget))
    {
        rpl = ":localhost 482 " + server->map_clients[fdtarget].getNickName() + ":Is not channel operator\r\n";
        send(fd, rpl.c_str(), rpl.size(), 0);
        return (EXIT_FAILURE);
    }
    server->map_channels[cmd[1]]._members.push_back(fdtarget);
    for (size_t i = 0; i < server->map_channels[cmd[1]]._operators.size(); i++)
    {
        if (server->map_channels[cmd[1]]._operators[i] == fdtarget)
        {
            server->map_channels[cmd[1]]._operators.erase(server->map_channels[cmd[1]]._operators.begin() + i);
            break ;
        }
    }
    // :server.name MODE #channel +o recipient
    rpl = ":localhost MODE " + cmd[1] + " -o " + cmd[3] + "\r\n";
    send(fd, rpl.c_str(), rpl.size(), 0);
    return (EXIT_FAILURE);
}